#include "std_include.hpp"
#include "lua.hpp"

namespace game::lua
{
	class reader
	{
	public:
		reader(const std::string& buffer) : buffer_(buffer)
		{
		}

		template <typename T>
		T read()
		{
			T result{};
			read_data(&result, sizeof(result));
			return result;
		}

		template <typename T>
		void read(T* result)
		{
			read_data(result, sizeof(*result));
		}

		void read_data(void* buffer, const size_t size)
		{
			if (buffer_.size() < offset_ + size)
			{
				throw std::runtime_error("Buffer overrun.");
			}

			std::memmove(buffer, buffer_.data() + offset_, size);
			offset_ += size;
		}

	private:
		size_t offset_ = 0;
		const std::string& buffer_;
	};

	void script::parse_header(reader& r)
	{
		header_ = r.read<header>();

		if (std::memcmp(header_.dump_signature, HKS_DUMP_SIGNATURE, 4))
		{
			throw std::runtime_error("Invalid lua magic.");
		}

		if (header_.lua_version != LUA_VERSION)
		{
			throw std::runtime_error("Invalid lua version.");
		}

		if (header_.format_version != FORMAT_VERSION)
		{
			throw std::runtime_error("Invalid format version.");
		}

		if (!header_.is_little_endian)
		{
			throw std::runtime_error("Invalid endianness.");
		}

		if (header_.int_size != 4            //
			|| header_.size_t_size != 8      //
			|| header_.instruction_size != 4 //
			|| header_.number_size != 4)
		{
			throw std::runtime_error("Invalid number size.");
		}

		if (header_.build_settings != (HKS_GETGLOBAL_MEMOIZATION | HKS_STRUCTURE_EXTENSION_ON))
		{
			throw std::runtime_error("Invalid build settings.");
		}
	}

	script script::parse(const std::string& buffer)
	{
		// See hks::BytecodeReader::readBytecode
		
		reader r(buffer);

		script s{};
		s.parse_header(r);
		r.read(&s.constant_type_count);

		return s;
	}
}

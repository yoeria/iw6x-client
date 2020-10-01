#include "std_include.hpp"
#include "lua.hpp"

namespace game::lua
{
	namespace
	{
		class reader
		{
		public:
			reader(const std::string& buffer) : buffer_(buffer) {}

			template <typename T>
			T read()
			{
				T result{};
				read_data(&result, sizeof(result));
				return result;
			}

			void read_data(void* buffer, const size_t size)
			{
				if(buffer_.size() < offset_ + size)
				{
					throw std::runtime_error("Buffer overrun.");
				}

				std::memmove(buffer, buffer_.data(), size);
				offset_ += size;
			}

		private:
			size_t offset_ = 0;
			const std::string& buffer_;
		};
	}
	
	script script::parse(const std::string& buffer)
	{
		reader reader(buffer);
		
		script s{};
		s.header_ = reader.read<header>();

		if(std::memcmp(s.header_.magic, "\x1B" "Lua", 4))
		{
			throw std::runtime_error("Invalid lua magic.");
		}

		return s;
	}
}

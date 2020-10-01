#pragma once

#include "game/structs.hpp"

namespace game::lua
{
#pragma pack(push)
#pragma pack(1)
	struct header
	{
		uint8_t magic[4];

		uint8_t lua_version;
		uint8_t compiler_version;
		uint8_t endianness;
		uint8_t size_of_int;
		uint8_t size_of_size_t;
		uint8_t size_of_instruction;
		uint8_t size_of_lua_number;
		uint8_t integral_flag;

		uint8_t pad[5];
		uint8_t constant_type_count;
	};
#pragma pack(pop)

	class script
	{
	public:
		~script() = default;

		script(script&&) = default;
		script(const script&) = default;
		script& operator=(script&&) = default;
		script& operator=(const script&) = default;

		static script assemble(const std::string& buffer);
		static script parse(const std::string& buffer);

		std::string disassemble();
		std::string write();

	private:
		header header_{};

		script() = default;
	};
}

#pragma once

#include "game/structs.hpp"

namespace game::lua
{
#define HKS_DUMP_SIGNATURE "\x1BLua"
#define LUA_VERSION 81
#define FORMAT_VERSION 13

	enum build_settings : uint8_t
	{
		HKS_GETGLOBAL_MEMOIZATION = 0x1,
		HKS_STRUCTURE_EXTENSION_ON = 0x2,
		HKS_SELF = 0x4,
		HKS_WITHDOUBLES = 0x8,
		HKS_WITHNATIVEINT = 0x10
	};

#pragma pack(push)
#pragma pack(1)
	struct header
	{
		uint8_t dump_signature[4];
		uint8_t lua_version;
		uint8_t format_version;
		uint8_t is_little_endian;
		uint8_t int_size;
		uint8_t size_t_size;
		uint8_t instruction_size;
		uint8_t number_size;
		uint8_t integral_flag;
		uint8_t build_settings;
		bool referenced_bytecode;
	};
#pragma pack(pop)

	class reader;

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
		uint32_t constant_type_count;

		script() = default;
		void parse_header(reader& r);
	};
}

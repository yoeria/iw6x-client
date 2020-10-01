#include <std_include.hpp>

#include "game/game.hpp"

#include "command.hpp"
#include "game_console.hpp"

#include "utils/io.hpp"
#include "utils/string.hpp"

#include "game/lua/lua.hpp"

namespace
{
	utils::hook::detour load_lua_file_asset_hook;
	std::unordered_map<std::string, std::string> lua_buffer;

	void load_lua_file_asset(game::native::LuaFile** file)
	{
		auto* lua_file = *file;

		if (lua_file && lua_file->name)
		{
			const auto buf = utils::io::read_file(utils::string::va("iw6x/%s", lua_file->name));

			if (!buf.empty())
			{
				lua_buffer[lua_file->name] = buf;

				lua_file->len = static_cast<int>(lua_buffer[lua_file->name].length());
				lua_file->buffer = lua_buffer[lua_file->name].data();
			}
		}

		load_lua_file_asset_hook.invoke<void>(file);
	}
}

class lua final : public module
{
public:
	void post_start() override
	{
#if DEV_BUILD
		const auto file = utils::io::read_file("cacloadoututils.lua");
		game::lua::script::parse(file);
#endif
	}
	
	void post_unpack() override
	{
		if (game::is_dedi()) return;

		command::add("lui_open", [](command::params params)
		{
			if (params.size() <= 1)
			{
				game_console::print(7, "usage: lui_open <name>\n");
				return;
			}

			game::native::LUI_OpenMenu(0, params[1], 1, 0, 0);
		});

		load_lua_file_asset_hook.create(0x140325C70, &load_lua_file_asset);
	}
};

REGISTER_MODULE(lua);

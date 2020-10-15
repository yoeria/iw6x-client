#include <std_include.hpp>
#include "loader/module_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "utils/io.hpp"
#include "utils/hook.hpp"
#include "utils/string.hpp"

#include "game/lua/lua.hpp"

namespace lui
{
	namespace
	{
		utils::hook::detour load_lua_file_asset_hook;
		std::unordered_map<std::string, std::string> lua_buffer;

		void load_lua_file_asset(game::LuaFile** file)
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

	class module final : public module_interface
	{
	public:
		void post_start() override
		{
#if DEV_BUILD
			const auto file = utils::io::read_file("iw6x/default.lua");
			game::lua::script::parse(file);
#endif
		}
		
		void post_unpack() override
		{
			if (game::environment::is_dedi()) return;

			load_lua_file_asset_hook.create(0x140325C70, &load_lua_file_asset);
		}
	};
}

REGISTER_MODULE(lui::module)

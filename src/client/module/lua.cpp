#include <std_include.hpp>
#include "lua.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "command.hpp"
#include "game_console.hpp"

#include "utils/string.hpp"

void lua::post_unpack()
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
}

REGISTER_MODULE(lua);
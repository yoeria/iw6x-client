#pragma once
#include "loader/module_loader.hpp"
#include "utils/hook.hpp"

#include "game/game.hpp"

class lua final : public module
{	
public:
	void post_unpack() override;

private:

};

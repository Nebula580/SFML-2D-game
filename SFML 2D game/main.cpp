#include <iostream>
#include "Game.h"

int main()
{
	// Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	Game game;

	// Game loop
	while (game.running() && !game.getEndGame())
	{
		game.update();
		game.render();
	}

	return 0;  // End app
}
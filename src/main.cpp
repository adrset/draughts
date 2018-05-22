#include <cstdlib>
#include <string>
#include "Game.h"

int main(int argc, char** argv) {
	int fps = 60;
	if(argc >=2)
		fps = std::atoi(argv[1]);

	Game* game = new Game(800, 800, "Potts", fps);
	game->start();

	delete game;

	return 0;
}

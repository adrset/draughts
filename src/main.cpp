#include <cstdlib>
#include <string>
#include "Game.h"

int main(int argc, char** argv) {

	

	Game* game = new Game(800, 800, "Potts", 60);
	game->start();

	delete game;

	return 0;
}

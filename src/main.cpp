#include <cstdlib>
#include <string>
#include "Game.h"

int main(int argc, char** argv) {
	int fps = 60;
	if(argc >=2)
		fps = std::atoi(argv[1]);
	std::cout<<"Please give the name of the room:"<<std::endl;
	std::string room;
	std::getline(std::cin,room);
	Game* game = new Game(800, 800, "Potts", fps, room);
	game->start();

	delete game;

	return 0;
}

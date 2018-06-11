#include <cstdlib>
#include <string>
#include "Game.h"

int main(int argc, char** argv) {
	int fps = 60;
	std::string ip;
	int port;
	if(argc <3){
		std::cout<<"Please provide IP:"<<std::endl;
		std::getline(std::cin, ip);
		std::cout<<"Please provide port:"<<std::endl;
	
		std::cin>>port;
	}else{
		ip = std::string(argv[1]);
		port = std::atoi(argv[2]);	
	}
	
	std::cout<<"Please give the name of the room:"<<std::endl;
	std::cin.ignore();
	std::string room;
	std::getline(std::cin,room);
	Game* game = new Game(800, 800, "Potts", ip, port, room);
	game->start();

	delete game;

	return 0;
}

#include <cstdlib>
#include <string>
#include "Game.h"

int main(int argc, char** argv) {

	if(argc == 2 && !strcmp("help", argv[1])){
		std::cout<< "Usage: ./binary <T> <cFactor> <n_spins> <dim_size> <screen_size_xy> {optional<fps>}" <<std::endl;
		std::cout<< "For best visual results <dim_size> | <screen_size_xy>." <<std::endl;
		return 0;
	}
	else if(argc < 6)
		return 1;
	// Assuming user is reasonable.
	float t     = atof(argv [1]);
    float cf    = atof(argv [2]);
    int n       = atoi(argv [3]);
    int size    = atoi(argv[4]);
    int dim    = atoi(argv[5]);
    int fps = 60;
    if(argc == 7){
    	fps = atoi(argv[6]) == 0 ? 60 : atoi(argv[6]);
    }

	Game* game = new Game(dim, dim, "Potts", t, cf, n, size, fps);
	game->start();

	delete game;

	return 0;
}

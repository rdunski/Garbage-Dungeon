#include "Game.h"
using namespace std;

int main(int argc, char* argv[])
{
	Game GD; //Game class
	GD.init(); //initialize the important things
	GD.run(); //run the game until we have quitters
	return 0; //safety measure
}

// THINGS TO DO:
//
//  escape/pause menu for various operations
//	add physics to jumping
//  add physics to movement
//	create animation for jumping <-- DONE
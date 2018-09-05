/*	
	
	USING THE SDL RESOURCE LIBRARY

	Original project by: Robert J Dunski and David Seagraves
	Continued and maintained by: Robert J Dunski and Sean Lowe
	
	Garbage Dungeon - Nonsense Adventuring!
	
*/

#include "Game.h"
using namespace std;

int main(int argc, char* argv[])
{
	Game GD;		// Game class
	GD.init();		// initialize the important things
	GD.run();		// run the game until we have quitters
	return 0;		// safety measure
}

// THINGS TO DO:
//
//  escape/pause menu for various operations - IN PROGRESS
//	add physics to jumping
//  add physics to non-player movement and projectiles
//	add AI to evilCarl
//	finalize sound for footsteps and jump land - DONE
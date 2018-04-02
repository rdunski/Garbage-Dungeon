#include "Game.h"
using namespace std;

int main(int argc, char* argv[])
{
	Game GD;
	GD.init();
	GD.run();
	return 0;
}



// THINGS TO DO:
//
//	add physics to jumping
//			create animation for jumping?
//	move physics to its own class?
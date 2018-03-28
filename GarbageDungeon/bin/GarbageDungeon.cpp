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
//	edit "dudeleft.bmp" to present a smooth transition of frames. Right now it's reading them backwards.
//	add physics to jumping
//			create animation for jumping?
//	move physics to its own class?
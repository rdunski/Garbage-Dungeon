#include <stdio.h>
#include <iostream>
#include<string>
#include<sstream>
#include<vector>
#include "SDL.h"
#include"Game.h"
#include"Sprite.h"
using namespace std;

Game GD;

int main(int argc, char* argv[])
{
	GD.init();
	GD.run("I LIIIIIIVE!", "Forest0.bmp");
	return 0;
}
#include "Atari_Breakout_CloneApp.h"

int main()
{
	// Allocation
	auto app = new Atari_Breakout_CloneApp();

	// Initialise and loop
	app->run("AIE", 1280, 720, false);

	// Deallocation
	delete app;

	return 0;
}
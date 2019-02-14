#include "PhysicsProjectApp.h"

int main()
{	
	// Allocation
	auto app = new PhysicsProjectApp();

	// Initialise and loop
	app->run("AIE", 1280, 720, false);

	// Deallocation
	delete app;

	return 0;
}
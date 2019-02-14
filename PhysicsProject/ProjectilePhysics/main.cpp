#include "ProjectilePhysicsApp.h"

int main()
{
	// Allocation
	auto app = new ProjectilePhysicsApp();

	// Initialise and loop
	app->run("AIE", 1280, 720, false);

	// Deallocation
	delete app;

	return 0;
}
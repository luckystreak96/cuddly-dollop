#define CATCH_CONFIG_RUNNER
#include "../tests/catch.hpp"


#include <iostream>
#include "glfwBackend.h"
#include "game.h"
#include "gameData.h"

//#define CATCH_CONFIG_MAIN

/*
TEST_CASE("1: All test cases reside in other .cpp files (empty)", "[multi-file:1]") {
	REQUIRE(1 == 0);
}
 */

int run_tests(int argc, char** argv)
{
	Catch::Session session; // There must be exactly one instance

  // writing to session.configData() here sets defaults
  // this is the preferred way to set them

	int returnCode = session.applyCommandLine(argc, argv);
	if (returnCode != 0) // Indicates a command line error
		return returnCode;

	// writing to session.configData() or session.Config() here 
	// overrides command line args
	// only do this if you know you need to

	int numFailed = session.run();

	// numFailed is clamped to 255 as some unices only use the lower 8 bits.
	// This clamping has already been applied, so just return it here
	// You can also do any post run clean-up here
	return numFailed;
}

int main(int argc, char* argv[])
{
	bool debug = false;
	for(int i = 0; i < argc; ++i)
    {
		std::string arg = std::string(argv[i]);
		if(arg == "-debug")
		{
			debug = true;
			std::cout << "Starting in debug mode" << std::endl;
		}
    }

	Game* game = new Game();
	GLFWManager manager = GLFWManager();

//#ifdef NDEBUG
	if(debug) {
		argc = 1;
		std::cout << run_tests(argc, argv) << " tests have failed." << std::endl;
	}
//#endif
	// Windows only
	//else
		//FreeConsole();


	manager.GLFWMainLoop(game);

	delete game;
	return 0;
}

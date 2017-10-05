#include <iostream>
#include "glutBackend.h"
#include "game.h"
#include "define_gl.h"
#include <rapidjson\document.h>

int main(int argc, char** argv)
{
	const char* json = "{\"hello\": \"world\","
		"\"t\" : true,"
		"\"f\" : false,"
		"\"n\" : null,"
		"\"i\" : 123,"
		"\"pi\" : 3.1416,"
		"\"a\" : [1, 2, 3, 4]"
		"}";
	rapidjson::Document doc;
	doc.Parse(json);
	if (doc.IsObject())
		if (doc.HasMember("hello"))
			std::cout << doc["hello"].GetString() << std::endl;
	std::cin.ignore();
    //GLUTBackendInit(argc, argv, false, false);

    //if(!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, IS_FULLSCREEN, "Game Title Here"))
    //{
    //    std::cout << "Failed to create window" << std::endl;
    //    return 1;
    //}

    //Game* game = new Game();
    //if(!game->init(WINDOW_WIDTH, WINDOW_HEIGHT))
    //{
    //    std::cout << "Game class init failed" << std::endl;
    //    return 1;
    //}

    //game->run();
    return 0;
}

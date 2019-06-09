//
// Created by yanik on 16/03/19.
//

#include "catch.hpp"

#include "../src/command_line.h"

TEST_CASE("CommandLine::create_command()", "[CommandLine]")
{
    // Init
    CommandLine cl;
    std::string input = "/dialogue \"Hi there\" punk 22";
    for(auto x : input)
        cl.handle_input(x);

    SECTION("no command exists until \\n")
    {
        REQUIRE(cl.get_command()._command == "");
    }

    cl.handle_input('\n');

    SECTION("command exists after \\n and is the correct string")
    {
        REQUIRE(cl.get_command()._command == "dialogue");
    }

    SECTION("args are proper")
    {
        CommandParams cp = cl.get_command();
        REQUIRE(cp._args[0] == "Hi there");
        REQUIRE(cp._args[1] == "punk");
        REQUIRE(cp._args[2] == "22");
    }
}

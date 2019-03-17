//
// Created by yanik on 16/03/19.
//
#include "catch.hpp"

#include "../src/utils.h"

/*
TEST_CASE("c++ random tests", "[main]")
{
	int h = 0;
	int v = 5;
	REQUIRE(h == v);
}
*/

TEST_CASE("Utils::interpret_type() tests", "[Utils]")
{
	// Init
	std::vector<std::string> integers;
	integers.push_back("245");
	integers.push_back("0");

	std::vector<std::string> floats;
	floats.push_back("245.0");
	floats.push_back("0.0");

	std::vector<std::string> strings;
	strings.push_back("245f");
	strings.push_back("25mph");
	strings.push_back("0,0");

	SECTION("integer values are considered ints")
	{
	    for(auto x : integers)
			REQUIRE(Utils::interpret_type(x) == BT_int);
	}

	SECTION("floating point values are considered floats")
	{
		for(auto x : floats)
			REQUIRE(Utils::interpret_type(x) == BT_float);
	}

	SECTION("string values that are not int of float are considered strings")
	{
		for(auto x : strings)
			REQUIRE(Utils::interpret_type(x) == BT_string);
	}
}

//
// Created by yanik on 16/03/19.
//
#include "catch.hpp"

#include "../src/utils.h"
#include "../src/color_generator.h"

TEST_CASE("ColorGenerator::next() test", "[ColorGenerator]")
{
	// Init
	Vector3f min = Vector3f(0.1f, 0.4f, 0.8f);
	Vector3f max = Vector3f(0.2f, 0.6f, 1.0f);
	UniformColorDistribution gen = UniformColorDistribution(min, max);

	bool result = true;
	SECTION("Values are within threshold")
	{
		Vector3f res;
		for(int i = 0; i < 100; i++)
		{
			res = gen.next();
			for(int j = 0; j < 3; j++)
				if (!(res[j] <= max[j] && res[j] >= min[j]))
				{
					result = false;
					break;
				}
		}
		REQUIRE(result);
	}

	result = true;
	SECTION("Opposite range is flipped and still works")
	{
		gen = UniformColorDistribution(max, min);
		Vector3f res;
		for(int i = 0; i < 100; i++)
		{
			res = gen.next();
			for(int j = 0; j < 3; j++)
				if (!(res[j] <= max[j] && res[j] >= min[j]))
				{
					result = false;
					break;
				}
		}
		REQUIRE(result);
	}
}

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

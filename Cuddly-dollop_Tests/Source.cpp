#include "stdafx.h"
#include <CppUnitTest.h>

#include "../Test/src/physics_rpg.h"
#include "../Test/src/physics_manager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace physics_2d
{

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestClassInit)
		{
			physics_rpg* rpg = new physics_rpg();
			//physics_manager manager(rpg);
			Assert::AreEqual(1, 1);
		}
	};
}

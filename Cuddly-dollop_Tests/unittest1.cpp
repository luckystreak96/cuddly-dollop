#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Test/src/vertex.h"
#include "../Test/src/vector3f.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Cuddlydollop_Tests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// setup
			std::vector<Vertex> verts;
			for (int i = 0; i < 4; i++)
			{
				verts.push_back(Vertex(Vector3f(1, 1, 1), Vector2f(1, 1)));
				verts[i].alpha = 100;
			}

			// execute
			Vertex::SetColorAll(verts.begin(), verts.end(), Vector3f(), 1);

			// test
			for (int i = 0; i < 4; i++)
				Assert::AreEqual(verts[i].alpha, 1.0f);
		}

	};
}
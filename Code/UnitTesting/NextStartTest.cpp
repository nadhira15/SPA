#include "stdafx.h"
#include "CppUnitTest.h"
#include "RunTimeDesignExtractor.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(NextStarTest)
	{
	public:

		TEST_METHOD(testNextStar)
		{
			PKB storage = PKB();
			storage.addNext(1, 2);
			storage.addNext(2, 3);
			storage.addNext(3, 4);
			storage.addNext(4, 5);
			storage.addNext(2, 5);
			bool result = storage.hasNextStarPair(1, 4);
			Assert::AreEqual(true, result);
		}
	};
}
#include "stdafx.h"
#include "CppUnitTest.h"
#include "AssignParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(AssignParserTest)
	{
	public:

		TEST_METHOD(testSimpleAssign)
		{
			std::string testStatement = "y=x";
			std::string leftExpected = "y";
			std::string rightExpected = "x";
			AssignParser ap;

			std::string output = ap.getLeft(testStatement);
			Assert::AreEqual(leftExpected, output);

			std::string output2 = ap.getRight(testStatement);
			Assert::AreEqual(rightExpected, output2);
		}

		TEST_METHOD(testExpresionAndFactorAssign)
		{
			std::string testStatement = "p=x+6/2+200";
			std::string leftExpected = "p";
			std::string rightExpected = "x+6/2+200";
			AssignParser ap;

			std::string output = ap.getLeft(testStatement);
			Assert::AreEqual(leftExpected, output);

			std::string output2 = ap.getRight(testStatement);
			Assert::AreEqual(rightExpected, output2);
		}

	};
}
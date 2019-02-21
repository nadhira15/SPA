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
			std::string parseExpected = "y";
			AssignParser ap;

			std::string output = ap.parseLeft(5, testStatement);

			
			Assert::AreEqual(parseExpected, output);
		}

		TEST_METHOD(testExpresionAndFactorAssign)
		{
			std::string testStatement = "p=x+6/2+200";
			std::string parseExpected = "x+6/2+200";
			AssignParser ap;

			std::string output = ap.parseRight(5, testStatement);


			Assert::AreEqual(parseExpected, output);
		}

	};
}
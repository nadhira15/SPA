#include "stdafx.h"
#include "CppUnitTest.h"
#include "Preprocessor.h"
#include "PKB.h"
#include "Parser.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(FrontEndPKBTest)
	{
	public:

		TEST_METHOD(parserPopulatePKBParentRelationTest)
		{
			PKB pkb = PKB();
			Parser parser = Parser();

			//Line 2 read cat;
			Statement readLine1 = Statement("read cat", 3, 2);
			//Line 3 print dog;
			Statement printLine1 = Statement("print dog", 4, 3);
			//Line 4 x = x + 8;
			Statement assignLine1 = Statement("x = x + 8", 1, 4);

			std::vector<Statement> simple;

			simple.push_back(readLine1);
			simple.push_back(printLine1);
			simple.push_back(assignLine1);

			parser.parse(simple, 1, "test");

			int parentTwo = pkb.getParent(2);
			int parentThree = pkb.getParent(3);
			int parentFour = pkb.getParent(4);

			Assert::AreEqual(1, parentTwo);
			Assert::AreEqual(1, parentThree);
			Assert::AreEqual(1, parentFour);
		}

	};
}
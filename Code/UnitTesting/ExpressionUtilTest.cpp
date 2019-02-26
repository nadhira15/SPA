#include "stdafx.h"
#include "CppUnitTest.h"
#include "ExpressionUtil.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ExpressionUtil;

namespace UnitTesting
{
	TEST_CLASS(ExpressionUtilTest)
	{
	public:

		TEST_METHOD(test2OperandConversion)
		{
			std::string testStatement = "x+6";
			std::string expectedConvert = "+ x 6";

			std::string output = ExpressionUtil::convertInfixToPrefix(testStatement);

			Assert::AreEqual(expectedConvert, output);
		}

		TEST_METHOD(test3OperandConversion)
		{
			std::string testStatement = "xy+995+lcz";
			std::string expectedConvert = "+ + xy 995 lcz";

			std::string output = ExpressionUtil::convertInfixToPrefix(testStatement);

			Assert::AreEqual(expectedConvert, output);
		}

		TEST_METHOD(test4OperandConversion)
		{
			std::string testStatement = "5*95+                vvv/4";
			std::string expectedConvert = "+ * 5 95 / vvv 4";

			std::string output = ExpressionUtil::convertInfixToPrefix(testStatement);

			Assert::AreEqual(expectedConvert, output);
		}

		TEST_METHOD(test4OperandWithBracketPreProcess)
		{
			std::string testStatement = "5*(a+b) / 5";

			

			std::string expectedConvert = "5 * ( a + b ) / 5";

			ExpressionUtil::expressionPreprocess(testStatement);

			Assert::AreEqual(expectedConvert, testStatement);
		}

		TEST_METHOD(test4OperandWithBracketConversion)
		{
			std::string testStatement = "5*(a+b) / 5";



			std::string expectedConvert = "/ * 5 + a b 5";

			std::string output = ExpressionUtil::convertInfixToPrefix(testStatement);

			Assert::AreEqual(expectedConvert, output);
		}
	};
}
#include "stdafx.h"
#include "CppUnitTest.h"
#include "ConditionalExp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ConditionalExpTest)
	{

		TEST_METHOD(testValidInput)
		{
			bool result = ConditionalExp::verifyConditionalExp("x==y+1");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(testInvalidInput)
		{
			bool result = ConditionalExp::verifyConditionalExp("x)=y+1");
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(testAND)
		{
			bool result = ConditionalExp::verifyConditionalExp("(x==(y+1))&&(z<=3)");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(testNOT)
		{
			bool result = ConditionalExp::verifyConditionalExp("!(x<(5+3))");
			Assert::AreEqual(true, result);
		}
	};
}
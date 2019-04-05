#include "stdafx.h"
#include "CppUnitTest.h"
#include "Optimizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(OptimizerTest)
	{
	public:
		TEST_METHOD(testExtractSuchThat1)
		{
			std::string str = "a";
			int actual = OptimizerUtility::getSuchThatEntityType(str);
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testExtractSuchThat2)
		{
			int actual = OptimizerUtility::getSuchThatEntityType("_");
			Assert::AreEqual(actual, 2);
		}
		TEST_METHOD(testExtractSuchThatWildCard)
		{
			std::string s = { '"', 'v', 'a', 'r', '"' };
			int actual = OptimizerUtility::getSuchThatEntityType(s);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testExtractWithSynonym1)
		{
			bool actual = OptimizerUtility::isWithSynonym("a.varName");
			Assert::IsTrue(actual);
		}
		TEST_METHOD(testExtractWithSynonym2)
		{
			bool actual = OptimizerUtility::isWithSynonym("14");
			Assert::IsTrue(!actual);
		}
		TEST_METHOD(testExtractPatternSynonymAssign)
		{
			std::string str = { '"', 's', '"' };
			int actual = OptimizerUtility::getPatternEntityType1(str);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testExtractPatternSynonymIf)
		{
			int actual = OptimizerUtility::getPatternEntityType1("v");
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testExtractPatternSynonymWhile)
		{
			int actual = OptimizerUtility::getPatternEntityType1("_");
			Assert::AreEqual(actual, 2);
		}
		TEST_METHOD(testCreateMaps)
		{
			std::string str = { '"',  's', '"' };
			int actual = OptimizerUtility::getPatternEntityType2(str);
			Assert::AreEqual(actual, 0);
		}
	};
}
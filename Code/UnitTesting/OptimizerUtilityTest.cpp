#include "stdafx.h"
#include "CppUnitTest.h"
#include "OptimizerUtility.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(OptimizerUtilityTest)
	{
	public:
		TEST_METHOD(testSuchThatEntityType1)
		{
			int actual = getSuchThatEntityType("a");
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testSuchThatEntityType2)
		{
			int actual = getSuchThatEntityType("_");
			Assert::AreEqual(actual, 2);
		}
		TEST_METHOD(testSuchThatEntityType0String)
		{
			std::string s = '"' + "var" + '"';
			int actual = getSuchThatEntityType(s);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testSuchThatEntityType0Num)
		{
			int actual = getSuchThatEntityType("14");
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testWithSynonymTrue)
		{
			bool actual = isWithSynonym("a.varName");
			Assert::IsTrue(actual);
		}
		TEST_METHOD(testWithSynonymFalseNum)
		{
			bool actual = isWithSynonym("14");
			Assert::IsTrue(!actual);
		}
		TEST_METHOD(testWithSynonymFalseName)
		{
			bool actual = isWithSynonym("procedure");
			Assert::IsTrue(!actual);
		}
		TEST_METHOD(testPatternEntityTypeFirst0)
		{
			std::string str = '"' + "s" + '"';
			int actual = getPatternEntityType1(str);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testPatternEntityTypeFirst1)
		{
			int actual = getPatternEntityType1("v");
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testPatternEntityTypeFirst2)
		{
			int actual = getPatternEntityType1("_");
			Assert::AreEqual(actual, 2);
		}
		TEST_METHOD(testPatternEntityTypeSecond0)
		{
			std::string str = { '"',  's', '"' };
			int actual = getPatternEntityType2(str);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testPatternEntityTypeSecond1)
		{
			std::string str = { '_', '"',  's', '"', '_' };
			int actual = getPatternEntityType2("v");
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testPatternEntityTypeSecond2)
		{
			int actual = getPatternEntityType2("_");
			Assert::AreEqual(actual, 2);
		}
		TEST_METHOD(testWithPriority1)
		{
			int actual = getWithPriority(1);
			Assert::AreEqual(actual, 4);
		}
		TEST_METHOD(testWithPriority2)
		{
			int actual = getWithPriority(2);
			Assert::AreEqual(actual, 14);
		}
		TEST_METHOD(testPatternPriorityA11)
		{
			int actual = getPatternPriority("assign", 1, 1);
			Assert::AreEqual(actual, 20);
		}
		TEST_METHOD(testPatternPriorityA02)
		{
			int actual = getPatternPriority("assign", 0, 2);
			Assert::AreEqual(actual, 12);
		}
		TEST_METHOD(testPatternPriorityIfs20)
		{
			int actual = getPatternPriority("if", 2, 0);
			Assert::AreEqual(actual, 10);
		}
		TEST_METHOD(testSuchThatPriorityFollowStar1)
		{
			int actual = getSuchThatPriority("Follow*", 1);
			Assert::AreEqual(actual, 17);
		}
	};
}
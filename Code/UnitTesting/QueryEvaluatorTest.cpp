#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryEvaluator.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(QueryEvaluatorTest)
	{
	public:

		TEST_METHOD(setToString_success) 
		{
			unordered_set<string> nonEmptyString ({"1", "2", "3"});
			string result = QueryEvaluator::setToString(nonEmptyString);
			string toCompare = "1, 2, 3, ";
			Assert::AreEqual(result.compare(toCompare), 0);
		}

		TEST_METHOD(intSetToStrSet_empty_success)
		{
			unordered_set<int> emptyIntSet;
			unordered_set<string> result = QueryEvaluator::intSetToStrSet(emptyIntSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(inSetToStrSet_nonEmpty_success)
		{
			unordered_set<int> nonEmptyIntSet ({ 1, 2, 3 });
			unordered_set<string> result = QueryEvaluator::intSetToStrSet(nonEmptyIntSet);
			unordered_set<string> toCompare({ "1", "2", "3" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(truthValue_true_success)
		{
			string result = QueryEvaluator::truthValue(true);
			Assert::AreEqual(result.compare("true"), 0);
		}

		TEST_METHOD(truthValue_false_success)
		{
			string result = QueryEvaluator::truthValue(false);
			Assert::AreEqual(result.compare("false"), 0);
		}

		TEST_METHOD(intPairSetToStrSet_empty_success)
		{
			unordered_set<pair<int, int>, intPairhash> emptyIntPairSet;
			unordered_set<string> result = QueryEvaluator::intPairSetToStrSet(emptyIntPairSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intPairSetToStrSet_nonEmpty_success)
		{
			unordered_set<pair<int, int>, intPairhash> nonEmptyIntPairSet;
			pair<int, int> firstPair(1, 2);
			pair<int, int> secondPair(3, 4);
			nonEmptyIntPairSet.insert(firstPair);
			nonEmptyIntPairSet.insert(secondPair);
			unordered_set<string> result = QueryEvaluator::intPairSetToStrSet(nonEmptyIntPairSet);
			unordered_set<string> toCompare;
			string firstString = "1 2";
			string secondString = "3 4";
			toCompare.insert(firstString);
			toCompare.insert(secondString);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intStrSetToStrSet_empty_success)
		{
			unordered_set<pair<int, string>, intStringhash> emptyIntStrSet;
			unordered_set<string> result = QueryEvaluator::intStrSetToStrSet(emptyIntStrSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intStrSetToStrSet_nonEmpty_success)
		{
			unordered_set<pair<int, string>, intStringhash> nonEmptyIntStrSet;
			pair<int, string> firstPair(1, "baby");
			pair<int, string> secondPair(2, "shark");
			nonEmptyIntStrSet.insert(firstPair);
			nonEmptyIntStrSet.insert(secondPair);
			unordered_set<string> result = QueryEvaluator::intStrSetToStrSet(nonEmptyIntStrSet);
			unordered_set<string> toCompare;
			string firstString = "1 baby";
			string secondString = "2 shark";
			toCompare.insert(firstString);
			toCompare.insert(secondString);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(strPairSetToStrSet_empty_success)
		{
			unordered_set<pair<string, string>, strPairhash> emptyStrPairSet;
			unordered_set<string> result = QueryEvaluator::strPairSetToStrSet(emptyStrPairSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(strPairSetToStrSet_nonEmpty_success)
		{
			unordered_set<pair<string, string>, strPairhash> nonEmptyStrPairSet;
			pair<string, string> firstPair("baby", "shark");
			pair<string, string> secondPair("telling", "lies");
			nonEmptyStrPairSet.insert(firstPair);
			nonEmptyStrPairSet.insert(secondPair);
			unordered_set<string> result = QueryEvaluator::strPairSetToStrSet(nonEmptyStrPairSet);
			unordered_set<string> toCompare;
			string firstString = "baby shark";
			string secondString = "telling lies";
			toCompare.insert(firstString);
			toCompare.insert(secondString);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getFirstParam_empty_success)
		{
			unordered_set<string> emptyStringPair;
			unordered_set<string> result = QueryEvaluator::getFirstParam(emptyStringPair);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getFirstParam_nonEmpty_success)
		{
			unordered_set<string> nonEmptyStringPair;
			string firstString = "1 3";
			string secondString = "2 4";
			nonEmptyStringPair.insert(firstString);
			nonEmptyStringPair.insert(secondString);
			unordered_set<string> result = QueryEvaluator::getFirstParam(nonEmptyStringPair);
			unordered_set<string> toCompare({ "1", "2" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getSecondParam_empty_success)
		{
			unordered_set<string> emptyStringPair;
			unordered_set<string> result = QueryEvaluator::getSecondParam(emptyStringPair);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getSecondParam_nonEmpty_success)
		{
			unordered_set<string> nonEmptyStringPair;
			string firstString = "1 3";
			string secondString = "2 4";
			nonEmptyStringPair.insert(firstString);
			nonEmptyStringPair.insert(secondString);
			unordered_set<string> result = QueryEvaluator::getSecondParam(nonEmptyStringPair);
			unordered_set<string> toCompare({ "3", "4" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intersection_bothEmpty_success) {
			unordered_set<string> firstSet;
			unordered_set<string> secondSet;
			unordered_set<string> result = QueryEvaluator::intersection(firstSet, secondSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intersection_oneEmpty_success) {
			unordered_set<string> firstSet;
			unordered_set<string> secondSet ({"1"});
			unordered_set<string> result = QueryEvaluator::intersection(firstSet, secondSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intersection_noEmpty_success) {
			unordered_set<string> firstSet ({ "1", "2", "3" });
			unordered_set<string> secondSet ({ "2", "3", "4" });
			unordered_set<string> result = QueryEvaluator::intersection(firstSet, secondSet);
			unordered_set<string> toCompare({ "2", "3" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getOtherPair_first_success) {
			unordered_set<string> stringPair ({ "1 2", "3 4", "5 6", "7 8" });
			unordered_set<string> toContain ({ "1", "2", "3", "4", "5", "6" });
			int position = 1;
			unordered_set<string> result = QueryEvaluator::getOtherPair(position, stringPair, toContain);
			unordered_set<string> toCompare({ "2", "4", "6" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getOtherPair_second_success) {
			unordered_set<string> stringPair({ "1 2", "3 4", "5 6", "7 8" });
			unordered_set<string> toContain({ "1", "2", "3", "4", "5", "6" });
			int position = 2;
			unordered_set<string> result = QueryEvaluator::getOtherPair(position, stringPair, toContain);
			unordered_set<string> toCompare({ "1", "3", "5" });
			Assert::AreEqual(result == toCompare, true);
		}
	};
}
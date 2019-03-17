#include "stdafx.h"
#include "CppUnitTest.h"
#include "ContainerUtil.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ContainerUtilTest)
	{
	public:

		TEST_METHOD(intVecToStrSet_empty_success)
		{
			vector<int> emptyIntVec;
			unordered_set<string> result = ContainerUtil::to_strset(emptyIntVec);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}
		TEST_METHOD(intVecToStrSet_nonEmpty_success)
		{
			vector<int> nonEmptyIntVec{ 1, 2, 3 };
			unordered_set<string> result = ContainerUtil::to_strset(nonEmptyIntVec);
			unordered_set<string> toCompare({ "1", "2", "3" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intStrVecToStrSet_empty_success)
		{
			vector<pair<int, string>> emptyIntStrVec;
			unordered_set<string> result = ContainerUtil::to_strset(emptyIntStrVec);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intStrVecToStrSet_nonEmpty_success)
		{
			vector<pair<int, string>> nonEmptyIntStrVec;
			pair<int, string> firstPair(1, "one");
			pair<int, string> secondPair(2, "two");
			nonEmptyIntStrVec.push_back(firstPair);
			nonEmptyIntStrVec.push_back(secondPair);
			unordered_set<string> result = ContainerUtil::to_strset(nonEmptyIntStrVec);
			unordered_set<string> toCompare;
			string firstString = "1 one";
			string secondString = "2 two";
			toCompare.insert(firstString);
			toCompare.insert(secondString);
			Assert::AreEqual(result == toCompare, true);
		}
		TEST_METHOD(intSetToStrSet_empty_success)
		{
			unordered_set<int> emptyIntSet;
			unordered_set<string> result = ContainerUtil::to_strset(emptyIntSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(inSetToStrSet_nonEmpty_success)
		{
			unordered_set<int> nonEmptyIntSet ({ 1, 2, 3 });
			unordered_set<string> result = ContainerUtil::to_strset(nonEmptyIntSet);
			unordered_set<string> toCompare({ "1", "2", "3" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intPairSetToStrSet_empty_success)
		{
			unordered_set<pair<int, int>, intPairhash> emptyIntPairSet;
			unordered_set<string> result = ContainerUtil::to_strset(emptyIntPairSet);
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
			unordered_set<string> result = ContainerUtil::to_strset(nonEmptyIntPairSet);
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
			unordered_set<string> result = ContainerUtil::to_strset(emptyIntStrSet);
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
			unordered_set<string> result = ContainerUtil::to_strset(nonEmptyIntStrSet);
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
			unordered_set<string> result = ContainerUtil::to_strset(emptyStrPairSet);
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
			unordered_set<string> result = ContainerUtil::to_strset(nonEmptyStrPairSet);
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
			unordered_set<string> result = ContainerUtil::getFirstParam(emptyStringPair);
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
			unordered_set<string> result = ContainerUtil::getFirstParam(nonEmptyStringPair);
			unordered_set<string> toCompare({ "1", "2" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getSecondParam_empty_success)
		{
			unordered_set<string> emptyStringPair;
			unordered_set<string> result = ContainerUtil::getSecondParam(emptyStringPair);
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
			unordered_set<string> result = ContainerUtil::getSecondParam(nonEmptyStringPair);
			unordered_set<string> toCompare({ "3", "4" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intersection_bothEmpty_success) {
			unordered_set<string> firstSet;
			unordered_set<string> secondSet;
			unordered_set<string> result = ContainerUtil::intersection(firstSet, secondSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intersection_oneEmpty_success) {
			unordered_set<string> firstSet;
			unordered_set<string> secondSet ({"1"});
			unordered_set<string> result = ContainerUtil::intersection(firstSet, secondSet);
			unordered_set<string> toCompare;
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intersection_noEmpty_success) {
			unordered_set<string> firstSet ({ "1", "2", "3" });
			unordered_set<string> secondSet ({ "2", "3", "4" });
			unordered_set<string> result = ContainerUtil::intersection(firstSet, secondSet);
			unordered_set<string> toCompare({ "2", "3" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getOtherPair_first_success) {
			unordered_set<string> stringPair ({ "1 2", "3 4", "5 6", "7 8" });
			unordered_set<string> toContain ({ "1", "2", "3", "4", "5", "6" });
			int position = 1;
			unordered_set<string> result = ContainerUtil::getOtherPair(position, stringPair, toContain);
			unordered_set<string> toCompare({ "2", "4", "6" });
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(getOtherPair_second_success) {
			unordered_set<string> stringPair({ "1 2", "3 4", "5 6", "7 8" });
			unordered_set<string> toContain({ "1", "2", "3", "4", "5", "6" });
			int position = 2;
			unordered_set<string> result = ContainerUtil::getOtherPair(position, stringPair, toContain);
			unordered_set<string> toCompare({ "1", "3", "5" });
			Assert::AreEqual(result == toCompare, true);
		}
	};
}
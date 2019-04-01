#include "stdafx.h"
#include "CppUnitTest.h"
#include "Preprocessor.h"
#include "PKB.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(DesignExtractorPKBTest)
	{
	public:

		TEST_METHOD(designExtractorCallStarTest) {
			PKB pkb = PKB();
			DesignExtractor de = DesignExtractor();

			pkb.addProc("One");
			pkb.addProc("Two");
			pkb.addProc("Three");
			pkb.addProc("Four");

			pkb.addCall("One", "Two");
			pkb.addCall("Two", "Three");
			pkb.addCall("Three", "Four");

			de.extractDesigns();

			Assert::IsTrue(pkb.hasCallStarPair("One", "Two"));

			Assert::IsTrue(pkb.hasCallStarPair("One", "Three"));

			Assert::IsTrue(pkb.hasCallStarPair("One", "Four"));

			Assert::IsTrue(pkb.hasCallStarPair("Two", "Three"));

			Assert::IsTrue(pkb.hasCallStarPair("Two", "Four"));

			Assert::IsTrue(pkb.hasCallStarPair("Three", "Four"));

		}

		TEST_METHOD(designExtractorCyclicTest) {
			PKB *pkb = new PKB();
			DesignExtractor de = DesignExtractor();


			pkb.addProc("One");
			pkb.addProc("Two");
			pkb.addProc("Three");

			pkb.addCall("One", "Two");
			pkb.addCall("Two", "Three");
			pkb.addCall("Three", "One");

			Assert::ExpectException<std::string>(DesignExtractor::extractDesigns);
		}

		TEST_METHOD(designExtractorSingleCyclicTest) {
			PKB pkb = PKB();
			DesignExtractor de = DesignExtractor();


			pkb.addProc("One");

			pkb.addCall("One", "One");

			Assert::ExpectException<std::string>(DesignExtractor::extractDesigns);
		}

		TEST_METHOD(designExtractorNonExistantProc) {
			PKB pkb = PKB();
			DesignExtractor de = DesignExtractor();

			pkb.addProc("One");
			pkb.addProc("Two");
			pkb.addProc("Three");

			pkb.addCall("One", "Two");
			pkb.addCall("Two", "Threev");
			pkb.addCall("Three", "Onez");

			Assert::ExpectException<std::string>(DesignExtractor::extractDesigns);
		}

		TEST_METHOD(designExtractorFollowStarTest) {
			PKB pkb = PKB();
			DesignExtractor de = DesignExtractor();

			pkb.addStatement(1, stmType::assign, "test");
			pkb.addStatement(2, stmType::assign, "test");

			pkb.addStatement(3, stmType::assign, "test");

			pkb.addStatement(4, stmType::assign, "test");

			pkb.addStatement(5, stmType::assign, "test");

			pkb.addStatement(6, stmType::assign, "test");

			pkb.addStatement(7, stmType::assign, "test");

			pkb.addFollow(1, 2);
			pkb.addFollow(2, 3);
			pkb.addFollow(3, 4);
			pkb.addFollow(5, 6);
			pkb.addFollow(4, 7);


			de.extractDesigns();



			Assert::IsTrue(pkb.hasFollowStarPair(1, 2));
			Assert::IsTrue(pkb.hasFollowStarPair(1, 3));
			Assert::IsTrue(pkb.hasFollowStarPair(1, 4));
			Assert::IsTrue(pkb.hasFollowStarPair(1, 7));
			Assert::IsTrue(pkb.hasFollowStarPair(2, 3));
			Assert::IsTrue(pkb.hasFollowStarPair(2, 4));
			Assert::IsTrue(pkb.hasFollowStarPair(2, 7));
			Assert::IsTrue(pkb.hasFollowStarPair(3, 4));
			Assert::IsTrue(pkb.hasFollowStarPair(3, 7));
			Assert::IsTrue(pkb.hasFollowStarPair(4, 7));
			Assert::IsTrue(pkb.hasFollowStarPair(5, 6));

			std::unordered_set<int> set({ 2, 3, 4, 7 });

			Assert::IsTrue(pkb.getAllFollowing(1) == set);





		}


	};
}
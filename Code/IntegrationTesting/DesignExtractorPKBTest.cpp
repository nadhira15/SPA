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
			PKB pkb = PKB();
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
	};
}
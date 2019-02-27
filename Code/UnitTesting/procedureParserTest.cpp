#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProcedureParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(procedureParserTest)
	{

		TEST_METHOD(testValidInput)
		{
			std::string result = ProcedureParser::parseProcName("procedure Main");
			std::string expected = "Main";
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(testInvalidInput)
		{
			std::string result = ProcedureParser::parseProcName("procedure 2day");
			std::string expected = "failed";
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(testVerifyProcName)
		{
			bool result = ProcedureParser::verifyProcName("procedure ComputeWatever");
			Assert::AreEqual(result, true);
		}

		TEST_METHOD(testVerifyProcInvalidName)
		{
			bool result = ProcedureParser::verifyProcName("procedure Com@puteWatever");
			Assert::AreEqual(result, false);
		}
	};
}
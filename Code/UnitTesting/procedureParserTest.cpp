#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"
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

		TEST_METHOD(testVarifyProcName)
		{
			bool result = ProcedureParser::varifyProcName("procedure ComputeWatever");
			Assert::AreEqual(result, true);
		}

		TEST_METHOD(testVarifyProcInvalidName)
		{
			bool result = ProcedureParser::varifyProcName("procedure Com@puteWatever");
			Assert::AreEqual(result, false);
		}
	};
}
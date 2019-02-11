#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryParser.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(QueryParserTest)
	{
	public:

		TEST_METHOD(initialValidation_success)
		{
			string result = QueryParser::initialValidation("assign a; Select a");
			Assert::AreEqual(result.compare(""), 0);
		}
		
		TEST_METHOD(initialValidation_lengthZero)
		{
			string result = QueryParser::initialValidation("");
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(initialValidation_noSelectStatement)
		{
			string result = QueryParser::initialValidation("assign a;");
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(initialValidation_noVariableDeclaration)
		{
			string result = QueryParser::initialValidation("Select a");
			Assert::AreEqual(result.compare("None"), 0);
		}

		TEST_METHOD(validateClauses_success)
		{
			vector<string> clauses{ "assign a", "Select a" };
			string result = QueryParser::validateClauses(clauses);
			Assert::AreEqual(result.compare(""), 0);
		}

		TEST_METHOD(validateClauses_noSpaceInDeclaration)
		{
			vector<string> clauses{ "assign", "Select a" };
			string result = QueryParser::validateClauses(clauses);
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(validateClauses_noSelectStatement)
		{
			vector<string> clauses{ "assign a", "while w" };
			string result = QueryParser::validateClauses(clauses);
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

	};
}
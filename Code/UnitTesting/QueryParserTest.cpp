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

		TEST_METHOD(splitClauses_success)
		{
			vector<string> actual = QueryParser::splitClauses("assign a; while w;Select a such that Follows(a,w)");
			vector<string> expected{ "assign a", "while w", "Select a such that Follows(a,w)" };
			Assert::AreEqual(actual == expected, true);
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

		TEST_METHOD(splitDeclarations_success)
		{
			vector<string> input = { "assign a, a1, a2", "while w", "Select a such that Follows(a,w)" };
			vector<pair<string, string>> actual = QueryParser::splitDeclarations(input);
			vector<pair<string, string>> expected{ {"assign", "a"}, {"assign", "a1"}, {"assign", "a2"}, {"while", "w"}};
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(removeAllWhitespaces_multipleTab_success)
		{
			string actual = QueryParser::removeAllWhitespaces("\t\ta\ts\td\t");
			string expected = "asd";
			Assert::AreEqual(actual, expected);
		}

		TEST_METHOD(removeAllWhitespaces_newLine_success)
		{
			string actual = QueryParser::removeAllWhitespaces("\n\na\nsd\n");
			string expected = "asd";
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(removeAllWhitespaces_multipleSpaces_success)
		{
			string actual = QueryParser::removeAllWhitespaces("a   s    d");
			string expected = "asd";
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(removeTrailingWhitespaces_multipleTab_success)
		{
			string actual = QueryParser::removeTrailingWhitespaces("\t\ta\ts\td\t");
			string expected = "a\ts\td";
			Assert::AreEqual(actual, expected);
		}

		TEST_METHOD(removeTrailingWhitespaces_newLine_success)
		{
			string actual = QueryParser::removeTrailingWhitespaces("\n\na\nsd\n");
			string expected = "a\nsd";
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(removeTrailingWhitespaces_multipleSpaces_success)
		{
			string actual = QueryParser::removeTrailingWhitespaces("   a   s    d   ");
			string expected = "a   s    d";
			Assert::AreEqual(expected, actual);
		}

	};
}
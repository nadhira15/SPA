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

		TEST_METHOD(validateDeclarations_success)
		{
			vector<pair<string, string>> declarations{ {"assign", "a"}, {"assign", "a1"}, {"assign", "a2"}, {"while", "w"} };
			string actual = QueryParser::validateDeclarations(declarations);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateDeclarations_invalidType)
		{
			vector<pair<string, string>> declarations{ {"assign", "a"}, {"hello", "h"} };
			string actual = QueryParser::validateDeclarations(declarations);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateDeclarations_invalidName)
		{
			vector<pair<string, string>> declarations{ {"assign", "a12_"}, {"hello", "h"} };
			string actual = QueryParser::validateDeclarations(declarations);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_success)
		{
			vector<string> actual = QueryParser::splitSelectParameter("Select a");
			vector<string> expected{ "a" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_success)
		{
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitSuchThatCondition("such that Parent(a, '_')");
			vector<pair<string, pair<string, string>>> expected{ {"Parent", {"a", "'_'"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_multipleWhitespaces_success)
		{
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitSuchThatCondition("such that    Parent    (   a  , '_'  )  ");
			vector<pair<string, pair<string, string>>> expected{ {"Parent", {"a", "'_'"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_success)
		{
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitPatternCondition("pattern a('_', 'x*y+z' )");
			vector<pair<string, pair<string, string>>> expected{ {"a", {"'_'", "'x*y+z'"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_multipleWhitespaces_success)
		{
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitPatternCondition("pattern   a  ( '_' , 'x*y+z' ) ");
			vector<pair<string, pair<string, string>>> expected{ {"a", {"'_'", "'x*y+z'"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_success)
		{
			vector<string> selectedVar = { "a" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_varNotDeclared)
		{
			vector<string> selectedVar = { "b" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "selected variable not found";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_invalidVarName)
		{
			vector<string> selectedVar = { "hello_1" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "invalid variable name";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"Parent", { "ifs", "_" }} };
			unordered_map<string, string> declarationsMap = { {"ifs", "if"}, {"s", "stmt"} };
			string actual = QueryParser::validateSuchThatParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_invalidRelName)
		{
			vector<pair<string, pair<string, string>>> param{ {"IfWhile", { "a", "_" }} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validateSuchThatParam(param, declarationsMap);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_invalidParamType)
		{
			vector<pair<string, pair<string, string>>> param{ {"Parent", { "a", "_" }} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validateSuchThatParam(param, declarationsMap);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"a", {"_", "\"x*y+z\""}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validatePatternParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_invalidStmtName)
		{
			vector<pair<string, pair<string, string>>> param{ {"a_a", {"_", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validatePatternParam(param, declarationsMap);
			string expected = "invalid assign synonym";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_stmtNotDeclared)
		{
			vector<pair<string, pair<string, string>>> param{ {"b", {"'_'", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validatePatternParam(param, declarationsMap);
			string expected = "statement not found";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_stmtTypeNotAssign)
		{
			vector<pair<string, pair<string, string>>> param{ {"s", {"'_'", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryParser::validatePatternParam(param, declarationsMap);
			string expected = "statement type is not assign";
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
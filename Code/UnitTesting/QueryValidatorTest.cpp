#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(QueryValidatorTest)
	{
	public:

		TEST_METHOD(initialValidation_success)
		{
			string result = QueryValidator::initialValidation("assign a; Select a");
			Assert::AreEqual(result.compare(""), 0);
		}

		TEST_METHOD(initialValidation_lengthZero)
		{
			string result = QueryValidator::initialValidation("");
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(initialValidation_noSelectStatement)
		{
			string result = QueryValidator::initialValidation("assign a;");
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(initialValidation_noVariableDeclaration)
		{
			string result = QueryValidator::initialValidation("Select a");
			Assert::AreEqual(result.compare("None"), 0);
		}

		TEST_METHOD(validateClauses_success)
		{
			vector<string> clauses{ "assign a", "Select a" };
			string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare(""), 0);
		}

		TEST_METHOD(validateClauses_noSpaceInDeclaration)
		{
			vector<string> clauses{ "assign", "Select a" };
			string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(validateClauses_noSelectStatement)
		{
			vector<string> clauses{ "assign a", "while w" };
			string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(validateDeclarations_success)
		{
			vector<pair<string, string>> declarations{ {"assign", "a"}, {"assign", "a1"}, {"assign", "a2"}, {"while", "w"} };
			string actual = QueryValidator::validateDeclarations(declarations);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateDeclarations_invalidType)
		{
			vector<pair<string, string>> declarations{ {"assign", "a"}, {"hello", "h"} };
			string actual = QueryValidator::validateDeclarations(declarations);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateDeclarations_invalidName)
		{
			vector<pair<string, string>> declarations{ {"assign", "a12_"}, {"hello", "h"} };
			string actual = QueryValidator::validateDeclarations(declarations);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_success)
		{
			vector<string> selectedVar = { "a" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectBoolean_success)
		{
			vector<string> selectedVar = { "BOOLEAN" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectTuple_success)
		{
			vector<string> selectedVar = { "a", "s" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_varNotDeclared)
		{
			vector<string> selectedVar = { "b" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "selected variable not found";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_invalidVarName)
		{
			vector<string> selectedVar = { "hello_1" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "invalid variable name";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectTupleBoolean)
		{
			vector<string> selectedVar = { "a", "BOOLEAN" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "too many selected variable for boolean";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"Parent", { "ifs", "_" }} };
			unordered_map<string, string> declarationsMap = { {"ifs", "if"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_invalidRelName)
		{
			vector<pair<string, pair<string, string>>> param{ {"IfWhile", { "a", "_" }} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_invalidParamType)
		{
			vector<pair<string, pair<string, string>>> param{ {"Parent", { "a", "_" }} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			string expected = "semantic error";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_multipleClauses_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"Parent", { "a", "_" }}, { "Follows", {"10", "11"} }, { "Modifies", {"3", "\"sum\""} }, { "Uses", {"5", "\"a\""} }, { "Next*", {"1", "10"} }, { "Affects*", {"4", "10"} } };
			unordered_map<string, string> declarationsMap = { {"a", "if"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"a", {"_", "\"x*y+z\""}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_invalidStmtName)
		{
			vector<pair<string, pair<string, string>>> param{ {"a_a", {"_", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "invalid synonym";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_stmtNotDeclared)
		{
			vector<pair<string, pair<string, string>>> param{ {"b", {"_", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "statement not found";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_stmtTypeNotAssign)
		{
			vector<pair<string, pair<string, string>>> param{ {"s", {"_", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "statement type is invalid";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_multipleClauses_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"a", {"_", "\"x\""}}, {"a1", {"_", "\"y\""}}, {"ifs", {"\"x\"", "_,_"}}, {"while1", {"\"y\"", "_"}}, {"ifs", {"_", "_,_"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"a1", "assign"}, {"ifs", "if"}, {"while1", "while"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

	};
}
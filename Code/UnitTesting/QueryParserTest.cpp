#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryParser.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(QueryParserTest)
	{
	public:

		TEST_METHOD(splitClauses_success)
		{
			vector<string> actual = QueryParser::splitClauses("assign a; while w;Select a such that Follows(a,w)");
			vector<string> expected{ "assign a", "while w", "Select a such that Follows(a,w)" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitDeclarations_success)
		{
			vector<string> input = { "assign a, a1, a2", "while w", "Select a such that Follows(a,w)" };
			vector<pair<string, string>> actual = QueryParser::splitDeclarations(input);
			vector<pair<string, string>> expected{ {"assign", "a"}, {"assign", "a1"}, {"assign", "a2"}, {"while", "w"}};
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

	};
}
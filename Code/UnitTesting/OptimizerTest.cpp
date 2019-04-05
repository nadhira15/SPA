#include "stdafx.h"
#include "CppUnitTest.h"
#include "Optimizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(OptimizerTest)
	{
	public:
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> empty;
		std::unordered_set<std::string> s;
		std::unordered_map<std::string, std::string> d;
		TEST_METHOD(testExtractSuchThat1)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause = 
				std::make_pair("Follows", std::make_pair("1", "s"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			st.push_back(clause);
			Optimizer op = Optimizer(st, empty, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractSuchThatSyn(0);
			std::vector<std::string> vect{ "s" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(49, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractSuchThat2)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("Parent*", std::make_pair("a", "s"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			st.push_back(clause);
			Optimizer op = Optimizer(st, empty, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractSuchThatSyn(0);
			std::vector<std::string> vect{ "a", "s" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(19, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractSuchThatWildCard)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("Modifies", std::make_pair("p", "_"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			st.push_back(clause);
			Optimizer op = Optimizer(st, empty, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractSuchThatSyn(0);
			std::vector<std::string> vect{ "p" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(23, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractWithSynonym1)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("with", std::make_pair("a.stmtNum", "14"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w;
			w.push_back(clause);
			Optimizer op = Optimizer(empty, w, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractWithSyn(0);
			std::vector<std::string> vect{ "a" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(46, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractWithSynonym2)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("with", std::make_pair("pr.varName", "re.varName"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w;
			w.push_back(clause);
			Optimizer op = Optimizer(empty, w, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractWithSyn(0);
			std::vector<std::string> vect{ "pr", "re" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(32, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractPatternSynonymAssign)
		{
			std::string str1 = { '"', 'v', 'a', 'r', '"' };
			std::string str2 = { '_', '"', 'a', '+', 'b', '"', '_' };
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("a", std::make_pair(str1, str2));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			d["a"] = "assign";
			p.push_back(clause);
			Optimizer op = Optimizer(empty, empty, p, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractPatternSyn(0);
			std::vector<std::string> vect{ "a" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(44, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractPatternSynonymIf)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("ifs", std::make_pair("v", "_"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			d["ifs"] = "if";
			p.push_back(clause);
			Optimizer op = Optimizer(empty, empty, p, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractPatternSyn(0);
			std::vector<std::string> vect{ "ifs", "v" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(29, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractPatternSynonymWhile)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("w", std::make_pair("_", "_"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			d["w"] = "while";
			p.push_back(clause);
			Optimizer op = Optimizer(empty, empty, p, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractPatternSyn(0);
			std::vector<std::string> vect{ "w" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(16, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testGraphChainingLogic1)
		{

		}
		TEST_METHOD(testGraphChainingLogic2)
		{

		}
		TEST_METHOD(testTriviality)
		{

		}
	};
}
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Preprocesser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PreprocesserTest)
	{
	public:
		Preprocesser p = Preprocesser("");
		// types = { 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE" };
		TEST_METHOD(testValidator)
		{
			int result = p.validateSemicolon("test");
			Assert::AreEqual(0, result);
		}
		TEST_METHOD(testAssignValidator1)
		{
			int result = p.validateSemicolon("a=b+c+d");
			Assert::AreEqual(1, result);
		}
		TEST_METHOD(testAssignValidator2)
		{
			int result = p.validateSemicolon("read =xb");
			Assert::AreEqual(1, result);
		}
		TEST_METHOD(testAssignValidator3)
		{
			int result = p.validateSemicolon("a==c+d");
			Assert::AreEqual(0, result);
		}
		TEST_METHOD(testCallValidator1)
		{
			int result = p.validateSemicolon("call   p");
			Assert::AreEqual(2, result);
		}
		TEST_METHOD(testCallValidator2)
		{
			int result = p.validateSemicolon("call p, p2");
			Assert::AreEqual(0, result);
		}
		TEST_METHOD(testReadValidator1)
		{
			int result = p.validateSemicolon("read   s");
			Assert::AreEqual(3, result);
		}
		TEST_METHOD(testReadValidator2)
		{
			int result = p.validateSemicolon("reads");
			Assert::AreEqual(0, result);
		}
		TEST_METHOD(testPrintValidator1)
		{
			int result = p.validateSemicolon("print   q");
			Assert::AreEqual(4, result);
		}
		TEST_METHOD(testPrintValidator2)
		{
			int result = p.validateSemicolon("print q p");
			Assert::AreEqual(0, result);
		}
		TEST_METHOD(testWhileValidator1)
		{
			int result = p.validateProc("while( a == b )");
			Assert::AreEqual(5, result);
		}
		TEST_METHOD(testWhileValidator2)
		{
			int result = p.validateProc("while (a==b)a");
			Assert::AreEqual(0, result);
		}
		TEST_METHOD(testIfValidator1)
		{
			int result = p.validateProc("if(aaaa)then");
			Assert::AreEqual(6, result);
		}
		TEST_METHOD(testIfValidator2)
		{
			int result = p.validateProc("if (a == b) then");
			Assert::AreEqual(6, result);
		}
		TEST_METHOD(testIfValidator3)
		{
			int result = p.validateProc("if(aaaa)athen");
			Assert::AreEqual(0, result);
		}
		TEST_METHOD(testElseValidator1)
		{
			int result = p.validateProc("else");
			Assert::AreEqual(7, result);
		}
		TEST_METHOD(testElseValidator2)
		{
			int result = p.validateProc("elsa");
			Assert::AreEqual(0, result);
		}
		TEST_METHOD(testProcValidator1)
		{
			int result = p.validateProc("procedure    q");
			Assert::AreEqual(8, result);
		}
		TEST_METHOD(testProcValidator2)
		{
			int result = p.validateProc("procedure   p q");
			Assert::AreEqual(0, result);
		}
	};
}
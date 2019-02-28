#include "TestWrapper.h"
#include "PKB.h"
#include "Preprocesser.h"
#include "Parser.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	PKB pkb = PKB();
	Preprocesser preprocesser = Preprocesser(filename);
	Parser parser = Parser();

	vector<Statement> procList = preprocesser.getProcLst();

	try {
		parser.parse(procList, 0, pkb);
	}
	catch (string exception) {
		std::cout << "Exception Occurred: " << exception << std::endl;
	}
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}

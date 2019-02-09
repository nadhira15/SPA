#include <string>
#include <vector>
#include "TNode.h"
#include "PKB.h"

PKB::PKB()
{
	TNode root{ proc };
	stmTable.push_back(&root);
}

void PKB::addStatement(NodeType type, string var)
{
	TNode node{ type };
	stmTable.push_back(&node);
	TNode varNode{ variable, var };
	node.addChild(&varNode);
	addVariable(var);
}

void PKB::addVariable(string name)
{
	varTable.push_back(name);
}

short PKB::PrvLn(short stm)
{
	if (stmTable.at(stm)->hasLeft())
	{
		return stm - 1;
	}
	else
	{
		return -1;
	}
}

short PKB::NxtLn(short stm)
{
	if (stmTable.at(stm)->hasRight())
	{
		return stm + 1;
	}
	else
	{
		return -1;
	}
}

vector<short> PKB::getAllFollowing(short index)
{
	vector<short> statements;
	TNode* node = stmTable.at(index);
	while (node->hasRight())
	{
		index++;
		statements.push_back(index);
		node = node->getRight();
	}
	return statements;
}

vector<short> PKB::getAllFollowedBy(short index)
{
	vector<short> statements;
	TNode* node = stmTable.at(index);
	while (node->hasLeft())
	{
		index++;
		statements.push_back(index);
		node = node->getLeft();
	}
	return statements;
}

vector<short> PKB::getAllFollowers()
{
	short index = 1;
	vector<short> statements;
	TNode* node = stmTable.at(index);
	while (node->hasRight())
	{
		index++;
		statements.push_back(index);
		node = node->getRight();
	}
	return statements;
}

vector<short> PKB::getAllFollowed()
{
	short index = stmTable.size() - 1;
	vector<short> statements;
	TNode* node = stmTable.at(index);
	while (node->hasLeft())
	{
		index++;
		statements.push_back(index);
		node = node->getLeft();
	}
	return statements;
}

vector<string> PKB::getAllVariable()
{
	return varTable;
}

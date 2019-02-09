#pragma once

#include <vector>
#include <string>

using namespace std;

enum NodeType {proc, read, print, variable, constant};

class TNode
{
public:
	TNode(NodeType type);
	TNode(NodeType type, string varName);
	TNode(NodeType type, TNode* parent);
	TNode(NodeType type, TNode* parent, TNode* leftSibling);
	TNode(NodeType type, short value);

	TNode* setRight(TNode* rightSibling);
	void addChild(TNode* child);

	TNode* getParent();
	TNode* getLeft();
	TNode* getRight();
	vector<TNode*> getChildren();
	NodeType getType();
	short getValue();
	string getName();

	bool hasParent();
	bool hasLeft();
	bool hasRight();
	bool hasChildren();

private:
	TNode* parent;
	TNode* leftSibling;
	TNode* rightSibling;
	vector<TNode*> children;
	NodeType type;
	short value;
	string varName;
};
#include "TNode.h"

TNode::TNode(NodeType t)
{
	type = t;
}

TNode::TNode(NodeType t, string v)
{
	type = t;
	varName = v;
}

TNode::TNode(NodeType t, TNode * p)
{
	type = t;
	parent = p;
}

TNode::TNode(NodeType t, TNode * p, TNode * l)
{
	type = t;
	parent = p;
	leftSibling = l;
}

TNode::TNode(NodeType t, short v)
{
	type = t;
	value = v;
}

TNode * TNode::setRight(TNode * r)
{
	if (rightSibling == nullptr) {
		rightSibling = r;
	}
	return rightSibling;
}

void TNode::addChild(TNode * child)
{
	children.push_back(child);
}

TNode * TNode::getParent()
{
	return parent;
}

TNode * TNode::getLeft()
{
	return leftSibling;
}

TNode * TNode::getRight()
{
	return rightSibling;
}

vector<TNode*> TNode::getChildren()
{
	return children;
}

NodeType TNode::getType()
{
	return type;
}

short TNode::getValue()
{
	return value;
}

string TNode::getName()
{
	return varName;
}

bool TNode::hasParent()
{
	return parent != nullptr;
}

bool TNode::hasLeft()
{
	return leftSibling != nullptr;
}

bool TNode::hasRight()
{
	return rightSibling != nullptr;
}

bool TNode::hasChildren()
{
	return children.size() > 0;
}

#include "operator-nodes.h"

OperatorNode::~OperatorNode()
{
	if (left_ != nullptr)
		delete left_;
	if (right_ != nullptr)
		delete right_;
}

std::string PlusOperatorNode::toString()
{
	return std::string ("+");
}

std::string MinusOperatorNode::toString()
{
	return std::string ("-");
}
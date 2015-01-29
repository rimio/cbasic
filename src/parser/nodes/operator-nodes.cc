#include "operator-nodes.h"

OperatorNode::~OperatorNode()
{
	if (left_ != nullptr)
		delete left_;
	if (right_ != nullptr)
		delete right_;
}

std::string OperatorNode::print ()
{
	if (right_ == nullptr)
	{
		return "(" + toString () + " " + left_->print () + ")";
	}
	else
	{
		return "(" + left_->print () + " " + toString () + " " + right_->print () + ")";
	}
}

std::string PlusOperatorNode::toString ()
{
	return std::string ("+");
}

std::string MinusOperatorNode::toString ()
{
	return std::string ("-");
}

std::string MultiplicationOperatorNode::toString ()
{
	return std::string ("*");
}

std::string DivisionOperatorNode::toString ()
{
	return std::string ("/");
}

std::string IntDivisionOperatorNode::toString ()
{
	return std::string ("\\");
}

std::string ModuloOperatorNode::toString ()
{
	return std::string ("mod");
}

std::string PowerOperatorNode::toString ()
{
	return std::string ("^");
}

std::string GreaterThanOperatorNode::toString ()
{
	return std::string (">");
}

std::string LessThanOperatorNode::toString ()
{
	return std::string ("<");
}

std::string GreaterThanOrEqualOperatorNode::toString ()
{
	return std::string (">=");
}

std::string LessThanOrEqualOperatorNode::toString ()
{
	return std::string ("<=");
}

std::string EqualOperatorNode::toString ()
{
	return std::string ("=");
}

std::string NotEqualOperatorNode::toString ()
{
	return std::string ("<>");
}

std::string NotOperatorNode::toString ()
{
	return std::string ("not");
}

std::string AndOperatorNode::toString ()
{
	return std::string ("and");
}

std::string OrOperatorNode::toString ()
{
	return std::string ("or");
}

std::string XorOperatorNode::toString ()
{
	return std::string ("xor");
}

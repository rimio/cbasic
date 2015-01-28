#include "value-nodes.h"
#include "error.h"

std::string ValueNode::print ()
{
	return toString ();
}

StringValueNode::StringValueNode (std::string val)
{
	value_ = std::string (val);
}

std::string StringValueNode::getValue ()
{
	return std::string (value_);
}

std::string StringValueNode::toString ()
{
	return std::string (value_);
}

std::string IntegerValueNode::toString()
{
	return std::to_string (value_);
}

std::string FloatValueNode::toString ()
{
	return std::to_string (value_);
}

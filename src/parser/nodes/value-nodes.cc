#include "value-nodes.h"
#include <cassert>
#include "error/error.h"

std::string ValueNode::print ()
{
	return toString ();
}

void ValueNode::setType (BasicType type)
{
	assert (false);
	Error::internalError ("attempting to set type of ValueNode");
}

StringValueNode::StringValueNode (std::string val)
{
	value_ = std::string (val);
}

std::string StringValueNode::getValue ()
{
	return value_;
}

std::string StringValueNode::toString ()
{
	return "\"" + value_ + "\"";
}

std::string IntegerValueNode::toString()
{
	return std::to_string (value_);
}

std::string FloatValueNode::toString ()
{
	return std::to_string (value_);
}

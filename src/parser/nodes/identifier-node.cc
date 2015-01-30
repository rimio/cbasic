#include "identifier-node.h"
#include <cassert>
#include "error/error.h"

std::string IdentifierNode::toString ()
{
	switch (type_)
	{
	case BT_INT:
		return name_ + "%";

	case BT_FLOAT:
		return name_;

	case BT_STRING:
		return name_ + "$";

	default:
		assert (false);
		return "ERROR";
	}
}

std::string IdentifierNode::print ()
{
	return toString ();
}

void IdentifierNode::setType (BasicType type)
{
	assert (false);
	Error::internalError ("attempting to set type of IdentifierNode");
}

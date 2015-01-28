#include "identifier-node.h"
#include <cassert>

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

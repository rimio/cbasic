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

std::string IdentifierNode::print (std::string indent)
{
	return indent + toString ();
}

void IdentifierNode::setType (BasicType type)
{
	assert (false);
	Error::internalError ("attempting to set type of IdentifierNode");
}

std::tuple<int, IlAddress *> IdentifierNode::generateIlCode (IlBlock *block)
{
	VariableSymbol *vsym = (VariableSymbol *) symbol_;
	return std::make_tuple (NO_ERROR, new VariableIlAddress (vsym));
}

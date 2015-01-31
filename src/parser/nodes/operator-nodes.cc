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

int ArithmeticOperatorNode::inferType ()
{
	// Disallow operation on string types
	if (left_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", getLeft ());
		return ER_FAILED;
	}
	if (right_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", getRight ());
		return ER_FAILED;
	}

	// Operation on INT and FLOAT
	if (left_->getType () == right_->getType ())
	{
		// Non-mixed operation
		setType (left_->getType ());
		return NO_ERROR;
	}
	else
	{
		// Mixed operation, apply cast
		if (left_->getType() == BT_INT)
		{
			CastOperatorNode *cast = new CastOperatorNode (left_, BT_FLOAT);
			left_ = cast;
			return left_->inferType ();
		}
		else
		{
			CastOperatorNode *cast = new CastOperatorNode (right_, BT_FLOAT);
			right_ = cast;
			return right_->inferType ();
		}

		// Done
		return NO_ERROR;
	}
}

int RelationalOperatorNode::inferType ()
{
	return NO_ERROR;
}

int LogicalOperatorNode::inferType ()
{
	// Disallow operation on string types
	if (getLeft ()->getType () == BT_STRING)
	{
		Error::semanticError ("logical operation not allowed on STRING type", getLeft ());
		return ER_FAILED;
	}
	if (getRight ()->getType () == BT_STRING)
	{
		Error::semanticError ("logical operation not allowed on STRING type", getRight ());
		return ER_FAILED;
	}

	// All ok
	return NO_ERROR;
}

std::string PlusOperatorNode::toString ()
{
	return std::string ("+");
}

int PlusOperatorNode::inferType ()
{
	if ((getLeft ()->getType () == BT_STRING)
		&& (getRight ()->getType () == BT_STRING))
	{
		// Allow PLUS as concatenation operator
		return NO_ERROR;
	}
	else
	{
		return ArithmeticOperatorNode::inferType ();
	}
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

std::string CastOperatorNode::toString ()
{
	return std::string ("cast as " + basic_type_to_string (return_type_));
}

int CastOperatorNode::inferType ()
{
	// TODO: disallow invalid casts
	return NO_ERROR;
}

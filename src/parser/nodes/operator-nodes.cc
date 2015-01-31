#include "operator-nodes.h"
#include <cassert>

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
	if (right_ != nullptr && right_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", getRight ());
		return ER_FAILED;
	}

	// Unary operator
	if (right_ == nullptr)
	{
		setType (left_->getType ());
		return NO_ERROR;
	}

	// Operation on INT or FLOAT
	if (left_->getType () == right_->getType ())
	{
		// Non-mixed operation
		setType (left_->getType ());
		return NO_ERROR;
	}
	else
	{
		// Mixed operation, cast to float
		setType (BT_FLOAT);

		// Apply cast
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
	// We know for sure the return type
	setType (BT_INT);

	if (right_ == nullptr)
	{
		assert (false);
		Error::internalError ("invalid unary relational operator found");
		return ER_FAILED;
	}

	if (left_->getType () == right_->getType ())
	{
		// Non-mixed operation
		return NO_ERROR;
	}
	else
	{
		// Disallow string types
		if (left_->getType () == BT_STRING)
		{
			Error::semanticError ("mixed-mode relational operation involving STRING", left_);
			return ER_FAILED;
		}
		if (right_->getType () == BT_STRING)
		{
			Error::semanticError ("mixed-mode relational operation involving STRING", right_);
			return ER_FAILED;
		}

		// Mixed operation, cast to int
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

int LogicalOperatorNode::inferType ()
{
	// We know for sure the return type
	setType (BT_INT);

	// Disallow operation on string types
	if (left_->getType () == BT_STRING)
	{
		Error::semanticError ("logical operation not allowed on STRING type", getLeft ());
		return ER_FAILED;
	}
	if (right_ != nullptr && right_->getType () == BT_STRING)
	{
		Error::semanticError ("logical operation not allowed on STRING type", getRight ());
		return ER_FAILED;
	}

	// Cast to integer, where needed
	int rc = NO_ERROR;
	if (left_->getType () != BT_INT)
	{
		CastOperatorNode *cast = new CastOperatorNode (left_, BT_INT);
		left_ = cast;
		rc = left_->inferType ();
	}
	if (rc == NO_ERROR && right_ != nullptr && right_->getType () != BT_INT)
	{
		CastOperatorNode *cast = new CastOperatorNode (right_, BT_INT);
		right_ = cast;
		rc = right_->inferType ();
	}

	// All ok
	return rc;
}

std::string PlusOperatorNode::toString ()
{
	return std::string ("+");
}

int PlusOperatorNode::inferType ()
{
	if ((left_->getType () == BT_STRING)
		&& (right_ != nullptr)
		&& (right_->getType () == BT_STRING))
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
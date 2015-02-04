#include "operator-nodes.h"
#include <cassert>

OperatorNode::~OperatorNode()
{
	if (left_ != nullptr)
		delete left_;
	if (right_ != nullptr)
		delete right_;
}

std::string OperatorNode::print (std::string indent)
{
	if (right_ == nullptr)
	{
		return indent + "(" + toString () + " " + left_->print ("") + ")";
	}
	else
	{
		return indent + "(" + left_->print ("") + " " + toString () + " " + right_->print ("") + ")";
	}
}

int ArithmeticOperatorNode::inferType ()
{
	// Disallow operation on string types
	if (left_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", left_);
		return ER_FAILED;
	}
	if (right_ != nullptr && right_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", right_);
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
		setType (BT_STRING);
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

int DivisionOperatorNode::inferType ()
{
	// We know the type
	setType (BT_FLOAT);

	// Disallow operation on string types
	if (left_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", left_);
		return ER_FAILED;
	}
	if (right_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", right_);
		return ER_FAILED;
	}

	// Apply cast
	int rc = NO_ERROR;
	if (left_->getType() == BT_INT)
	{
		CastOperatorNode *cast = new CastOperatorNode (left_, BT_FLOAT);
		left_ = cast;
		rc = left_->inferType ();
	}
	if (rc == NO_ERROR && right_->getType () == BT_INT)
	{
		CastOperatorNode *cast = new CastOperatorNode (right_, BT_FLOAT);
		right_ = cast;
		rc = right_->inferType ();
	}

	// Done
	return rc;
}

std::string IntDivisionOperatorNode::toString ()
{
	return std::string ("\\");
}

int IntDivisionOperatorNode::inferType ()
{
	// We know the type
	setType (BT_INT);

	// Disallow operation on string types
	if (left_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", left_);
		return ER_FAILED;
	}
	if (right_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", right_);
		return ER_FAILED;
	}

	// Apply cast
	int rc = NO_ERROR;
	if (left_->getType() == BT_FLOAT)
	{
		CastOperatorNode *cast = new CastOperatorNode (left_, BT_INT);
		left_ = cast;
		rc = left_->inferType ();
	}
	if (rc == NO_ERROR && right_->getType () == BT_FLOAT)
	{
		CastOperatorNode *cast = new CastOperatorNode (right_, BT_INT);
		right_ = cast;
		rc = right_->inferType ();
	}

	// Done
	return rc;
}

std::string ModuloOperatorNode::toString ()
{
	return std::string ("mod");
}

int ModuloOperatorNode::inferType ()
{
	// We know the type
	setType (BT_INT);

	// Disallow operation on string types
	if (left_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", left_);
		return ER_FAILED;
	}
	if (right_->getType () == BT_STRING)
	{
		Error::semanticError ("arithmetic operation not allowed on STRING type", right_);
		return ER_FAILED;
	}

	// Apply cast
	int rc = NO_ERROR;
	if (left_->getType() == BT_FLOAT)
	{
		CastOperatorNode *cast = new CastOperatorNode (left_, BT_INT);
		left_ = cast;
		rc = left_->inferType ();
	}
	if (rc == NO_ERROR && right_->getType () == BT_FLOAT)
	{
		CastOperatorNode *cast = new CastOperatorNode (right_, BT_INT);
		right_ = cast;
		rc = right_->inferType ();
	}

	// Done
	return rc;
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
	return std::string ("cast as " + BasicTypeAlias[return_type_]);
}

int CastOperatorNode::inferType ()
{
	if ((return_type_ == BT_STRING && left_->getType () != BT_STRING)
		|| (return_type_ != BT_STRING && left_->getType () == BT_STRING))
	{
		Error::semanticError ("cannot convert expression of type "
							  + BasicTypeAlias [left_->getType ()] + " to type "
							  + BasicTypeAlias [return_type_],
							  left_);
		return ER_FAILED;
	}

	return NO_ERROR;
}

CastOperatorNode::CastOperatorNode (ExpressionNode *l, BasicType ttype) : OperatorNode (l, ttype)
{
	// Save location of expression as this node's location
	setLocation (l->getLocation ());
}

#include "statement-nodes.h"

std::string AssignmentStatementNode::toString ()
{
	return "=";
}

std::string AssignmentStatementNode::print ()
{
	return identifier_->print () + " " + toString () + " " + expression_->print ();
}

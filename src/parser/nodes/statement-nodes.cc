#include "statement-nodes.h"

std::string AssignmentStatementNode::toString ()
{
	return "=";
}

std::string AssignmentStatementNode::print ()
{
	std::string this_stmt = identifier_->print () + " " + toString () + " " + expression_->print ();
	if (next_ != nullptr)
	{
		this_stmt = this_stmt + "\n" + next_->print ();
	}
	return this_stmt;
}

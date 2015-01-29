#include "statement-nodes.h"

std::string AssignmentStatementNode::toString ()
{
	return "=";
}

std::string AssignmentStatementNode::print ()
{
	std::string this_stmt = "let " + identifier_->print () + " = " + expression_->print ();
	if (next_ != nullptr)
	{
		this_stmt = this_stmt + "\n" + next_->print ();
	}
	return this_stmt;
}

std::string AllocationStatementNode::toString ()
{
	return "dim";
}

std::string AllocationStatementNode::print ()
{
	std::string this_stmt = "dim " + identifier_->print () + " (";
	ParserNode *list = dimension_list_;
	while (list != nullptr)
	{
		this_stmt += list->print ();
		if (list->getNext () != nullptr)
		{
			this_stmt += ", ";
		}
		list = list->getNext ();
	}
	this_stmt += ")";

	if (next_ != nullptr)
	{
		this_stmt = this_stmt + "\n" + next_->print ();
	}
	return this_stmt;
}

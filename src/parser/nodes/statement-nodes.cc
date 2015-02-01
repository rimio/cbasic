#include "statement-nodes.h"

AssignmentStatementNode::~AssignmentStatementNode ()
{
	if (identifier_ != nullptr)
		delete identifier_;
	if (expression_ != nullptr)
		delete expression_;
}

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

AllocationStatementNode::~AllocationStatementNode ()
{
	if (identifier_ != nullptr)
		delete identifier_;

	for (ParserNode *list = dimension_list_, *next = nullptr; list != nullptr; list = next)
	{
		next = list->getNext ();
		delete list;
	}
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

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

std::string AssignmentStatementNode::print (std::string indent)
{
	std::string this_stmt = "let " + identifier_->print ("") + " = " + expression_->print ("");
	if (next_ != nullptr)
	{
		this_stmt = this_stmt + "\n" + next_->print (indent);
	}
	return indent + this_stmt;
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

std::string AllocationStatementNode::print (std::string indent)
{
	std::string this_stmt = "dim " + identifier_->print ("") + " (";
	ParserNode *list = dimension_list_;
	while (list != nullptr)
	{
		this_stmt += list->print ("");
		if (list->getNext () != nullptr)
		{
			this_stmt += ", ";
		}
		list = list->getNext ();
	}
	this_stmt += ")";

	if (next_ != nullptr)
	{
		this_stmt = this_stmt + "\n" + next_->print (indent);
	}
	return indent + this_stmt;
}

WhileStatementNode::~WhileStatementNode ()
{
	if (condition_ != nullptr)
		delete condition_;

	for (ParserNode *list = statements_, *next = nullptr; list != nullptr; list = next)
	{
		next = list->getNext ();
		delete list;
	}
}

std::string WhileStatementNode::toString ()
{
	return "while";
}

std::string WhileStatementNode::print (std::string indent)
{
	std::string this_stmt = indent + "while " + condition_->print ("") + "\n";
	this_stmt += statements_->print (indent + "  ") + "\n";
	this_stmt += indent + "wend";

	if (next_ != nullptr)
	{
		this_stmt = this_stmt + "\n" + next_->print (indent);
	}
	return this_stmt;
}

IfStatementNode::~IfStatementNode ()
{
	if (condition_ != nullptr)
		delete condition_;

	for (ParserNode *list = then_, *next = nullptr; list != nullptr; list = next)
	{
		next = list->getNext ();
		delete list;
	}

	for (ParserNode *list = else_, *next = nullptr; list != nullptr; list = next)
	{
		next = list->getNext ();
		delete list;
	}
}

std::string IfStatementNode::toString ()
{
	return "if";
}

std::string IfStatementNode::print (std::string indent)
{
	std::string this_stmt = indent + "if " + condition_->print ("") + " then\n";
	this_stmt += then_->print (indent + "  ") + "\n";

	if (else_ != nullptr)
	{
		this_stmt += indent + "else\n";
		this_stmt += else_->print (indent + "  ") + "\n";
	}
	this_stmt += indent + "endif\n";

	if (next_ != nullptr)
	{
		this_stmt = this_stmt + next_->print (indent);
	}
	return this_stmt;
}

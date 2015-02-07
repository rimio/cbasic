#include "statement-nodes.h"
#include "error/error.h"
#include "ilang/il-instructions.h"
#include <cassert>

AssignmentStatementNode::~AssignmentStatementNode ()
{
	if (identifier_ != nullptr)
		delete identifier_;
	if (expression_ != nullptr)
		delete expression_;
}

std::tuple<int, IlAddress *> AssignmentStatementNode::generateIlCode (IlBlock *block)
{
	// Generate code for expression
	std::tuple<int, IlAddress *> ret = expression_->generateIlCode (block);
	if (std::get<0>(ret) != NO_ERROR)
	{
		return std::make_tuple(ER_FAILED, nullptr);
	}
	assert (std::get<1>(ret) != nullptr);

	// Create address from identifier
	std::tuple<int, IlAddress *> iret = identifier_->generateIlCode (block);
	if (std::get<0>(iret) != NO_ERROR)
	{
		return std::make_tuple(ER_FAILED, nullptr);
	}
	assert (std::get<1>(iret) != nullptr);

	// Create new assignment expression
	AssignmentIlInstruction *asg = new AssignmentIlInstruction (std::get<1>(iret), std::get<1>(ret));

	// Push it!
	block->addInstruction (asg);

	// All ok
	// Do NOT return result address, this is a statement!
	return std::make_tuple(NO_ERROR, nullptr);
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

std::tuple<int, IlAddress *> AllocationStatementNode::generateIlCode (IlBlock *block)
{
	assert (false);
	return std::make_tuple(ER_FAILED, nullptr);
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

std::tuple<int, IlAddress *> WhileStatementNode::generateIlCode (IlBlock *block)
{
	assert (false);
	return std::make_tuple(ER_FAILED, nullptr);
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

std::tuple<int, IlAddress *> IfStatementNode::generateIlCode (IlBlock *block)
{
	assert (false);
	return std::make_tuple(ER_FAILED, nullptr);
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

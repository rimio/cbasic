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

int AssignmentStatementNode::generateIlCode (IlBlock *block)
{
	// Generate code for expression
	int rc = expression_->generateIlCode (block);
	if (rc != NO_ERROR)
	{
		return rc;
	}
	assert (block->getLastResultAddress () != nullptr);

	// Create address from identifier
	Symbol *sym = identifier_->getSymbol ();
	if (sym->getSymbolType() != SY_VARIABLE)
	{
		Error::internalError ("non-variable symbol found in left hand side of assignment");
		return ER_FAILED;
	}
	VariableSymbol *vsym = (VariableSymbol *) sym;
	VariableIlAddress *via = new VariableIlAddress (vsym);

	// Create new assignment expression
	AssignmentIlInstruction *asg = new AssignmentIlInstruction (via, block->getLastResultAddress ());

	// Push it!
	block->addInstruction (asg);

	// All ok
	return NO_ERROR;
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

int AllocationStatementNode::generateIlCode (IlBlock *block)
{
	assert (false);
	return NO_ERROR;
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

int WhileStatementNode::generateIlCode (IlBlock *block)
{
	assert (false);
	return NO_ERROR;
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

int IfStatementNode::generateIlCode (IlBlock *block)
{
	assert (false);
	return NO_ERROR;
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

#include "statement-nodes.h"
#include "error/error.h"
#include "ilang/il-instructions.h"
#include <cassert>
#include <list>

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
	// Generate labels
	LabelIlInstruction *while_start = new LabelIlInstruction ();
	LabelIlInstruction *while_end = new LabelIlInstruction ();

	// Add start label
	block->addInstruction (while_start);

	// Generate code for condition
	std::tuple<int, IlAddress *> ret = condition_->generateIlCode (block);
	if (std::get<0>(ret) != NO_ERROR)
	{
		return std::make_tuple(ER_FAILED, nullptr);
	}
	assert (std::get<1>(ret) != nullptr);

	// Generate conditional jump
	JumpIlInstruction *cjump = new JumpIlInstruction (while_end, std::get<1>(ret), true);
	block->addInstruction (cjump);

	// Generate code for inner statements
	ParserNode *st = statements_;
	while (st != nullptr)
	{
		if (st->getNodeType () != PT_STATEMENT)
		{
			Error::internalError ("while block contains non-statement");
			return std::make_tuple(ER_FAILED, nullptr);
		}

		ret = st->generateIlCode (block);
		if (std::get<0>(ret) != NO_ERROR)
		{
			return std::make_tuple(ER_FAILED, nullptr);
		}

		st = st->getNext ();
	}

	// Generate unconditional jump
	JumpIlInstruction *jump = new JumpIlInstruction (while_start);
	block->addInstruction (jump);

	// Add end label
	block->addInstruction (while_end);

	// All ok
	// Do NOT return result address, this is a statement!
	return std::make_tuple(NO_ERROR, nullptr);
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
	if (then_ == nullptr && else_ == nullptr)
	{
		// This is an empty IF. Should not have gotten here, but just ignore it
		return std::make_tuple(NO_ERROR, nullptr);
	}

	// Create else label
	LabelIlInstruction *else_start = new LabelIlInstruction ();

	// Generate condition code
	std::tuple<int, IlAddress *> ret = condition_->generateIlCode (block);
	if (std::get<0>(ret) != NO_ERROR)
	{
		return std::make_tuple(ER_FAILED, nullptr);
	}
	assert (std::get<1>(ret) != nullptr);

	// Generate conditional jump
	JumpIlInstruction *cjump = new JumpIlInstruction (else_start, std::get<1>(ret), true);
	block->addInstruction (cjump);

	// Generate code for then block
	ParserNode *st = then_;
	while (st != nullptr)
	{
		if (st->getNodeType () != PT_STATEMENT)
		{
			Error::internalError ("THEN block contains non-statement");
			return std::make_tuple(ER_FAILED, nullptr);
		}

		ret = st->generateIlCode (block);
		if (std::get<0>(ret) != NO_ERROR)
		{
			return std::make_tuple(ER_FAILED, nullptr);
		}

		st = st->getNext ();
	}

	// Add label
	block->addInstruction (else_start);

	// Generate code for else block
	st = else_;
	while (st != nullptr)
	{
		if (st->getNodeType () != PT_STATEMENT)
		{
			Error::internalError ("ELSE block contains non-statement");
			return std::make_tuple(ER_FAILED, nullptr);
		}

		ret = st->generateIlCode (block);
		if (std::get<0>(ret) != NO_ERROR)
		{
			return std::make_tuple(ER_FAILED, nullptr);
		}

		st = st->getNext ();
	}

	// All ok
	// Do NOT return result address, this is a statement!
	return std::make_tuple(NO_ERROR, nullptr);
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

PrintStatementNode::~PrintStatementNode ()
{
	for (ParserNode *list = list_, *next = nullptr; list != nullptr; list = next)
	{
		next = list->getNext ();
		delete list;
	}
}

std::string PrintStatementNode::toString ()
{
	return "print";
}

std::string PrintStatementNode::print (std::string indent)
{
	std::string this_stmt = indent + "print ";
	ExpressionNode *expr = list_;
	while (expr != nullptr)
	{
		this_stmt += expr->print ("") + (expr->getNext () != nullptr ? ", " : "");
		expr = (ExpressionNode *) expr->getNext ();
	}

	if (next_ != nullptr)
	{
		this_stmt = this_stmt + "\n" + next_->print (indent);
	}
	return this_stmt;
}

std::tuple<int, IlAddress *> PrintStatementNode::generateIlCode (IlBlock *block)
{
	std::string format_string = "";
	unsigned int pcount = 0;

	// Make a list of all parameters
	std::list<ExpressionNode *> expr_list;
	for (ExpressionNode *e = list_; e != nullptr; e = (ExpressionNode *) e->getNext ())
	{
		expr_list.push_back (e);
		pcount ++;

		switch (e->getType ())
		{
		case BT_INT:
			format_string += "%d";
			break;

		case BT_FLOAT:
			format_string += "%f";
			break;

		case BT_STRING:
			format_string += "%s";
			break;

		default:
			break;
		}
	}
	format_string += "\n";

	// Generate parameters from back to front
	for (std::list<ExpressionNode *>::reverse_iterator it = expr_list.rbegin ();
		 it != expr_list.rend (); it ++)
	{
		// Generate code for expression
		std::tuple<int, IlAddress *> iret = (*it)->generateIlCode (block);
		if (std::get<0>(iret) != NO_ERROR)
		{
			return std::make_tuple(ER_FAILED, nullptr);
		}
		assert (std::get<1>(iret) != nullptr);

		// Insert parameter
		ParamIlInstruction *param = new ParamIlInstruction (std::get<1> (iret));
		block->addInstruction (param);
	}

	// Insert format parameter
	ParamIlInstruction *param = new ParamIlInstruction (new ConstantIlAddress (format_string));
	block->addInstruction (param);

	// Generate call
	CallIlInstruction *call = new CallIlInstruction ("printf", pcount + 1);
	block->addInstruction (call);

	// All ok
	// Do NOT return result address, this is a statement!
	return std::make_tuple(NO_ERROR, nullptr);
}

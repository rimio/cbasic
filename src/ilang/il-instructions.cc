#include "il-instructions.h"

int LabelIlInstruction::last_temp_index_ = 0;

AssignmentIlInstruction::~AssignmentIlInstruction ()
{
	if (result_ == nullptr)
		delete result_;
	if (operand1_ == nullptr)
		delete operand1_;
	if (operand2_ == nullptr)
		delete operand2_;
}

std::string AssignmentIlInstruction::toString ()
{
	std::string res = result_->toString () + " = ";
	if (operand1_ != nullptr)
		res += operand1_->toString ();
	if (operator_ != ILOP_NONE)
		res += " " + IlOperatorAlias[operator_];
	if (operand2_ != nullptr)
		res += " " + operand2_->toString ();

	return res;
}

LabelIlInstruction::LabelIlInstruction ()
{
	name_ = "label_" + std::to_string (last_temp_index_);
	last_temp_index_ ++;
}

std::string LabelIlInstruction::toString ()
{
	return name_ + ":";
}

std::string JumpIlInstruction::toString ()
{
	if (condition_ != nullptr)
	{
		if (negate_)
		{
			return "iffalse (" + condition_->toString () + ") jumpto " + target_->getName ();
		}
		else
		{
			return "iftrue (" + condition_->toString () + ") jumpto " + target_->getName ();
		}
	}
	else
	{
		return "jumpto " + target_->getName ();
	}
}

std::string ParamIlInstruction::toString ()
{
	return "param " + param_->toString ();
}

std::string CallIlInstruction::toString ()
{
	return "call " + function_;
}

#include "il-instructions.h"

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

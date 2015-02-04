#ifndef IL_INSTRUCTIONS_H_
#define IL_INSTRUCTIONS_H_

#include "il-address.h"

//
// Intermediate language operators
//
enum IlOperatorType
{
	ILOP_NONE = 0,
	ILOP_ADD,
	ILOP_SUB,
	ILOP_MUL,
	ILOP_DIV,
	ILOP_MOD
};

//
// IL operator aliases
//
static std::string IlOperatorAlias[] = { "none", "+", "-", "*", "/", "%" };

//
// Base class for intermediate language instruction
//
class IlInstruction
{
protected:
	IlInstruction () { };
public:
	virtual ~IlInstruction () { };

	virtual std::string toString () = 0;
};

//
// Assignment instruction
// Can take three forms:
//    result = operand1 operator operand2
//    result = operator operand1   	(iff operand2 == nullptr)
//    result = operand1				(iff operator == operand2 == nullptr)
//
class AssignmentIlInstruction : public IlInstruction
{
private:
	IlAddress *result_;
	IlAddress *operand1_;
	IlAddress *operand2_;
	IlOperatorType operator_;
public:
	// Constructors & destructor
	AssignmentIlInstruction (IlAddress *res, IlAddress *opr1) : result_ (res), operand1_ (opr1), operand2_ (nullptr), operator_ (ILOP_NONE) { };
	AssignmentIlInstruction (IlAddress *res, IlAddress *opr1, IlOperatorType op) : result_ (res), operand1_ (opr1), operand2_ (nullptr), operator_ (op) { };
	AssignmentIlInstruction (IlAddress *res, IlAddress *opr1, IlAddress *opr2, IlOperatorType op) : result_ (res), operand1_ (opr1), operand2_ (opr2), operator_ (op) { };
	~AssignmentIlInstruction ();

	// Instruction string representation
	std::string toString ();

	// Getters
	IlAddress *getResult () const { return result_; }
	IlAddress *getOperand1 () const { return operand1_; }
	IlAddress *getOperand2 () const { return operand2_; }
	IlOperatorType getOperator () const { return operator_; }
};

#endif
#ifndef OPERATOR_NODES_H_
#define OPERATOR_NODES_H_

#include "parser-node.h"
#include "expression-node.h"
#include "error/error.h"
#include <tuple>

//
// Operator enumeration
//
typedef enum
{
	OT_PLUS,
	OT_MINUS,
	OT_MULTIPLICATION,
	OT_DIVISION,
	OT_INTDIVISION,
	OT_MODULO,
	OT_POWER,
	OT_GT,
	OT_LT,
	OT_GT_EQ,
	OT_LT_EQ,
	OT_EQUAL,
	OT_NOT_EQUAL,
	OT_NOT,
	OT_AND,
	OT_OR,
	OT_XOR,
	OT_CAST
} OperatorType;

//
// Base class for operators
//
class OperatorNode : public ExpressionNode
{
protected:
	// Hidden constructors
	OperatorNode (ExpressionNode *operand, BasicType type) :
		left_ (operand), right_ (nullptr), return_type_ (type) { };					// Unary with type
	OperatorNode (ExpressionNode *operand) :
		left_ (operand), right_ (nullptr), return_type_ (BT_UNKNOWN) { };			// Unary
	OperatorNode (ExpressionNode *l, ExpressionNode *r) :
		left_ (l), right_ (r), return_type_ (BT_UNKNOWN) { };						// Binary

	// Operands
	ExpressionNode *left_;
	ExpressionNode *right_;

	// Return type
	BasicType return_type_;

	// Generate code for left/right and return addresses
	std::tuple<int, IlAddress *, IlAddress *> generateLeftRight (IlBlock *block);

public:
	virtual ~OperatorNode ();

	// Return operator type
	virtual OperatorType getOperatorType () const = 0;

	// Getters for lhs/rhs
	ExpressionNode *getLeft () const { return left_; }
	ExpressionNode *getRight () const { return right_; }

	// This is an operator node
	ParserNodeType getNodeType () const { return PT_OPERATOR; }

	// Get and set type
	BasicType getType () const { return return_type_; }
	void setType (BasicType type) { return_type_ = type; }

	// Print expression
	std::string print (std::string indent);

	// Return children
	std::list<ParserNode *> getChildren () { return { left_, right_ }; };
	std::list<ParserNode **> getChildrenReferences () { return { (ParserNode **) &left_, (ParserNode **) &right_ }; };
};

//
// Arithmetic operator base class
//
class ArithmeticOperatorNode : public OperatorNode
{
protected:
	ArithmeticOperatorNode (ExpressionNode *operand, BasicType type) :
		OperatorNode (operand, type) { };
	ArithmeticOperatorNode (ExpressionNode *operand) :
		OperatorNode (operand) { };
	ArithmeticOperatorNode (ExpressionNode *l, ExpressionNode *r) :
		OperatorNode (l, r) { };

public:
	int inferType ();
};

//
// Relational operator base class
//
class RelationalOperatorNode : public OperatorNode
{
protected:
	RelationalOperatorNode (ExpressionNode *operand, BasicType type) :
		OperatorNode (operand, type) { };
	RelationalOperatorNode (ExpressionNode *operand) :
		OperatorNode (operand) { };
	RelationalOperatorNode (ExpressionNode *l, ExpressionNode *r) :
		OperatorNode (l, r) { };

public:
	int inferType ();
};

//
// Relational operator base class
//
class LogicalOperatorNode : public OperatorNode
{
protected:
	LogicalOperatorNode (ExpressionNode *operand, BasicType type) :
		OperatorNode (operand, type) { };
	LogicalOperatorNode (ExpressionNode *operand) :
		OperatorNode (operand) { };
	LogicalOperatorNode (ExpressionNode *l, ExpressionNode *r) :
		OperatorNode (l, r) { };

public:
	int inferType ();
};

//
// Plus operator
//
class PlusOperatorNode : public ArithmeticOperatorNode
{
public:
	PlusOperatorNode (ExpressionNode *operand) : ArithmeticOperatorNode (operand) { };
	PlusOperatorNode (ExpressionNode *l, ExpressionNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	int inferType ();
	std::tuple<int, IlAddress *> generateIlCode (IlBlock *block);
	OperatorType getOperatorType () const { return OT_PLUS; }
};

//
// Minus operator
//
class MinusOperatorNode : public ArithmeticOperatorNode
{
public:
	MinusOperatorNode (ExpressionNode *operand) : ArithmeticOperatorNode (operand) { };
	MinusOperatorNode (ExpressionNode *l, ExpressionNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	std::tuple<int, IlAddress *> generateIlCode (IlBlock *block);
	OperatorType getOperatorType () const { return OT_MINUS; }
};

//
// Multiplication operator
//
class MultiplicationOperatorNode : public ArithmeticOperatorNode
{
public:
	MultiplicationOperatorNode (ExpressionNode *l, ExpressionNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	std::tuple<int, IlAddress *> generateIlCode (IlBlock *block);
	OperatorType getOperatorType () const { return OT_MULTIPLICATION; }
};

//
// Division operator
//
class DivisionOperatorNode : public ArithmeticOperatorNode
{
public:
	DivisionOperatorNode (ExpressionNode *l, ExpressionNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	int inferType ();
	std::tuple<int, IlAddress *> generateIlCode (IlBlock *block);
	OperatorType getOperatorType () const { return OT_DIVISION; }
};

//
// Integer division operator
//
class IntDivisionOperatorNode : public ArithmeticOperatorNode
{
public:
	IntDivisionOperatorNode (ExpressionNode *l, ExpressionNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	int inferType ();
	std::tuple<int, IlAddress *> generateIlCode (IlBlock *block);
	OperatorType getOperatorType () const { return OT_INTDIVISION; }
};

//
// Modulo operator
//
class ModuloOperatorNode : public ArithmeticOperatorNode
{
public:
	ModuloOperatorNode (ExpressionNode *l, ExpressionNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	int inferType ();
	OperatorType getOperatorType () const { return OT_MODULO; }
};

//
// Power operator
//
class PowerOperatorNode : public ArithmeticOperatorNode
{
public:
	PowerOperatorNode (ExpressionNode *l, ExpressionNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_POWER; }
};

//
// Greater than operator
//
class GreaterThanOperatorNode : public RelationalOperatorNode
{
public:
	GreaterThanOperatorNode (ExpressionNode *l, ExpressionNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_GT; }
};

//
// Less than operator
//
class LessThanOperatorNode : public RelationalOperatorNode
{
public:
	LessThanOperatorNode (ExpressionNode *l, ExpressionNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_LT; }
};

//
// Greater than or equal operator
//
class GreaterThanOrEqualOperatorNode : public RelationalOperatorNode
{
public:
	GreaterThanOrEqualOperatorNode (ExpressionNode *l, ExpressionNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_GT_EQ; }
};

//
// Less than or equal operator
//
class LessThanOrEqualOperatorNode : public RelationalOperatorNode
{
public:
	LessThanOrEqualOperatorNode (ExpressionNode *l, ExpressionNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_LT_EQ; }
};

//
// Equal operator
//
class EqualOperatorNode : public RelationalOperatorNode
{
public:
	EqualOperatorNode (ExpressionNode *l, ExpressionNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_EQUAL; }
};

//
// Not equal operator
//
class NotEqualOperatorNode : public RelationalOperatorNode
{
public:
	NotEqualOperatorNode (ExpressionNode *l, ExpressionNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_NOT_EQUAL; }
};

//
// NOT operator
//
class NotOperatorNode : public LogicalOperatorNode
{
public:
	NotOperatorNode (ExpressionNode *l) : LogicalOperatorNode (l, nullptr) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_NOT; }
};

//
// AND operator
//
class AndOperatorNode : public LogicalOperatorNode
{
public:
	AndOperatorNode (ExpressionNode *l, ExpressionNode *r) : LogicalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_AND; }
};

//
// OR operator
//
class OrOperatorNode : public LogicalOperatorNode
{
public:
	OrOperatorNode (ExpressionNode *l, ExpressionNode *r) : LogicalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_OR; }
};

//
// XOR operator
//
class XorOperatorNode : public LogicalOperatorNode
{
public:
	XorOperatorNode (ExpressionNode *l, ExpressionNode *r) : LogicalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_XOR; }
};

//
// Internal CAST operator
//
class CastOperatorNode : public OperatorNode
{
public:
	CastOperatorNode (ExpressionNode *l, BasicType ttype);

	std::string toString ();
	OperatorType getOperatorType () const { return OT_CAST; }

	// Nothing to infer
	int inferType ();
};


#endif

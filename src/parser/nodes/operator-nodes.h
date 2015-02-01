#ifndef OPERATOR_NODES_H_
#define OPERATOR_NODES_H_

#include "parser-node.h"
#include "error/error.h"

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
class OperatorNode : public TypedParserNode
{
protected:
	// Hidden constructors
	OperatorNode (TypedParserNode *operand, BasicType type) :
		left_ (operand), right_ (nullptr), return_type_ (type) { };					// Unary with type
	OperatorNode (TypedParserNode *operand) :
		left_ (operand), right_ (nullptr), return_type_ (BT_UNKNOWN) { };			// Unary
	OperatorNode (TypedParserNode *l, TypedParserNode *r) :
		left_ (l), right_ (r), return_type_ (BT_UNKNOWN) { };						// Binary

	// Operands
	TypedParserNode *left_;
	TypedParserNode *right_;

	// Return type
	BasicType return_type_;

public:
	virtual ~OperatorNode ();

	// Return operator type
	virtual OperatorType getOperatorType () const = 0;

	// Getters for lhs/rhs
	TypedParserNode *getLeft () const { return left_; }
	TypedParserNode *getRight () const { return right_; }

	// This is an operator node
	ParserNodeType getNodeType () const { return PT_OPERATOR; }

	// Get and set type
	BasicType getType () const { return return_type_; }
	void setType (BasicType type) { return_type_ = type; }

	// Print expression
	std::string print ();

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
	ArithmeticOperatorNode (TypedParserNode *operand, BasicType type) :
		OperatorNode (operand, type) { };
	ArithmeticOperatorNode (TypedParserNode *operand) :
		OperatorNode (operand) { };
	ArithmeticOperatorNode (TypedParserNode *l, TypedParserNode *r) :
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
	RelationalOperatorNode (TypedParserNode *operand, BasicType type) :
		OperatorNode (operand, type) { };
	RelationalOperatorNode (TypedParserNode *operand) :
		OperatorNode (operand) { };
	RelationalOperatorNode (TypedParserNode *l, TypedParserNode *r) :
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
	LogicalOperatorNode (TypedParserNode *operand, BasicType type) :
		OperatorNode (operand, type) { };
	LogicalOperatorNode (TypedParserNode *operand) :
		OperatorNode (operand) { };
	LogicalOperatorNode (TypedParserNode *l, TypedParserNode *r) :
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
	PlusOperatorNode (TypedParserNode *operand) : ArithmeticOperatorNode (operand) { };
	PlusOperatorNode (TypedParserNode *l, TypedParserNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	int inferType ();
	OperatorType getOperatorType () const { return OT_PLUS; }
};

//
// Minus operator
//
class MinusOperatorNode : public ArithmeticOperatorNode
{
public:
	MinusOperatorNode (TypedParserNode *operand) : ArithmeticOperatorNode (operand) { };
	MinusOperatorNode (TypedParserNode *l, TypedParserNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_MINUS; }
};

//
// Multiplication operator
//
class MultiplicationOperatorNode : public ArithmeticOperatorNode
{
public:
	MultiplicationOperatorNode (TypedParserNode *l, TypedParserNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_MULTIPLICATION; }
};

//
// Division operator
//
class DivisionOperatorNode : public ArithmeticOperatorNode
{
public:
	DivisionOperatorNode (TypedParserNode *l, TypedParserNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	int inferType ();
	OperatorType getOperatorType () const { return OT_DIVISION; }
};

//
// Integer division operator
//
class IntDivisionOperatorNode : public ArithmeticOperatorNode
{
public:
	IntDivisionOperatorNode (TypedParserNode *l, TypedParserNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	int inferType ();
	OperatorType getOperatorType () const { return OT_INTDIVISION; }
};

//
// Modulo operator
//
class ModuloOperatorNode : public ArithmeticOperatorNode
{
public:
	ModuloOperatorNode (TypedParserNode *l, TypedParserNode *r) : ArithmeticOperatorNode (l, r) { };

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
	PowerOperatorNode (TypedParserNode *l, TypedParserNode *r) : ArithmeticOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_POWER; }
};

//
// Greater than operator
//
class GreaterThanOperatorNode : public RelationalOperatorNode
{
public:
	GreaterThanOperatorNode (TypedParserNode *l, TypedParserNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_GT; }
};

//
// Less than operator
//
class LessThanOperatorNode : public RelationalOperatorNode
{
public:
	LessThanOperatorNode (TypedParserNode *l, TypedParserNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_LT; }
};

//
// Greater than or equal operator
//
class GreaterThanOrEqualOperatorNode : public RelationalOperatorNode
{
public:
	GreaterThanOrEqualOperatorNode (TypedParserNode *l, TypedParserNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_GT_EQ; }
};

//
// Less than or equal operator
//
class LessThanOrEqualOperatorNode : public RelationalOperatorNode
{
public:
	LessThanOrEqualOperatorNode (TypedParserNode *l, TypedParserNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_LT_EQ; }
};

//
// Equal operator
//
class EqualOperatorNode : public RelationalOperatorNode
{
public:
	EqualOperatorNode (TypedParserNode *l, TypedParserNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_EQUAL; }
};

//
// Not equal operator
//
class NotEqualOperatorNode : public RelationalOperatorNode
{
public:
	NotEqualOperatorNode (TypedParserNode *l, TypedParserNode *r) : RelationalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_NOT_EQUAL; }
};

//
// NOT operator
//
class NotOperatorNode : public LogicalOperatorNode
{
public:
	NotOperatorNode (TypedParserNode *l) : LogicalOperatorNode (l, nullptr) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_NOT; }
};

//
// AND operator
//
class AndOperatorNode : public LogicalOperatorNode
{
public:
	AndOperatorNode (TypedParserNode *l, TypedParserNode *r) : LogicalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_AND; }
};

//
// OR operator
//
class OrOperatorNode : public LogicalOperatorNode
{
public:
	OrOperatorNode (TypedParserNode *l, TypedParserNode *r) : LogicalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_OR; }
};

//
// XOR operator
//
class XorOperatorNode : public LogicalOperatorNode
{
public:
	XorOperatorNode (TypedParserNode *l, TypedParserNode *r) : LogicalOperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_XOR; }
};

//
// Internal CAST operator
//
class CastOperatorNode : public OperatorNode
{
public:
	CastOperatorNode (TypedParserNode *l, BasicType ttype);

	std::string toString ();
	OperatorType getOperatorType () const { return OT_CAST; }

	// Nothing to infer
	int inferType ();
};


#endif

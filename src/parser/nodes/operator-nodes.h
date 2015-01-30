#ifndef OPERATOR_NODES_H_
#define OPERATOR_NODES_H_

#include "parser-node.h"

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
	OperatorNode (ParserNode *operand) : left_ (operand), right_ (nullptr) { };		// Unary
	OperatorNode (ParserNode *l, ParserNode *r) : left_ (l), right_ (r) { };		// Binary

	// Operands
	ParserNode *left_;
	ParserNode *right_;

	// Return type
	BasicType return_type_;

public:
	virtual ~OperatorNode ();

	// Return operator type
	virtual OperatorType getOperatorType () const = 0;

	// Getters for lhs/rhs
	ParserNode *getLeft () const { return left_; }
	ParserNode *getRight () const { return right_; }

	// This is an operator node
	ParserNodeType getNodeType () const { return PT_OPERATOR; }

	// Return type
	BasicType getType () const { return return_type_; }
	void setType (BasicType type) { return_type_ = type; }

	// Print expression
	std::string print ();

	// Return children
	std::list<ParserNode *> getChildren () { return { left_, right_ }; };
	std::list<ParserNode **> getChildrenReferences () { return { &left_, &right_ }; };
};

//
// Plus operator
//
class PlusOperatorNode : public OperatorNode
{
public:
	PlusOperatorNode (ParserNode *operand) : OperatorNode (operand) { };
	PlusOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_PLUS; }
};

//
// Minus operator
//
class MinusOperatorNode : public OperatorNode
{
public:
	MinusOperatorNode (ParserNode *operand) : OperatorNode (operand) { };
	MinusOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_MINUS; }
};

//
// Multiplication operator
//
class MultiplicationOperatorNode : public OperatorNode
{
public:
	MultiplicationOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_MULTIPLICATION; }
};

//
// Division operator
//
class DivisionOperatorNode : public OperatorNode
{
public:
	DivisionOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_DIVISION; }
};

//
// Integer division operator
//
class IntDivisionOperatorNode : public OperatorNode
{
public:
	IntDivisionOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_INTDIVISION; }
};

//
// Modulo operator
//
class ModuloOperatorNode : public OperatorNode
{
public:
	ModuloOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_MODULO; }
};

//
// Power operator
//
class PowerOperatorNode : public OperatorNode
{
public:
	PowerOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_POWER; }
};

//
// Greater than operator
//
class GreaterThanOperatorNode : public OperatorNode
{
public:
	GreaterThanOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_GT; }
};

//
// Less than operator
//
class LessThanOperatorNode : public OperatorNode
{
public:
	LessThanOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_LT; }
};

//
// Greater than or equal operator
//
class GreaterThanOrEqualOperatorNode : public OperatorNode
{
public:
	GreaterThanOrEqualOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_GT_EQ; }
};

//
// Less than or equal operator
//
class LessThanOrEqualOperatorNode : public OperatorNode
{
public:
	LessThanOrEqualOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_LT_EQ; }
};

//
// Equal operator
//
class EqualOperatorNode : public OperatorNode
{
public:
	EqualOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_EQUAL; }
};

//
// Not equal operator
//
class NotEqualOperatorNode : public OperatorNode
{
public:
	NotEqualOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_NOT_EQUAL; }
};

//
// NOT operator
//
class NotOperatorNode : public OperatorNode
{
public:
	NotOperatorNode (ParserNode *l) : OperatorNode (l, nullptr) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_NOT; }
};

//
// AND operator
//
class AndOperatorNode : public OperatorNode
{
public:
	AndOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_AND; }
};

//
// OR operator
//
class OrOperatorNode : public OperatorNode
{
public:
	OrOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_OR; }
};

//
// XOR operator
//
class XorOperatorNode : public OperatorNode
{
public:
	XorOperatorNode (ParserNode *l, ParserNode *r) : OperatorNode (l, r) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_XOR; }
};

//
// Internal CAST operator
//
class CastOperatorNode : public OperatorNode
{
public:
	CastOperatorNode (ParserNode *l, BasicType ttype) : OperatorNode (l, nullptr), return_type_ (ttype) { };

	std::string toString ();
	OperatorType getOperatorType () const { return OT_CAST; }
};


#endif

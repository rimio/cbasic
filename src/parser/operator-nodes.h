#ifndef OPERATOR_NODES_H_
#define OPERATOR_NODES_H_

#include "parser-node.h"

//
// Operator enumeration
//
typedef enum
{
	OT_PLUS,
	OT_MINUS
} OperatorType;

//
// Base class for operators
//
class OperatorNode : public ParserNode
{
protected:
	// Hidden constructors
	OperatorNode (ParserNode *operand) : left_ (operand), right_ (nullptr) { };		// Unary
	OperatorNode (ParserNode *l, ParserNode *r) : left_ (l), right_ (r) { };		// Binary

	// Operands
	ParserNode *left_;
	ParserNode *right_;

public:
	virtual ~OperatorNode ();

	// Return operator type
	virtual OperatorType getOperatorType () const = 0;

	// Getters for lhs/rhs
	ParserNode *getLeft () const { return left_; }
	ParserNode *getRight () const { return right_; }

	// This is an operator node
	ParserNodeType getNodeType () const { return PT_OPERATOR; }

	// Return children
	std::list<ParserNode *> getChildren () { return { left_, right_ }; };
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

#endif

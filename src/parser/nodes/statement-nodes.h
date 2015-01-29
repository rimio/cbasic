#ifndef STATEMENT_NODES_H_
#define STATEMENT_NODES_H_

#include <string>
#include "parser-node.h"
#include "identifier-node.h"

typedef enum
{
	ST_ASSIGNMENT,
	ST_ALLOCATION
} StatementType;

//
// Statement node base class
//
class StatementNode : public ParserNode
{
protected:
	// Hidden constructor
	StatementNode () { };

public:
	// Get statement type
	virtual StatementType getStatementType () const = 0;

	// This is a statement
	ParserNodeType getNodeType () const { return PT_STATEMENT; }
};

//
// Assignment statement
//
class AssignmentStatementNode : public StatementNode
{
private:
	// Hidden constructor
	AssignmentStatementNode () { }

	// Left hand side - identifier
	IdentifierNode *identifier_;

	// Right hand side - expression
	ParserNode *expression_;

public:
	AssignmentStatementNode (IdentifierNode *iden, ParserNode *expr) : identifier_ (iden), expression_ (expr) { }

	ParserNode *getIdentifier () const { return identifier_; }
	ParserNode *getExpression () const { return expression_; }

	// Implementations of StatementNode pure virtual functions
	StatementType getStatementType () const { return ST_ASSIGNMENT; }

	// Implementations of ParserNode pure virtual functions
	std::string toString ();
	std::string print ();
	std::list<ParserNode *> getChildren () { return { identifier_, expression_ }; }
	std::list<ParserNode **> getChildrenReferences () { return { (ParserNode **) &identifier_, &expression_ }; }
};

//
// Allocation (DIM) statement
//
class AllocationStatementNode : public StatementNode
{
private:
	// Hidden constructor
	AllocationStatementNode () { };

	// Left hand side - identifier
	IdentifierNode *identifier_;

	// Right hand side - dimensions list
	ParserNode *dimension_list_;

public:
	AllocationStatementNode (IdentifierNode *iden, ParserNode *dims) : identifier_ (iden), dimension_list_ (dims) { };

	// Implementations of StatementNode pure virtual functions
	StatementType getStatementType () const { return ST_ALLOCATION; }

	// Implementations of ParserNode pure virtual functions
	std::string toString ();
	std::string print ();
	std::list<ParserNode *> getChildren () { return { identifier_, dimension_list_ }; }
	std::list<ParserNode **> getChildrenReferences () { return { (ParserNode **) &identifier_, &dimension_list_ }; }
};

#endif

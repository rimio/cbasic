#ifndef STATEMENT_NODES_H_
#define STATEMENT_NODES_H_

#include <string>
#include "parser-node.h"
#include "identifier-node.h"

typedef enum
{
	ST_ASSIGNMENT
} StatementType;

//
// Statement node base class
//
class StatementNode : public ParserNode
{
protected:
	// Hidden constructor
	StatementNode () : next_ (nullptr) { };

	// Next statement
	StatementNode *next_;

public:
	// Next node getters and setters
	StatementNode *getNext () const { return next_; }
	void setNext (StatementNode *next) { next_ = next; }

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
};

#endif

#ifndef STATEMENT_NODES_H_
#define STATEMENT_NODES_H_

#include <string>
#include "parser-node.h"
#include "identifier-node.h"

typedef enum
{
	ST_ASSIGNMENT,
	ST_ALLOCATION,
	ST_WHILE,
	ST_IF
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
	ExpressionNode *expression_;

public:
	AssignmentStatementNode (IdentifierNode *iden, ExpressionNode *expr) : identifier_ (iden), expression_ (expr) { }
	virtual ~AssignmentStatementNode ();

	IdentifierNode *getIdentifier () const { return identifier_; }
	ExpressionNode *getExpression () const { return expression_; }

	// Implementations of StatementNode pure virtual functions
	StatementType getStatementType () const { return ST_ASSIGNMENT; }

	// Implementations of ParserNode pure virtual functions
	std::string toString ();
	std::string print (std::string indent);
	std::list<ParserNode *> getChildren () { return { identifier_, expression_ }; }
	std::list<ParserNode **> getChildrenReferences () { return { (ParserNode **) &identifier_, (ParserNode **)&expression_ }; }
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
	virtual ~AllocationStatementNode ();

	IdentifierNode *getIdentifier () const { return identifier_; }
	ParserNode *getDimensionList () const { return dimension_list_; }

	// Implementations of StatementNode pure virtual functions
	StatementType getStatementType () const { return ST_ALLOCATION; }

	// Implementations of ParserNode pure virtual functions
	std::string toString ();
	std::string print (std::string indent);
	std::list<ParserNode *> getChildren () { return { identifier_, dimension_list_ }; }
	std::list<ParserNode **> getChildrenReferences () { return { (ParserNode **) &identifier_, &dimension_list_ }; }
};

//
// WHILE statement
//
class WhileStatementNode : public StatementNode
{
private:
	// Hidden constructor
	WhileStatementNode () { };

	// Condition for loop
	ExpressionNode *condition_;

	// Code do execute in loop
	ParserNode *statements_;

public:
	WhileStatementNode (ExpressionNode *cond, ParserNode *stmts) : condition_ (cond), statements_ (stmts) { };
	virtual ~WhileStatementNode ();

	ExpressionNode *getCondition () const { return condition_; }
	ParserNode *getStatements () const { return statements_; }

	// Implementations of StatementNode pure virtual functions
	StatementType getStatementType () const { return ST_WHILE; }

	// Implementations of ParserNode pure virtual functions
	std::string toString ();
	std::string print (std::string indent);
	std::list<ParserNode *> getChildren () { return { condition_, statements_ }; }
	std::list<ParserNode **> getChildrenReferences () { return { (ParserNode **) &condition_, &statements_ }; }
};

//
// IF statement
//
class IfStatementNode : public StatementNode
{
private:
	// Hidden constructor
	IfStatementNode () { };

	// Condition
	ExpressionNode *condition_;

	// Code do execute for THEN and ELSE
	ParserNode *then_;
	ParserNode *else_;

public:
	IfStatementNode (ExpressionNode *cond, ParserNode *then, ParserNode *el) : condition_ (cond), then_ (then), else_ (el) { };
	virtual ~IfStatementNode ();

	ExpressionNode *getCondition () const { return condition_; }
	ParserNode *getThen () const { return then_; }
	ParserNode *getElse () const { return else_; }

	// Implementations of StatementNode pure virtual functions
	StatementType getStatementType () const { return ST_IF; }

	// Implementations of ParserNode pure virtual functions
	std::string toString ();
	std::string print (std::string indent);
	std::list<ParserNode *> getChildren () { return { condition_, then_, else_ }; }
	std::list<ParserNode **> getChildrenReferences () { return { (ParserNode **) &condition_, &then_, &else_ }; }
};

#endif

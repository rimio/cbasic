#ifndef IDENTIFIER_NODE_H_
#define IDENTIFIER_NODE_H_

#include <string>
#include "parser-node.h"
#include "expression-node.h"
#include "symbols/basic-types.h"
#include "symbols/symbol-table.h"
#include "error/error.h"

//
// Node for an identifier
//
class IdentifierNode : public ExpressionNode
{
private:
	// Name of identifier, excluding type suffix
	std::string name_;

	// Type of identifier
	BasicType type_;

	// Associated symbol from symbol table
	Symbol *symbol_;

public:
	IdentifierNode (std::string name, BasicType type) : name_ (name), type_ (type), symbol_ (nullptr) { };

	std::string toString ();
	std::string print (std::string indent);

	// Get name and type of identifier
	std::string getName () const { return name_; }
	BasicType getType () const { return type_; }

	// Get and set associated symbol
	Symbol *getSymbol () const { return symbol_; }
	void setSymbol (Symbol *sym) { symbol_ = sym; }

	// Setting the type should be disallowed
	void setType (BasicType type);

	// Nothing to infer on values
	int inferType () { return NO_ERROR; };

	// This is an identifier
	ParserNodeType getNodeType () const { return PT_IDENTIFIER; }

	// No children
	std::list<ParserNode *> getChildren () { return { nullptr }; }
	std::list<ParserNode **> getChildrenReferences () { return { nullptr }; }
};

#endif

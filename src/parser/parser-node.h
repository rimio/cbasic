#ifndef PARSER_NODE_H_
#define PARSER_NODE_H_

#include <string>
#include <list>

//
// Parser node types
//
typedef enum
{
	PT_VALUE,
	PT_OPERATOR,
	PT_IDENTIFIER,
	PT_STATEMENT
} ParserNodeType;

//
// Generic parser node
//
class ParserNode
{
protected:
	// Disallow instances of ParserNode
	ParserNode () { };

public:
	virtual ~ParserNode () { };

	// Returns a string representation of the node
	virtual std::string toString () = 0;

	// Returns a printable representation of the node (and children)
	virtual std::string print () = 0;

	// Returns the type of parser node
	virtual ParserNodeType getNodeType () const = 0;

	// Returns a list of all children
	virtual std::list<ParserNode *> getChildren () = 0;
};

#endif

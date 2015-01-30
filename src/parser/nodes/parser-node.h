#ifndef PARSER_NODE_H_
#define PARSER_NODE_H_

#include <string>
#include <list>
#include "symbols/basic-types.h"

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
	ParserNode () : next_ (nullptr) { };

	// Next node in list
	ParserNode *next_;

public:
	virtual ~ParserNode () { };

	// Next node getters and setters
	ParserNode *getNext () const { return next_; }
	void setNext (ParserNode *next) { next_ = next; }

	// Returns a string representation of the node
	virtual std::string toString () = 0;

	// Returns a printable representation of the node (and children)
	virtual std::string print () = 0;

	// Returns the type of parser node
	virtual ParserNodeType getNodeType () const = 0;

	// Returns a list of all children
	virtual std::list<ParserNode *> getChildren () = 0;

	// Returns a list of pointers to all children addresses
	virtual std::list<ParserNode **> getChildrenReferences () = 0;
};

//
// Generic typed parser node that defines the getType () member for expression components and such
//
class TypedParserNode : public ParserNode
{
public:
	virtual BasicType getType () const = 0;
	virtual void setType (BasicType type) = 0;
};

#endif

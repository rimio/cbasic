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
	PT_VALUE 				= 1,
	PT_OPERATOR				= 2,
	PT_IDENTIFIER			= 3,

	// Only nodes inheriting TypedParserNode before this
	PT_LAST_TYPED			= 99,
	// Only nodes inheriting ParserNode after this

	PT_STATEMENT			= 100
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
// Generic typed parser node
//
class TypedParserNode : public ParserNode
{
public:
	virtual BasicType getType () const = 0;
	virtual void setType (BasicType type) = 0;
	virtual int inferType () = 0;
};

#endif
#ifndef PARSER_NODE_H_
#define PARSER_NODE_H_

#include <string>
#include <list>
#include <tuple>
#include "symbols/basic-types.h"
#include "parser/location.hh"
#include "ilang/il-block.h"
#include "ilang/il-address.h"

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
	ParserNode ();

	// Next node in list
	ParserNode *next_;

	// Position in input buffer
	yy::location location_;

public:
	virtual ~ParserNode ();

	// Unlink all children.
	// NOTE: Caller must make sure all references have been either reused or freed.
	//       Otherwise, a memory leak will occur.
	void unlink ();

	// Location getter/setter
	yy::location getLocation () const { return location_; }
	void setLocation (yy::location loc) { location_ = loc; }

	// Next node getters and setters
	ParserNode *getNext () const { return next_; }
	void setNext (ParserNode *next) { next_ = next; }

	// Returns a string representation of the node
	virtual std::string toString () = 0;

	// Returns a printable representation of the node (and children)
	virtual std::string print (std::string indent) = 0;

	// Returns the type of parser node
	virtual ParserNodeType getNodeType () const = 0;

	// Returns a list of all children
	virtual std::list<ParserNode *> getChildren () = 0;

	// Returns a list of pointers to all children addresses
	virtual std::list<ParserNode **> getChildrenReferences () = 0;

	// Generate intermediate language code
	// Return tuple: <err_code, result_address>
	virtual std::tuple<int, IlAddress *> generateIlCode (IlBlock *block) { return std::make_tuple(0, nullptr); };
};

#endif

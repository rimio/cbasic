#ifndef PARSER_NODE_H_
#define PARSER_NODE_H_

#include <string>

//
// Parser node types
//
typedef enum
{
	PT_VALUE
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

	// Returns the type of parser node
	virtual ParserNodeType getNodeType () = 0;

	// Print the toString() result to stdout, prefixed by <level> tabs
	void debugPrint (int level);
};

#endif

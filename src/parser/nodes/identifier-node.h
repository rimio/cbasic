#ifndef IDENTIFIER_NODE_H_
#define IDENTIFIER_NODE_H_

#include <string>
#include "parser-node.h"
#include "symbols/basic-types.h"

//
// Node for an identifier
//
class IdentifierNode : public TypedParserNode
{
private:
	// Name of identifier, excluding type suffix
	std::string name_;

	// Type of identifier
	BasicType type_;

public:
	IdentifierNode (std::string name, BasicType type) : name_ (name), type_ (type) { };

	std::string toString ();
	std::string print ();

	// Get name type of identifier
	std::string getName () const { return name_; }
	BasicType getType () const { return type_; }

	// Setting the type should be disallowed
	void setType (BasicType type);

	// This is an identifier
	ParserNodeType getNodeType () const { return PT_IDENTIFIER; }

	// No children
	std::list<ParserNode *> getChildren () { return { nullptr }; }
	std::list<ParserNode **> getChildrenReferences () { return { nullptr }; }
};

#endif

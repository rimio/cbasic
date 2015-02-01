#ifndef VALUE_NODES_H_
#define VALUE_NODES_H_

#include <string>
#include "parser-node.h"
#include "error/error.h"

//
// Node to hold intermediate values (like literals and folded results)
//
class ValueNode : public TypedParserNode
{
protected:
	// Hidden constructor
	ValueNode () { };

public:
	// This is a value node
	ParserNodeType getNodeType () const { return PT_VALUE; }

	// All value nodes will be printed as is
	std::string print (std::string indent);

	// All attempts to set type must fail
	void setType (BasicType type);

	// Nothing to infer on values
	int inferType () { return NO_ERROR; };

	// No children for value nodes
	std::list<ParserNode *> getChildren () { return { nullptr }; }
	std::list<ParserNode **> getChildrenReferences () { return { nullptr }; }
};

//
// Node to hold string value
//
class StringValueNode : public ValueNode
{
private:
	std::string value_;

public:
	StringValueNode (std::string val);
	std::string getValue ();

	std::string toString ();
	BasicType getType () const { return BT_STRING; }
};

//
// Node to hold integer value node
//
class IntegerValueNode : public ValueNode
{
private:
	int value_;

public:
	IntegerValueNode (int val) : value_ (val) { };
	int getValue () const { return value_; }

	std::string toString ();
	BasicType getType () const { return BT_INT; }
};

//
// Node to hold float value
//
class FloatValueNode : public ValueNode
{
private:
	float value_;

public:
	FloatValueNode (float val) : value_ (val) { };
	float getValue () const { return value_; }

	std::string toString ();
	BasicType getType () const { return BT_FLOAT; }
};

#endif

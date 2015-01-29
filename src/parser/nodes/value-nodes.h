#ifndef VALUE_NODES_H_
#define VALUE_NODES_H_

#include <string>
#include "parser-node.h"

//
// Enumeration of possible types of variables and the values of ValueNode
//
typedef enum
{
	BT_STRING,
	BT_INT,
	BT_FLOAT
} BasicType;

//
// Node to hold intermediate values (like literals and folded results)
//
class ValueNode : public ParserNode
{
protected:
	// Hidden constructor
	ValueNode () { };

public:
	// Return the type of the value
	virtual BasicType getType () = 0;

	// This is a value node
	ParserNodeType getNodeType () const { return PT_VALUE; }

	// All value nodes will be printed as is
	std::string print ();

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
	BasicType getType () { return BT_STRING; }
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
	BasicType getType () { return BT_INT; }
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
	BasicType getType () { return BT_FLOAT; }
};

#endif

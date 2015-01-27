#ifndef VALUE_NODES_H_
#define VALUE_NODES_H_

#include <string>
#include "parser-node.h"

//
// Enumeration of possible types of variables and the values of ValueNode
//
typedef enum
{
	VT_STRING,
	VT_INT,
	VT_FLOAT
} ValueType;

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
	virtual ValueType getType () = 0;

	// This is a value node
	ParserNodeType getNodeType () const { return PT_VALUE; }

	// No children for value nodes
	std::list<ParserNode *> getChildren () { return { nullptr }; }
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
	ValueType getType () { return VT_STRING; }
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
	ValueType getType () { return VT_INT; }
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
	ValueType getType () { return VT_FLOAT; }
};

#endif
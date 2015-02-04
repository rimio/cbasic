#ifndef BASIC_TYPES_H_
#define BASIC_TYPES_H_

#include <string>

//
// Enumeration of possible types of variables and the values of ValueNode
//
typedef enum
{
	BT_UNKNOWN = 0,
	BT_STRING,
	BT_INT,
	BT_FLOAT
} BasicType;

//
// Basic type aliases
//
static std::string BasicTypeAlias[] = { "UNKNOWN", "STRING", "INT", "FLOAT" };

#endif

#ifndef BASIC_TYPES_H_
#define BASIC_TYPES_H_

#include <string>

//
// Enumeration of possible types of variables and the values of ValueNode
//
typedef enum
{
	BT_UNKNOWN,
	BT_STRING,
	BT_INT,
	BT_FLOAT
} BasicType;

extern std::string basic_type_to_string (BasicType bt);

#endif

#include "basic-types.h"

std::string basic_type_to_string (BasicType bt)
{
	switch (bt)
	{
	case BT_STRING:
		return "string";

	case BT_INT:
		return "int";

	case BT_FLOAT:
		return "float";

	default:
		return "";
	}
}

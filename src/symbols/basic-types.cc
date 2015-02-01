#include "basic-types.h"

std::string basic_type_to_string (BasicType bt)
{
	switch (bt)
	{
	case BT_STRING:
		return "STRING";

	case BT_INT:
		return "INTEGER";

	case BT_FLOAT:
		return "FLOAT";

	default:
		return "unknown";
	}
}

#include "constant-folding.h"
#include <cassert>
#include "symbols/basic-types.h"
#include "parser/nodes/operator-nodes.h"
#include "parser/nodes/value-nodes.h"

ParserNode *check_types (ParserNode *node, struct TreeWalkContext *context)
{
	if (node->getNodeType () == PT_OPERATOR)
	{
		OperatorNode *op = (OperatorNode *) node;

		switch (op->getOperatorType ())
		{
		case OT_PLUS:
		case OT_MINUS:
		case OT_MULTIPLICATION:
		case OT_DIVISION:
		case OT_INTDIVISION:
		case OT_MODULO:
		case OT_POWER:
		case OT_GT:
		case OT_LT:
		case OT_GT_EQ:
		case OT_LT_EQ:
		case OT_EQUAL:
		case OT_NOT_EQUAL:
			// Integer + float ==> float + float
			break;

		default:
			break;
		}
	}

	// Nothing done
	return node;
}

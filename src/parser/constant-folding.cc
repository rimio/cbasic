#include "constant-folding.h"
#include <cassert>
#include "nodes/operator-nodes.h"
#include "nodes/value-nodes.h"
#include "symbols/basic-types.h"

ParserNode *fold_constants (ParserNode *node, struct TreeWalkContext *context)
{
	if (node == nullptr)
	{
		// Safety, if you can call this safety
		return nullptr;
	}

	// DEBUG STUB
	if (node->getNodeType () != PT_OPERATOR)
	{
		return node;
	}

	OperatorNode *op = (OperatorNode *) node;
	if (op->getOperatorType () == OT_PLUS)
	{
		PlusOperatorNode *pl = (PlusOperatorNode *) op;

		if (pl->getLeft ()->getNodeType () == PT_VALUE && pl->getRight ()->getNodeType () == PT_VALUE)
		{
			ValueNode *left = (ValueNode *) pl->getLeft ();
			ValueNode *right = (ValueNode *) pl->getRight ();

			BasicType t_left = left->getType ();
			BasicType t_right = right->getType ();

			if (t_left == t_right)
			{
				ParserNode *val = nullptr;

				switch (t_left)
				{
				case BT_INT:
					val = new IntegerValueNode ( ((IntegerValueNode *) left)->getValue () + ((IntegerValueNode *) right)->getValue ());
					break;

				case BT_FLOAT:
					val = new FloatValueNode ( ((FloatValueNode *) left)->getValue () + ((FloatValueNode *) right)->getValue ());
					break;

				case BT_STRING:
					val = new StringValueNode ( ((StringValueNode *) left)->getValue () + ((StringValueNode *) right)->getValue ());
					break;

				default:
					assert (false);
					return nullptr;
				}

				delete node;
				return val;
			}
		}
	}

	return node;
}

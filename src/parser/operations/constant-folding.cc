#include "constant-folding.h"
#include <cassert>
#include <cmath>
#include "parser/nodes/operator-nodes.h"
#include "parser/nodes/value-nodes.h"
#include "symbols/basic-types.h"

ParserNode *fold_constants (ParserNode *node, struct TreeWalkContext *context)
{
	if (node == nullptr)
	{
		// Safety, if you can call this safety
		return nullptr;
	}

	//
	// Fold casts
	//
	if (node->getNodeType () == PT_OPERATOR)
	{
		OperatorNode *op = (OperatorNode *) node;

		if (op->getOperatorType () == OT_CAST
			&& op->getLeft ()->getNodeType () == PT_VALUE)
		{
			CastOperatorNode *cast = (CastOperatorNode *) node;
			ValueNode *val = (ValueNode *) cast->getLeft ();

			switch (cast->getType ())
			{
			case BT_INT:
				switch (val->getType ())
				{
				case BT_INT:
					assert (false);
					Error::internalError ("redundant int cast detected");
					context->ret_code = ER_FAILED;
					return node;

				case BT_FLOAT:
					{
						ValueNode *new_val = new IntegerValueNode (roundf ( ((FloatValueNode *)val)->getValue () ));
						new_val->setLocation (node->getLocation ());
						node->unlink ();
						return new_val;
					}

				case BT_STRING:
					assert (false);
					Error::internalError ("cast to integer type from string type");
					context->ret_code = ER_FAILED;
					return node;

				default:
					assert (false);
					Error::internalError ("cast from unknown type");
					context->ret_code = ER_FAILED;
					return node;
				}
				break;

			case BT_FLOAT:
				switch (val->getType ())
				{
				case BT_INT:
					{
						ValueNode *new_val = new FloatValueNode ( ((IntegerValueNode *)val)->getValue () );
						new_val->setLocation (node->getLocation ());
						node->unlink ();
						return new_val;
					}

				case BT_FLOAT:
					assert (false);
					Error::internalError ("redundant float cast detected");
					context->ret_code = ER_FAILED;
					return node;

				case BT_STRING:
					assert (false);
					Error::internalError ("cast to float type from string type");
					context->ret_code = ER_FAILED;
					return node;

				default:
					assert (false);
					Error::internalError ("cast from unknown type");
					context->ret_code = ER_FAILED;
					return node;
				}
				break;

			case BT_STRING:
				assert (false);
				Error::internalError ("cast to string type");
				context->ret_code = ER_FAILED;
				return node;

			default:
				assert (false);
				Error::internalError ("cast to unknown type");
				context->ret_code = ER_FAILED;
				return node;
			}
		}
	}

	//
	// Fold unary operators
	//
	if (node->getNodeType() == PT_OPERATOR)
	{
		OperatorNode *op = (OperatorNode *) node;

		if ((op->getLeft ()->getNodeType () == PT_VALUE)
			&& (op->getRight () == nullptr))
		{
			// Get type
			ValueNode *val = (ValueNode *) op->getLeft ();

			// Fold
			ValueNode *new_val = nullptr;
			switch (op->getOperatorType ())
			{
			case OT_PLUS:
				switch (val->getType ())
				{
				case BT_INT:
					new_val = new IntegerValueNode ( ((IntegerValueNode *) val)->getValue () );
					break;
				case BT_FLOAT:
					new_val = new FloatValueNode ( ((FloatValueNode *) val)->getValue () );
					break;
				case BT_STRING:
					assert (false);
					Error::internalError ("cannot fold UNARY PLUS on STRING types");
					context->ret_code = ER_FAILED;
					return node;
				default:
					assert (false);
					Error::internalError ("operand of unknown type");
					context->ret_code = ER_FAILED;
					return node;
				}
				break;

			case OT_MINUS:
				switch (val->getType ())
				{
				case BT_INT:
					new_val = new IntegerValueNode ( -1 * ((IntegerValueNode *) val)->getValue () );
					break;
				case BT_FLOAT:
					new_val = new FloatValueNode ( -1 * ((FloatValueNode *) val)->getValue () );
					break;
				case BT_STRING:
					assert (false);
					Error::internalError ("cannot fold UNARY MINUS on STRING types");
					context->ret_code = ER_FAILED;
					return node;
				default:
					assert (false);
					Error::internalError ("operand of unknown type");
					context->ret_code = ER_FAILED;
					return node;
				}
				break;

			case OT_NOT:
				switch (val->getType ())
				{
				case BT_INT:
					new_val = new IntegerValueNode ( ((IntegerValueNode *) val)->getValue () ? 0 : -1 );
					break;
				case BT_FLOAT:
					assert (false);
					Error::internalError ("cannot fold NOT on FLOAT types");
					context->ret_code = ER_FAILED;
					return node;
				case BT_STRING:
					assert (false);
					Error::internalError ("cannot fold NOT on STRING types");
					context->ret_code = ER_FAILED;
					return node;
				default:
					assert (false);
					Error::internalError ("operand of unknown type");
					context->ret_code = ER_FAILED;
					return node;
				}
				break;

			default:
				break;
			}

			// Check return type
			if (new_val != nullptr && op->getType () != new_val->getType ())
			{
				assert (false);
				Error::internalError ("folding resulted in type different than expected type");
				context->ret_code = ER_FAILED;
				delete new_val;
				return node;
			}

			// Replace node?
			if (new_val != nullptr)
			{
				new_val->setLocation (node->getLocation ());
				node->unlink ();
				return new_val;
			}
		}
	}

	//
	// Fold binary operators
	//
	if (node->getNodeType () == PT_OPERATOR)
		{
			OperatorNode *op = (OperatorNode *) node;

			if ((op->getLeft ()->getNodeType () == PT_VALUE)
				&& (op->getRight () != nullptr)
				&& (op->getRight ()->getNodeType () == PT_VALUE))
			{
				ValueNode *left = (ValueNode *) op->getLeft ();
				ValueNode *right = (ValueNode *) op->getRight ();
				BasicType t_left = left->getType ();
				BasicType t_right = right->getType ();
				std::string s_left = "", s_right = "";
				int i_left = 0, i_right = 0;
				float f_left = .0f, f_right = .0f;

				// Must be same type
				if (t_left != t_right)
				{
					assert (false);
					Error::internalError ("attempting constant folding on operands of different type");
					context->ret_code = ER_FAILED;
					return node;
				}

				// Get left value
				switch (t_left)
				{
				case BT_INT:
					i_left = ((IntegerValueNode *) left)->getValue ();
					i_right = ((IntegerValueNode *) right)->getValue ();
					break;

				case BT_FLOAT:
					f_left = ((FloatValueNode *) left)->getValue ();
					f_right = ((FloatValueNode *) right)->getValue ();
					break;

				case BT_STRING:
					s_left = ((StringValueNode *) left)->getValue ();
					s_right = ((StringValueNode *) right)->getValue ();
					break;

				default:
					assert (false);
					Error::internalError ("operand of unknown type");
					context->ret_code = ER_FAILED;
					return node;
				}

				// Fold
				ValueNode *new_val = nullptr;
				switch (op->getOperatorType ())
				{
				case OT_PLUS:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left + i_right);
						break;
					case BT_FLOAT:
						new_val = new FloatValueNode (f_left + f_right);
						break;
					case BT_STRING:
						new_val = new StringValueNode (s_left + s_right);
						break;
					default:
						break;
					}
					break;

				case OT_MINUS:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left - i_right);
						break;
					case BT_FLOAT:
						new_val = new FloatValueNode (f_left - f_right);
						break;
					case BT_STRING:
						assert (false);
						Error::internalError ("cannot fold MINUS on STRING types");
						context->ret_code = ER_FAILED;
						return node;
					default:
						break;
					}
					break;

				case OT_MULTIPLICATION:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left * i_right);
						break;

					case BT_FLOAT:
						new_val = new FloatValueNode (f_left * f_right);
						break;

					case BT_STRING:
						assert (false);
						Error::internalError ("cannot fold MULTIPLICATION on STRING types");
						context->ret_code = ER_FAILED;
						return node;
					}
					break;

				case OT_DIVISION:
				case OT_INTDIVISION:
					switch (t_left)
					{
					case BT_INT:
						if (i_right == 0)
						{
							Error::semanticError ("division by zero while folding constants", right);
							context->ret_code = ER_FAILED;
						}
						else
						{
							new_val = new IntegerValueNode (i_left / i_right);
						}
						break;

					case BT_FLOAT:
						if (f_right == 0.0f)
						{
							Error::semanticError ("division by zero while folding constants", right);
							context->ret_code = ER_FAILED;
						}
						else
						{
							new_val = new FloatValueNode (f_left / f_right);
						}
						break;

					case BT_STRING:
						assert (false);
						Error::internalError ("cannot fold DIVISION on STRING types");
						context->ret_code = ER_FAILED;
						return node;
					}
					break;

				case OT_MODULO:
					switch (t_left)
					{
					case BT_INT:
						if (i_right == 0)
						{
							Error::semanticError ("modulo zero while folding constants", right);
							context->ret_code = ER_FAILED;
						}
						else
						{
							new_val = new IntegerValueNode (i_left % i_right);
						}
						break;
					case BT_FLOAT:
						assert (false);
						Error::internalError ("cannot fold MODULO on FLOAT types");
						context->ret_code = ER_FAILED;
						return node;
						break;
					case BT_STRING:
						assert (false);
						Error::internalError ("cannot fold MINUS on STRING types");
						context->ret_code = ER_FAILED;
						return node;
					default:
						break;
					}
					break;

				case OT_POWER:
					switch (t_left)
					{
					case BT_INT:
						{
							if (i_right >= 0)
							{
								new_val = new IntegerValueNode ((int) pow (i_left, i_right));
							}
							else
							{
								Error::semanticError ("negative INTEGER base for POWER operator", right);
								context->ret_code = ER_FAILED;
							}
						}
						break;
					case BT_FLOAT:
						if (i_left >= 0 && i_right >= 0)
						{
							new_val = new FloatValueNode (log (i_left) / log (i_right));
						}
						else
						{
							Error::semanticError ("negative FLOAT base or exponent for POWER operator", node);
							context->ret_code = ER_FAILED;
						}
						break;
					case BT_STRING:
						assert (false);
						Error::internalError ("cannot fold OR on STRING types");
						context->ret_code = ER_FAILED;
						return node;
					default:
						break;
					}
					break;

				case OT_GT:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left > i_right ? -1 : 0);
						break;
					case BT_FLOAT:
						new_val = new IntegerValueNode (f_left > f_right ? -1 : 0);
						break;
					case BT_STRING:
						new_val = new IntegerValueNode (s_left.compare (s_right) > 0 ? -1 : 0);
						break;
					default:
						break;
					}
					break;

				case OT_LT:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left < i_right ? -1 : 0);
						break;
					case BT_FLOAT:
						new_val = new IntegerValueNode (f_left < f_right ? -1 : 0);
						break;
					case BT_STRING:
						new_val = new IntegerValueNode (s_left.compare (s_right) < 0 ? -1 : 0);
						break;
					default:
						break;
					}
					break;

				case OT_GT_EQ:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left >= i_right ? -1 : 0);
						break;
					case BT_FLOAT:
						new_val = new IntegerValueNode (f_left >= f_right ? -1 : 0);
						break;
					case BT_STRING:
						new_val = new IntegerValueNode (s_left.compare (s_right) >= 0 ? -1 : 0);
						break;
					default:
						break;
					}
					break;

				case OT_LT_EQ:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left <= i_right ? -1 : 0);
						break;
					case BT_FLOAT:
						new_val = new IntegerValueNode (f_left <= f_right ? -1 : 0);
						break;
					case BT_STRING:
						new_val = new IntegerValueNode (s_left.compare (s_right) <= 0 ? -1 : 0);
						break;
					default:
						break;
					}
					break;

				case OT_EQUAL:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left == i_right ? -1 : 0);
						break;
					case BT_FLOAT:
						new_val = new IntegerValueNode (f_left == f_right ? -1 : 0);
						break;
					case BT_STRING:
						new_val = new IntegerValueNode (s_left.compare (s_right) == 0 ? -1 : 0);
						break;
					default:
						break;
					}
					break;

				case OT_NOT_EQUAL:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left != i_right ? -1 : 0);
						break;
					case BT_FLOAT:
						new_val = new IntegerValueNode (f_left != f_right ? -1 : 0);
						break;
					case BT_STRING:
						new_val = new IntegerValueNode (s_left.compare (s_right) != 0 ? -1 : 0);
						break;
					default:
						break;
					}
					break;

				case OT_AND:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left & i_right);
						break;
					case BT_FLOAT:
						assert (false);
						Error::internalError ("cannot fold AND on FLOAT types");
						context->ret_code = ER_FAILED;
						return node;
						break;
					case BT_STRING:
						assert (false);
						Error::internalError ("cannot fold AND on STRING types");
						context->ret_code = ER_FAILED;
						return node;
					default:
						break;
					}
					break;

				case OT_OR:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left | i_right);
						break;
					case BT_FLOAT:
						assert (false);
						Error::internalError ("cannot fold OR on FLOAT types");
						context->ret_code = ER_FAILED;
						return node;
						break;
					case BT_STRING:
						assert (false);
						Error::internalError ("cannot fold OR on STRING types");
						context->ret_code = ER_FAILED;
						return node;
					default:
						break;
					}
					break;

				case OT_XOR:
					switch (t_left)
					{
					case BT_INT:
						new_val = new IntegerValueNode (i_left ^ i_right);
						break;
					case BT_FLOAT:
						assert (false);
						Error::internalError ("cannot fold XOR on FLOAT types");
						context->ret_code = ER_FAILED;
						return node;
						break;
					case BT_STRING:
						assert (false);
						Error::internalError ("cannot fold XOR on STRING types");
						context->ret_code = ER_FAILED;
						return node;
					default:
						break;
					}
					break;

				default:
					// Don't treat this as actual error case; if we missed an op, we don't
					// want the user to feel this pain. Better a slower program than an error.
					break;
				}

				// Check return type
				if (new_val != nullptr && op->getType () != new_val->getType ())
				{
					assert (false);
					Error::internalError ("folding resulted in type different than expected type");
					context->ret_code = ER_FAILED;
					delete new_val;
					return node;
				}

				// Replace node?
				if (new_val != nullptr)
				{
					new_val->setLocation (node->getLocation ());
					node->unlink ();
					return new_val;
				}
			}
		}

	return node;
}

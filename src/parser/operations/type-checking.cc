#include "constant-folding.h"
#include <cassert>
#include "symbols/basic-types.h"
#include "parser/nodes/statement-nodes.h"
#include "parser/nodes/operator-nodes.h"
#include "parser/nodes/value-nodes.h"

ParserNode *check_types (ParserNode *node, struct TreeWalkContext *context)
{
	// First infer types for inferrable nodes (i.e. typed nodes)
	// NOTE: type checking errors may appear here as well.
	if (node->getNodeType () < PT_LAST_TYPED)
	{
		ExpressionNode *typed = (ExpressionNode *) node;
		int error = typed->inferType ();
		// TODO: handle error
	}

	// Check types of assignments
	if (node->getNodeType () == PT_STATEMENT)
	{
		StatementNode *st = (StatementNode *) node;

		// Assignment
		if (st->getStatementType () == ST_ASSIGNMENT)
		{
			AssignmentStatementNode *asn = (AssignmentStatementNode *) node;
			BasicType id_type = asn->getIdentifier ()->getType ();
			BasicType expr_type = asn->getExpression ()->getType ();

			if (id_type != expr_type)
			{
				// Check STRING conversions
				if (id_type == BT_STRING || expr_type == BT_STRING)
				{
					Error::semanticError ("cannot assign " + basic_type_to_string (expr_type) + " expression to "
										  + basic_type_to_string (id_type) + " variable", asn->getExpression ());
					return node;
				}

				// Create CAST node
				CastOperatorNode *cast = new CastOperatorNode (asn->getExpression (), asn->getIdentifier ()->getType ());
				int error = cast->inferType ();

				// Replace old node
				if (error == NO_ERROR)
				{
					AssignmentStatementNode *new_node = new AssignmentStatementNode (asn->getIdentifier (), cast);
					new_node->setLocation (node->getLocation ());
					node->unlink ();
					return new_node;
				}
				else
				{
					// TODO: handle error
					return node;
				}
			}
		}

		// While statement
		if (st->getStatementType () == ST_WHILE)
		{
			WhileStatementNode *wh = (WhileStatementNode *)node;
			if (wh->getCondition ()->getType () == BT_STRING)
			{
				Error::semanticError ("WHILE statement condition cannot be of type STRING", wh->getCondition ());
				return node;
			}
		}

		// If statement
		if (st->getStatementType () == ST_IF)
		{
			IfStatementNode *if_ = (IfStatementNode *)node;
			if (if_->getCondition ()->getType () == BT_STRING)
			{
				Error::semanticError ("WHILE statement condition cannot be of type STRING", if_->getCondition ());
				return node;
			}
		}
	}

	// Node not modified
	return node;
}

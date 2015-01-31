#include "find-symbols.h"
#include "symbols/symbol-table.h"
#include "parser/nodes/parser-node.h"
#include "parser/nodes/statement-nodes.h"

ParserNode *find_symbols (ParserNode *node, struct TreeWalkContext *context)
{
	// Check statements
	if (node->getNodeType () == PT_STATEMENT)
	{
		StatementNode *st = (StatementNode *) node;

		if (st->getStatementType () == ST_ASSIGNMENT)
		{
			IdentifierNode *id = ((AssignmentStatementNode *) st)->getIdentifier ();
			if (!SymbolTable::addSymbol (new VariableSymbol (id->getName (), "", id->getType ())))
			{
				Error::semanticError ("symbol '" + id->getName () + "' already declared in the same scope", node);
				// TODO: error case
			}
		}

		if (st->getStatementType () == ST_ALLOCATION)
		{
			IdentifierNode *id = ((AllocationStatementNode *) st)->getIdentifier ();
			if (!SymbolTable::addSymbol (new VariableSymbol (id->getName (), "", id->getType ())))
			{
				Error::semanticError ("symbol '" + id->getName () + "' already declared in the same scope", node);
				// TODO: error case
			}
		}
	}

	// This is a search function, return same node
	return node;
}

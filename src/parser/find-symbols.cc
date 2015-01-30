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
			SymbolTable::addSymbol (new VariableSymbol (id->getName (), "", id->getType ()));
		}

		if (st->getStatementType () == ST_ALLOCATION)
		{
			IdentifierNode *id = ((AllocationStatementNode *) st)->getIdentifier ();
			SymbolTable::addSymbol (new VariableSymbol (id->getName (), "", id->getType ()));
		}
	}

	// This is a search function, return same node
	return node;
}

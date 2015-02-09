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
			Symbol *sym = SymbolTable::getSymbol (id->getName ());
			if (sym != nullptr && (sym->getSymbolType () != SY_VARIABLE || ((VariableSymbol *) sym)->getType () != id->getType ()))
			{
				Error::semanticError ("symbol '" + id->getName () + "' already declared with different type in the same scope", node);
				context->ret_code = ER_FAILED;
			}
			else
			{
				SymbolTable::addSymbol (new VariableSymbol (id->getName (), "", id->getType ()));
			}
		}

		if (st->getStatementType () == ST_ALLOCATION)
		{
			IdentifierNode *id = ((AllocationStatementNode *) st)->getIdentifier ();
			// TODO: check
		}
	}

	// This is a search function, return same node
	return node;
}

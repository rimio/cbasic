#include "resolve-identifiers.h"
#include "symbols/symbol-table.h"
#include "parser/nodes/identifier-node.h"
#include "error/error.h"

ParserNode *resolve_identifiers (ParserNode *node, struct TreeWalkContext *context)
{
	if (node->getNodeType () == PT_IDENTIFIER)
	{
		// TODO: this is only applicable for variable identifiers within assignments and expressions
		// TODO: we should check the node stack for this context
		IdentifierNode *in = (IdentifierNode *)node;

		// Search in table
		Symbol *sym = SymbolTable::getSymbol (in->getName ());

		// Check existence
		if (sym == nullptr)
		{
			Error::semanticError ("symbol '" + in->getName () + "' not found in symbol table", node);
			context->ret_code = ER_FAILED;
			return node;
		}

		// Check it's a variable type symbol
		if (sym->getSymbolType () != SY_VARIABLE)
		{
			Error::semanticError ("VARIABLE symbol expected, found " + SymbolTypeAlias[sym->getSymbolType ()],
								  node);
			context->ret_code = ER_FAILED;
			return node;
		}

		// Check type
		VariableSymbol *vsym = (VariableSymbol *) sym;
		if (vsym->getType () != in->getType ())
		{
			Error::semanticError ("cannot map identifier of type " + BasicTypeAlias [in->getType ()]
								  + " to symbol of type " + BasicTypeAlias [vsym->getType ()],
								  node);
			context->ret_code = ER_FAILED;
			return node;
		}

		// All valid, now kiss
		in->setSymbol (sym);
	}

	// All ok
	return node;
}

#include "constant-folding.h"
#include <cassert>
#include "symbols/basic-types.h"
#include "parser/nodes/operator-nodes.h"
#include "parser/nodes/value-nodes.h"

ParserNode *check_types (ParserNode *node, struct TreeWalkContext *context)
{
	// First infer types for inferrable nodes (i.e. typed nodes)
	// NOTE: type checking errors may appear here as well.
	if (node->getNodeType () < PT_LAST_TYPED)
	{
		TypedParserNode *typed = (TypedParserNode *) node;
		int error = typed->inferType ();
		// TODO: handle error
	}

	// Node not modified
	return node;
}

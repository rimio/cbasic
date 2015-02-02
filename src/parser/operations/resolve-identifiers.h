#ifndef RESOLVE_IDENTIFIERS_H_
#define RESOLVE_IDENTIFIERS_H_

#include "parser/tree-walker.h"
#include "parser/nodes/parser-node.h"

//
// Callback function for tree walk to resolve identifiers against symbol table
//
extern ParserNode *resolve_identifiers (ParserNode *node, struct TreeWalkContext *context);

#endif

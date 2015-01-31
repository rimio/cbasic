#ifndef FIND_SYMBOLS_H_
#define FIND_SYMBOLS_H_

#include "parser/tree-walker.h"
#include "parser/nodes/parser-node.h"

//
// Callback function for tree walk to find symbols in program and add them to symbol table
//
extern ParserNode *find_symbols (ParserNode *node, struct TreeWalkContext *context);

#endif

#ifndef CONSTANT_FOLDING_H_
#define CONSTANT_FOLDING_H_

#include "parser/tree-walker.h"
#include "parser/nodes/parser-node.h"

//
// Constant folding callback function for tree walk
//
extern ParserNode *fold_constants (ParserNode *node, struct TreeWalkContext *context);

#endif

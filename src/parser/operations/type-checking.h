#ifndef TYPE_CHECKING_H_
#define TYPE_CHECKING_H_

#include "parser/tree-walker.h"
#include "parser/nodes/parser-node.h"

//
// Type checking callback function for tree walk
//
extern ParserNode *check_types (ParserNode *node, struct TreeWalkContext *context);

#endif

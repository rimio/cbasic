#ifndef TREE_WALKER_H_
#define TREE_WALKER_H_

#include <list>
#include <stack>
#include "nodes/parser-node.h"

//
// Tree walk context
//
struct TreeWalkContext
{
	std::stack<ParserNode *> node_stack;
};

//
// Walk function callback signature
// NOTE: If callback function returns node other than input node, it must call unlink() on the old node.
// The walk function will delete the old node and link the new node in it's place (parent reference and list reference).
//
typedef ParserNode * (*WALK_CALLBACK)(ParserNode *, struct TreeWalkContext *);

//
// Static class that exposes routines for walking a parse tree
//
class TreeWalker
{
private:
	// Function that will be called recursively for all children nodes
	static ParserNode *leafToRootWalk (ParserNode *node, struct TreeWalkContext *context, WALK_CALLBACK callback);

public:
	// Function for a depth-first walk with the callback called for children first and for parent afterwards.
	// root (in): node to start with
	// callback (in): function to call for each node
	// omit_root_list (in): if true, will not walk root->getNext().
	// Returns root or new tree pointer (if root was replaced).
	static ParserNode *leafToRoot (ParserNode *root, WALK_CALLBACK callback, bool omit_root_list);
};

#endif

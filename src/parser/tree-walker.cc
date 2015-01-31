#include "tree-walker.h"

ParserNode *TreeWalker::leafToRootWalk (ParserNode *node, struct TreeWalkContext *context, WALK_CALLBACK callback)
{
	if (node == nullptr)
	{
		// Safety
		return nullptr;
	}

	// Add node to stack
	context->node_stack.push (node);

	// Walk children
	std::list<ParserNode **> children_refs = node->getChildrenReferences ();
	for (std::list<ParserNode **>::iterator it = children_refs.begin ();
		 it != children_refs.end (); it ++)
	{
		// Get iterator value
		ParserNode **child = *it;

		if (child != nullptr && *child != nullptr)
		{
			// Walk child
			ParserNode *ret = leafToRootWalk (*child, context, callback);

			// Handle change
			if (ret != *child)
			{
				*child = ret;
			}
		}
	}

	// Pop node from stack
	context->node_stack.pop ();

	// Walk list
	ParserNode *next = node->getNext ();
	if (next != nullptr)
	{
		ParserNode *new_next = leafToRootWalk (next, context, callback);
		if (new_next != next)
		{
			node->setNext (new_next);
		}
	}

	// Apply to node
	ParserNode *new_node = callback (node, context);

	// All done
	return new_node;
}

ParserNode *TreeWalker::leafToRoot (ParserNode *root, WALK_CALLBACK callback, bool omit_root_list)
{
	// Create context
	struct TreeWalkContext context;

	// Unlink next if omit_root_list
	ParserNode *saved_list = root->getNext ();
	if (omit_root_list)
	{
		root->setNext (nullptr);
	}

	// Call on root
	ParserNode *new_root = leafToRootWalk (root, &context, callback);
	if (new_root != root)
	{
		delete root;
		root = nullptr;
	}

	// Relink
	new_root->setNext (saved_list);

	// All ok
	return new_root;
}
#include "tree-walker.h"
#include "error/error.h"

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
				ret->setNext ((*child)->getNext ());
				(*child)->setNext (nullptr);
				delete *child;
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
			new_next->setNext (next->getNext ());
			next->setNext (nullptr);
			delete next;
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
		root->setNext (nullptr);
		delete root;
	}

	// Relink
	if (omit_root_list)
	{
		new_root->setNext (saved_list);
	}

	// All ok
	return new_root;
}

int TreeWalker::codeGenerationWalk (ParserNode *root, IlBlock *block)
{
	while (root != nullptr)
	{
		// Check valid node types
		if (root->getNodeType () != PT_STATEMENT)
		{
			Error::internalError ("non-statement high level parser node was found during code generation");
			return ER_FAILED;
		}

		// Generate code
		int rc = root->generateIlCode (block);
		if (rc != NO_ERROR)
		{
			// Error message should have been set
			return rc;
		}

		// Advance
		root = root->getNext ();
	}

	// All ok
	return NO_ERROR;
}

#include "parser-node.h"
#include <iostream>

ParserNode::ParserNode ()
{
	next_ = nullptr;
	location_.begin.initialize(nullptr, 0, 0);
	location_.end.initialize(nullptr, 0, 0);
}

ParserNode::~ParserNode ()
{
	delete next_;
}

void ParserNode::unlink ()
{
	std::list<ParserNode **> refs = getChildrenReferences ();
	for (std::list<ParserNode **>::iterator it = refs.begin (); it != refs.end (); it ++)
	{
		*(*it) = NULL;
	}
}

#include "parser-node.h"
#include <iostream>

void ParserNode::debugPrint(int level)
{
	// Pad for level
	for (int i = 0; i < level; i ++)
		std::cout << "\t";

	// Print text
	std::cout << this->toString () << std::endl;
}

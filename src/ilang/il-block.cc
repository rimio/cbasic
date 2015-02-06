#include "il-block.h"
#include <iostream>

IlBlock::~IlBlock ()
{
	for (std::list<IlInstruction *>::iterator it = instructions_.begin ();
		 it != instructions_.end (); it ++)
	{
		// Because list of pointers
		delete *it;
	}
}

void IlBlock::addInstruction (IlInstruction *ins)
{
	instructions_.push_back (ins);
}

void IlBlock::debugPrint ()
{
	for (std::list<IlInstruction *>::iterator it = instructions_.begin ();
		 it != instructions_.end (); it ++)
	{
		std::cout << (*it)->toString () << std::endl;
	}
}

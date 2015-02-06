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
	if (ins->getInstructionType () == ILI_ASSIGNMENT)
	{
		AssignmentIlInstruction *asi = (AssignmentIlInstruction *) ins;
		last_result_ = asi->getResult ();
	}
}

void IlBlock::debugPrint ()
{
	for (std::list<IlInstruction *>::iterator it = instructions_.begin ();
		 it != instructions_.end (); it ++)
	{
		std::cout << (*it)->toString () << std::endl;
	}
}

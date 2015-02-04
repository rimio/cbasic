#include "il-block.h"

IlInstruction *IlBlock::getInstruction (int index) const
{
	if (index >= 0 && index < instructions_.size ())
	{
		return instructions_[index];
	}
	else
	{
		return nullptr;
	}
}

void IlBlock::addInstruction (IlInstruction *ins)
{
	instructions_.push_back (ins);
}

void IlBlock::join (IlBlock *block)
{
	// TODO: met, to be continued
	//instructions_.splice (instructions_.end(), block->instructions_);
}

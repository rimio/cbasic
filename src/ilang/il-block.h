#ifndef IL_BLOCK_H_
#define IL_BLOCK_H_

#include "il-instructions.h"
#include <list>
#include <tuple>

// Instruction list iterator
typedef std::list<IlInstruction *>::iterator IlInstructionIterator;
typedef std::tuple<IlInstructionIterator, IlInstructionIterator> IlBlockIterator;

//
// Instruction block
//
class IlBlock
{
protected:
	// Instruction list
	std::list<IlInstruction *> instructions_;

public:
	IlBlock () { };
	~IlBlock ();

	int getInstructionCount () const { return instructions_.size (); }

	// Add instruction at the end of the block
	void addInstruction (IlInstruction *ins);

	// Get iterator for this instruction block
	IlBlockIterator getIterator ();

	// Print the block's instrunctions, in order
	void debugPrint ();
};

#endif

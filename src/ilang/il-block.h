#ifndef IL_BLOCK_H_
#define IL_BLOCK_H_

#include "il-instructions.h"
#include <vector>

//
// Instruction block
//
class IlBlock
{
protected:
	// Instruction list
	std::vector<IlInstruction *> instructions_;

public:
	IlBlock () { };
	~IlBlock ();

	int getInstructionCount () const { return instructions_.size (); }
	IlInstruction *getInstruction (int index) const;

	// Add instruction at the end of the block
	void addInstruction (IlInstruction *ins);

	// Take all instructions of "block", add them to instruction list, empty "block"
	void join (IlBlock *block);
};

#endif

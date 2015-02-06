#ifndef IL_BLOCK_H_
#define IL_BLOCK_H_

#include "il-instructions.h"
#include <list>

//
// Instruction block
//
class IlBlock
{
protected:
	// Instruction list
	std::list<IlInstruction *> instructions_;
	IlAddress *last_result_;

public:
	IlBlock () : last_result_ (nullptr) { };
	~IlBlock ();

	int getInstructionCount () const { return instructions_.size (); }
	IlAddress *getLastResultAddress () { return last_result_; }

	// Add instruction at the end of the block
	void addInstruction (IlInstruction *ins);

	// Print the block's instrunctions, in order
	void debugPrint ();
};

#endif

#ifndef IL_PROGRAM_H_
#define IL_PROGRAM_H_

#include "il-block.h"

//
// The intermediate language program
//
class IlProgram
{
private:
	IlBlock main_block_;

public:
	IlProgram () { };

	IlBlock *getMainBlock () { return &main_block_; }

	void debugPrint ();
};

#endif

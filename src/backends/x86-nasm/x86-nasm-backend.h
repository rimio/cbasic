#ifndef X86_NASM_BACKEND_H_
#define X86_NASM_BACKEND_H_

#include "backends/interface/backend.h"
#include "ilang/il-block.h"
#include "ilang/il-program.h"
#include "ilang/il-instructions.h"
#include "x86-nasm-primitives.h"

//
// x86 NASM backend
// This backend is crappy, mainly because of the way the code is directly generated from IL
//
class X86NasmBackend : public Backend
{
private:
	NasmDataMap data_;
	NasmBssMap bss_;

	NasmInstructionList program_exit_;

	int compileAssignmentInstruction (AssignmentIlInstruction *instruction, NasmInstructionList &ilist, unsigned int &stack_offset);
	int compileInstruction (IlInstruction *instruction, NasmInstructionList &ilist, unsigned int &stack_offset);
	int compileBlock (IlBlock *block, NasmInstructionList &ilist);
	void printInstructionList (NasmInstructionList &ilist, std::ofstream &stream);

public:
	X86NasmBackend ();

	int compile (IlProgram *program, std::string output_file);
};

#endif

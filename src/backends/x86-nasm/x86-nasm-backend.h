#ifndef X86_NASM_BACKEND_H_
#define X86_NASM_BACKEND_H_

#include "backends/interface/backend.h"
#include "ilang/il-block.h"
#include "ilang/il-program.h"
#include "ilang/il-instructions.h"

class X86NasmBackend : public Backend
{
private:
	int compileBlockStorage (IlBlock *block);
	int compileInstruction (IlInstruction *instruction, std::ofstream &file);
	int compileBlock (IlBlock *block, std::ofstream &file);
	int compileSymbols (std::ofstream &file);

public:
	int compile (IlProgram *program, std::string output_file);
};

#endif

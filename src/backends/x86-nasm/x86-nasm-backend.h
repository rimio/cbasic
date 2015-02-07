#ifndef X86_NASM_BACKEND_H_
#define X86_NASM_BACKEND_H_

#include "backends/interface/backend.h"

class X86NasmBackend : public Backend
{
public:
	int compile (IlProgram *program, std::ofstream &file);
};

#endif

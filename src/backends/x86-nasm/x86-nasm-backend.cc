#include "x86-nasm-backend.h"
#include <cstdlib>
#include <cassert>
#include "error/error.h"
#include "x86-nasm-primitives.h"

//
// Printing aliases
//
#define INDENT			"  "
#define ENTRY_POINT		"__program_start"

// Location macros
#define EFFECTIVE_ADDRESS(sym)			(std::string ("[") + std::string ((sym)) + std::string ("]"))

//
// Implementation of backend
//
int X86NasmBackend::compileBlockStorage (IlBlock *block)
{
	// TODO: intelligent storage for temps
	return NO_ERROR;
}

int X86NasmBackend::compileInstruction (IlInstruction *instruction, std::ofstream &file)
{
	IlInstructionType itype = instruction->getInstructionType ();

	if (itype == ILI_ASSIGNMENT)
	{
	}
	else if (itype == ILI_LABEL)
	{
	}

	// All ok
	return NO_ERROR;
}

int X86NasmBackend::compileBlock (IlBlock *block, std::ofstream &file)
{
	// First stage - decide locations for operation results
	if (compileBlockStorage (block) != NO_ERROR)
	{
		return ER_FAILED;
	}

	// Second stage - actual compilation
	IlBlockIterator block_it = block->getIterator ();
	for (IlInstructionIterator it = std::get<0> (block_it);
		 it != std::get<1> (block_it); it ++)
	{
		IlInstruction *ins = (*it);
		if (compileInstruction (ins, file) != NO_ERROR)
		{
			return ER_FAILED;
		}
	}

	return NO_ERROR;
}

int X86NasmBackend::compile (IlProgram *program, std::string output_file)
{
	// Compile program to Nasm primitives

	// Determine assembly file
	std::string assembly_file = output_file + ".asm";
	std::cout << "[x86-nasm] generating assembly file" << std::endl;

	// Open assembly file
	std::ofstream assembly;
	assembly.open (assembly_file, std::ios::out);

	// Write symbols in data section
	assembly << "section .data" << std::endl;
	for (NasmDataMap::iterator it = data_.begin (); it != data_.end (); it ++)
	{
		assembly << INDENT << (*it).second->toString () << std::endl;
	}
	assembly << std::endl;

	// Write symbols in bss section
	assembly << "section .bss" << std::endl;
	for (NasmBssMap::iterator it = bss_.begin (); it != bss_.end (); it ++)
	{
		assembly << INDENT << (*it).second->toString () << std::endl;
	}
	assembly << std::endl;

	// Start .text section
	assembly << "section .text" << std::endl;
	assembly << INDENT << "global " << ENTRY_POINT << std::endl << std::endl;

	// TODO: Write internal subroutines

	// TODO: Write program subroutines

	// Write main block
	assembly << ENTRY_POINT << ":" << std::endl;

	// Write exit point
	assembly << INDENT << "; program exit point" << std::endl;
	//assembly << INDENT << MOV << EAX << SEPARATOR << "1" << std::endl;
	//assembly << INDENT << MOV << EBX << SEPARATOR << "0" << std::endl;
	//assembly << INDENT << INT << "80h" << std::endl;

	// Close assembly
	assembly.close ();

	// Determine object file
	std::string object_file = output_file + ".o";

	// Call NASM
	std::string nasm_command =
		"nasm -f aout -o " + object_file + " " + assembly_file;
	std::cout << "[x86-nasm] running assembler: " << nasm_command << std::endl;
	int nasm_rc = system (nasm_command.c_str ());
	if (nasm_rc != NO_ERROR)
	{
		Error::internalError ("[x86-nasm] nasm exited with error code "	+ std::to_string (nasm_rc));
		return ER_FAILED;
	}

	// Call linker
	std::string ld_command =
		"ld -e " ENTRY_POINT " -o " + output_file + " -m elf_i386 " + object_file;
	std::cout << "[x86-nasm] running linker: " << ld_command << std::endl;
	int ld_rc = system (ld_command.c_str ());
	if (ld_rc != NO_ERROR)
	{
		Error::internalError ("[x86-nasm] ld exited with error code " + std::to_string (ld_rc));
		return ER_FAILED;
	}

	// All ok
	return NO_ERROR;
}

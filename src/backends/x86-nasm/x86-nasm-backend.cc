#include "x86-nasm-backend.h"
#include "error/error.h"
#include <cstdlib>

//
// Printing aliases for instructions and registers
//
#define INDENT			"  "
#define SEPARATOR 		", "

#define ENTRY_POINT		"__program_start"

#define EAX		"eax"
#define EBX		"ebx"

#define MOV		"mov  "
#define INT		"int  "

int X86NasmBackend::compileBlock (IlBlock *block, std::ofstream &file)
{
	return NO_ERROR;
}

int X86NasmBackend::compileSymbols (std::ofstream &file)
{
	std::tuple< std::unordered_map<std::string, Symbol *>::iterator,
				std::unordered_map<std::string, Symbol *>::iterator > iterator;

	iterator = SymbolTable::getIterator ();
	for (std::unordered_map<std::string, Symbol *>::iterator it = std::get<0>(iterator);
		 it != std::get<1>(iterator); it ++)
	{
		Symbol *sym = (*it).second;
		if (sym->getSymbolType () != SY_VARIABLE)
		{
			// Only variables
			continue;
		}
		VariableSymbol *vsym = (VariableSymbol *) sym;

		// Write symbol
		file << INDENT << vsym->getName () << ":";
		switch (vsym->getType ())
		{
		case BT_INT:
			file << "resd 1 ; INT";
			break;

		case BT_FLOAT:
			file << "resd 1 ; FLOAT";
			break;

		case BT_STRING:
			file << "resb 256 ; STRING";
			break;

		default:
			Error::internalError ("[x86-nasm] sug pula unknown symbol type");
			break;
		}
		file << std::endl;
	}

	// All ok
	return NO_ERROR;
}

int X86NasmBackend::compile (IlProgram *program, std::string output_file)
{
	// Determine assembly file
	std::string assembly_file = output_file + ".asm";
	std::cout << "[x86-nasm] generating assembly file" << std::endl;

	// Open assembly file
	std::ofstream assembly;
	assembly.open (assembly_file, std::ios::out);

	// Write symbols in bss section
	assembly << "section .bss" << std::endl;
	if (compileSymbols (assembly) != NO_ERROR)
	{
		return ER_FAILED;
	}
	assembly << std::endl;

	// Start .text section
	assembly << "section .text" << std::endl;
	assembly << INDENT << "global " << ENTRY_POINT << std::endl << std::endl;

	// Write main block
	assembly << ENTRY_POINT << ":" << std::endl;
	if (compileBlock (program->getMainBlock (), assembly) != NO_ERROR)
	{
		return ER_FAILED;
	}

	// Write exit point
	assembly << INDENT << "; program exit point" << std::endl;
	assembly << INDENT << MOV << EAX << SEPARATOR << "1" << std::endl;
	assembly << INDENT << MOV << EBX << SEPARATOR << "0" << std::endl;
	assembly << INDENT << INT << "80h" << std::endl;

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
		Error::internalError ("[x86-nasm] nasm exited with error code " + std::to_string (nasm_rc));
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

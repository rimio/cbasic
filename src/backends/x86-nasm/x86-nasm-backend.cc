#include "x86-nasm-backend.h"
#include "error/error.h"
#include <cstdlib>

//
// Printing aliases
//
#define INDENT			"  "
#define SEPARATOR 		", "

#define ENTRY_POINT		"__program_start"

// Registers
#define EAX		"eax"
#define EBX		"ebx"

// Instructions
#define MOV		"mov  "
#define INT		"int  "
#define POP		"pop  "
#define ADD		"add  "

// Location macros
#define EFFECTIVE_ADDRESS(sym)			(std::string ("[") + std::string ((sym)) + std::string ("]"))

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
		// Fetch instrcution data for easy access
		AssignmentIlInstruction *asi = (AssignmentIlInstruction *) instruction;
		IlAddress *r_addr = asi->getResult ();
		IlAddress *op1_addr = asi->getOperand1 ();
		IlAddress *op2_addr = asi->getOperand2 ();
		IlOperatorType op = asi->getOperator ();
		IlAddressType r_atype = r_addr->getAddressType ();
		IlAddressType op1_atype = op1_addr->getAddressType ();
		IlAddressType op2_atype;
		if (op2_addr != nullptr)
			op2_atype = op2_addr->getAddressType ();
		BasicType r_type = r_addr->getType ();
		BasicType op1_type = op1_addr->getType ();
		BasicType op2_type;
		if (op2_addr != nullptr)
			BasicType op2_type = op2_addr->getType ();

		std::string op1_location = "";
		std::string op2_location = "xxx";

		// Check types
		if ((r_type != op1_type) || (op2_addr != nullptr && r_type != op2_type))
		{
			Error::internalError ("[x86-nasm] instruction operands do not match result type");
			return ER_FAILED;
		}

		// Fetch operands
		switch (op1_atype)
		{
		case ILA_VARIABLE:
			{
				// Fetch actual location
				VariableIlAddress *vil = (VariableIlAddress *) op1_addr;
				VariableSymbol *sym = vil->getSymbol ();

				if (op1_type != BT_STRING)
				{
					op1_location = EFFECTIVE_ADDRESS (sym->getName ());
				}
				else
				{
					// We only keep the string pointer, all string operations should accept it
					op1_location = sym->getName ();
				}

				// Move operand to EAX
				file << INDENT << MOV << EAX << SEPARATOR << op1_location << std::endl;
				op1_location = EAX;
			}
			break;

		case ILA_CONSTANT:
			{
				ConstantIlAddress *cil = (ConstantIlAddress *) op1_addr;
				if (cil->getType () != BT_INT && cil->getType() != BT_FLOAT)
				{
					Error::internalError ("[x86-nasm] invalid immediate type");
				}

				// Get hexadecimal representation
				int ival, *addr = nullptr;
				float fval;
				if (cil->getType () == BT_INT)
				{
					ival = cil->getInt ();
					addr = (int *) &ival;
				}
				else
				{
					fval = cil->getFloat ();
					addr = (int *) &fval;
				}

				// Move immediate to EAX
				file << INDENT << MOV << EAX << SEPARATOR << std::hex << *addr << "h";
				file << " ; " << cil->toString () << std::endl;
			}
			break;

		case ILA_TEMPORARY:
			file << INDENT << POP << EAX << std::endl;
			break;

		default:
			Error::internalError ("[x86-nasm] invalid address type");
			return ER_FAILED;
		}

		// Write indent
		file << INDENT;

		// Write instruction
		switch (op)
		{
		case ILOP_ADD:
			switch (r_type)
			{
			case BT_INT:
				file << ADD << op1_location << SEPARATOR << op2_location;
				break;

			case BT_FLOAT:
				break;

			case BT_STRING:
				break;

			default:
				Error::internalError ("[x86-nasm] invalid result type");
				return ER_FAILED;
			}
			break;

		default:
			Error::internalError ("[x86-nasm] invalid operator");
			return ER_FAILED;
		}

		// Write instruction comment
		file << " ; " << asi->toString () << std::endl;

		// Store output
	}
	else if (itype == ILI_LABEL)
	{
		LabelIlInstruction *lins = (LabelIlInstruction *) instruction;
		file << lins->getName () << ":" << std::endl;
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
			Error::internalError ("[x86-nasm] unknown symbol type");
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
		assembly.close ();
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

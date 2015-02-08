#include "x86-nasm-backend.h"
#include <cstdlib>
#include <cassert>
#include "error/error.h"
#include "ilang/il-address.h"
#include "x86-nasm-primitives.h"

//
// Printing aliases
//
#define INDENT			"  "
#define ENTRY_POINT		"__main"

//
// Implementation of backend
//
X86NasmBackend::X86NasmBackend ()
{
	NasmInstruction *ins;

	// Create program exit instruction list
	ins = new MovNasmInstruction (
				new RegisterNasmAddress (REG_EAX),
				new ImmediateNasmAddress ((unsigned int) 1)
			);
	ins->setComment ("program exit point");
	program_exit_.push_back (ins);
	ins = new MovNasmInstruction (
				new RegisterNasmAddress (REG_EBX),
				new ImmediateNasmAddress ((unsigned int) 0)
			);
	program_exit_.push_back (ins);
	ins = new IntNasmInstruction (0x80);
	program_exit_.push_back (ins);
}

int X86NasmBackend::compileAssignmentInstruction (AssignmentIlInstruction *instruction, NasmInstructionList &ilist, unsigned int &stack_offset)
{
	IlAddress *r_iladdr = instruction->getResult ();
	IlAddress *op1_iladdr = instruction->getOperand1 ();
	IlAddress *op2_iladdr = instruction->getOperand2 ();
	IlOperatorType op_type = instruction->getOperator ();

	assert (r_iladdr != nullptr && op1_iladdr != nullptr);

	// Get result address
	NasmAddress *r_addr = NasmAddress::fromIl (r_iladdr, data_, bss_, stack_offset);
	if (r_addr == nullptr)
	{
		return ER_FAILED;
	}

	// Get op1 address
	NasmAddress *op1_addr = NasmAddress::fromIl (op1_iladdr, data_, bss_, stack_offset);
	if (op1_addr == nullptr)
	{
		return ER_FAILED;
	}

	//
	// One operand case
	//
	if (op2_iladdr == nullptr)
	{
		return NO_ERROR;
	}

	// Get op2 address
	NasmAddress *op2_addr = NasmAddress::fromIl (op2_iladdr, data_, bss_, stack_offset);
	if (op2_addr == nullptr)
	{
		return ER_FAILED;
	}

	//
	// Two operand case
	//

	return NO_ERROR;
}

int X86NasmBackend::compileInstruction (IlInstruction *instruction, NasmInstructionList &ilist, unsigned int &stack_offset)
{
	IlInstructionType itype = instruction->getInstructionType ();

	if (itype == ILI_ASSIGNMENT)
	{
		AssignmentIlInstruction *asi = (AssignmentIlInstruction *) instruction;
		return compileAssignmentInstruction (asi, ilist, stack_offset);
	}
	else if (itype == ILI_LABEL)
	{
	}

	// All ok
	return NO_ERROR;
}

int X86NasmBackend::compileBlock (IlBlock *block, NasmInstructionList &ilist)
{
	NasmInstruction *ins;

	// Stack offset
	unsigned int stack_offset = 0;

	// Compile instructions
	IlBlockIterator block_it = block->getIterator ();
	for (IlInstructionIterator it = std::get<0> (block_it);
		 it != std::get<1> (block_it); it ++)
	{
		IlInstruction *ins = (*it);
		if (compileInstruction (ins, ilist, stack_offset) != NO_ERROR)
		{
			return ER_FAILED;
		}
	}

	// Save frame
	// We are building the following instructions at the beginning of the block:
	//   PUSH EBP
	//   MOV  EBP, ESP
	//   SUB  ESP, stack_offset
	// We are inserting them in reverse order so they get executed in the correct order
	ins = new SubNasmInstruction (
				new RegisterNasmAddress (REG_ESP),
				new ImmediateNasmAddress ((unsigned int) stack_offset)
			);
	ilist.push_front (ins);
	ins = new MovNasmInstruction (
				new RegisterNasmAddress (REG_EBP),
				new RegisterNasmAddress (REG_ESP)
			);
	ilist.push_front (ins);
	ins = new PushNasmInstruction (
				new RegisterNasmAddress (REG_EBP)
			);
	ins->setComment ("save frame");
	ilist.push_front (ins);

	return NO_ERROR;
}

void X86NasmBackend::printInstructionList (NasmInstructionList &ilist, std::ofstream &stream)
{
	for (NasmInstructionList::iterator it = ilist.begin ();
		 it != ilist.end (); it ++)
	{
		if ((*it)->getComment ().compare ("") != 0)
		{
			// We have a comment, print it
			stream << INDENT << "; " << (*it)->getComment () << std::endl;
		}

		stream << INDENT << (*it)->toString () << std::endl;
	}
}

int X86NasmBackend::compile (IlProgram *program, std::string output_file)
{
	// Compile program to Nasm primitives
	NasmInstructionList main_block;
	if (compileBlock (program->getMainBlock (), main_block) != NO_ERROR)
	{
		// Error should have been printed
		return ER_FAILED;
	}

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
	printInstructionList (main_block, assembly);

	// Write exit point
	printInstructionList (program_exit_, assembly);

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

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
#define ENTRY_POINT		"main"

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

int X86NasmBackend::compileAssignmentInstruction (AssignmentIlInstruction *instruction, NasmInstructionList &ilist, NasmStackMap &stack)
{
	IlAddress *r_iladdr = instruction->getResult ();
	IlAddress *op1_iladdr = instruction->getOperand1 ();
	IlAddress *op2_iladdr = instruction->getOperand2 ();
	IlOperatorType op_type = instruction->getOperator ();

	assert (r_iladdr != nullptr && op1_iladdr != nullptr);

	// Get result address
	NasmAddress *r_addr = NasmAddress::fromIl (r_iladdr, data_, bss_, stack);
	if (r_addr == nullptr)
	{
		return ER_FAILED;
	}
	if (r_addr->getAddressType () == ADDR_IMMEDIATE)
	{
		Error::internalError ("[x86-nasm] immediate address as operation result");
		return ER_FAILED;
	}

	// Get op1 address
	NasmAddress *op1_addr = NasmAddress::fromIl (op1_iladdr, data_, bss_, stack);
	if (op1_addr == nullptr)
	{
		return ER_FAILED;
	}
	if (r_iladdr->getType () != op1_iladdr->getType ())
	{
		Error::internalError ("[x86-nasm] result address type differs from operand1 address type");
		return ER_FAILED;
	}

	//
	// One operand case
	//
	if (op2_iladdr == nullptr)
	{
		if (op_type == ILOP_NONE)
		{
			// Simple assignment
			if (r_iladdr->getType () == BT_STRING)
			{
				// TODO: string case, call copy subroutine
				assert (false);
			}
			else
			{
				if (r_addr->isMemory () && op1_addr->isMemory ())
				{
					// memory -> memory / FLOAT or INT
					MovNasmInstruction *mov = nullptr;

					// memory -> register
					mov = new MovNasmInstruction (new RegisterNasmAddress (REG_EAX), op1_addr);
					mov->setComment (instruction->toString ());
					ilist.push_back (mov);

					// register -> memory
					mov = new MovNasmInstruction (r_addr, new RegisterNasmAddress (REG_EAX));
					ilist.push_back (mov);

					return NO_ERROR;
				}
				else
				{
					// we can do it in one MOV instruction
					MovNasmInstruction *mov = new MovNasmInstruction (r_addr, op1_addr);
					mov->setComment (instruction->toString ());
					ilist.push_back (mov);
					return NO_ERROR;
				}
			}
		}
		else if (op_type == ILOP_NOT)
		{
			// TODO: negation
			assert (false);
		}
		else
		{
			Error::internalError ("[x86-nasm] invalid single-operand instruction '" + IlOperatorAlias[op_type] + "'");
			return ER_FAILED;
		}

		// All ok
		return NO_ERROR;
	}

	// Get op2 address
	NasmAddress *op2_addr = NasmAddress::fromIl (op2_iladdr, data_, bss_, stack);
	if (op2_addr == nullptr)
	{
		return ER_FAILED;
	}
	if (r_iladdr->getType () != op2_iladdr->getType ())
	{
		Error::internalError ("[x86-nasm] result address type differs from operand2 address type");
		return ER_FAILED;
	}

	//
	// Two operand case
	//
	if (r_iladdr->getType () == BT_STRING)
	{
		if (op_type != ILOP_ADD)
		{
			Error::internalError ("[x86-nasm] non-concatenation operator '" + IlOperatorAlias[op_type] + "' on string types");
			return ER_FAILED;
		}

		// TODO: string case, call concat subroutine
		assert (false);
		return NO_ERROR;
	}

	if (r_iladdr->getType () == BT_FLOAT)
	{
		bool push_op1 = false;
		bool push_op2 = false;

		// Keep operator addresses here
		NasmAddress *i_op1_addr = op1_addr;
		NasmAddress *i_op2_addr = op2_addr;

		//
		// Prepare context
		//

		// Bring op1 to memory
		if (!i_op1_addr->isMemory ())
		{
			// Push op1 on stack
			PushNasmInstruction *push = new PushNasmInstruction (i_op1_addr);
			ilist.push_back (push);

			// New address
			i_op1_addr = new MemoryBasedNasmAddress (REG_ESP, 0);
			push_op1 = true;
		}

		// ST0 = op1
		FldNasmInstruction *fld = new FldNasmInstruction (i_op1_addr);
		ilist.push_back (fld);

		// Bring op2 to memory
		if (!i_op2_addr->isMemory ())
		{
			// Push op1 on stack
			PushNasmInstruction *push = new PushNasmInstruction (i_op2_addr);
			ilist.push_back (push);

			// New address
			i_op2_addr = new MemoryBasedNasmAddress (REG_ESP, 0);
			push_op2 = true;
		}

		//
		// Execute operation
		//
		NasmInstruction *inst = nullptr;
		switch (op_type)
		{
		case ILOP_ADD:
			inst = new FaddNasmInstruction (i_op2_addr);
			break;

		case ILOP_SUB:
			inst = new FsubNasmInstruction (i_op2_addr);
			break;

		case ILOP_MUL:
			inst = new FmulNasmInstruction (i_op2_addr);
			break;

		case ILOP_DIV:
			inst = new FdivNasmInstruction (i_op2_addr);
			break;

		default:
			Error::internalError ("invalid '" + IlOperatorAlias[op_type] + "' operator for floating point context");
			return ER_FAILED;
		}

		inst->setComment (instruction->toString ());
		ilist.push_back (inst);

		//
		// Get result
		//
		if (r_addr->isMemory ())
		{
			// ST0 -> mem, pop ST0
			FstpNasmInstruction *fstp = new FstpNasmInstruction (r_addr);
			ilist.push_back (fstp);
		}
		else
		{
			// SUB  ESP, 4
			// FSPT [ESP]
			// POP  r
			SubNasmInstruction *sub = new SubNasmInstruction (
						new RegisterNasmAddress (REG_ESP),
						new ImmediateNasmAddress ((unsigned int) 4)
					);
			ilist.push_back (sub);

			FstpNasmInstruction *fstp = new FstpNasmInstruction (new MemoryBasedNasmAddress (REG_ESP, 0));
			ilist.push_back (fstp);

			PopNasmInstruction *pop = new PopNasmInstruction (r_addr);
			ilist.push_back (pop);
		}

		//
		// Pop operands
		//
		if (push_op1 || push_op2)
		{
			unsigned int amount = (push_op1 ? 4 : 0) + (push_op2 ? 4 : 0);

			AddNasmInstruction *add = new AddNasmInstruction (
						new RegisterNasmAddress (REG_ESP),
						new ImmediateNasmAddress (amount)
					);
			ilist.push_back (add);
		}

		return NO_ERROR;
	}

	if (r_iladdr->getType () == BT_INT)
	{
		// These will be used by the actual operations
		NasmAddress *i_dest_addr = nullptr;
		NasmAddress *i_opr_addr = nullptr;

		// We'll store the instruction here
		NasmInstruction *inst = nullptr;

		// Determine instruction operand addresses
		if (op1_addr->isMemory ())
		{
			if (op2_addr->isMemory ())
			{
				// OP1 and OP2 in memory
				if (r_addr->isMemory())
				{
					// MOV EAX, [op1]
					// XXX EAX, [op2]
					// MOV [r], EAX
					i_dest_addr = new RegisterNasmAddress (REG_EAX);
					NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
					ilist.push_back (aux);
					i_opr_addr = op2_addr;
				}
				else
				{
					// MOV r, [op1]
					// XXX r, [op2]
					i_dest_addr = r_addr;
					NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
					ilist.push_back (aux);
					i_opr_addr = op2_addr;
				}
			}
			else
			{
				// Just OP1 in memory
				if (r_addr->isMemory())
				{
					if (op2_addr->getAddressType () == ADDR_IMMEDIATE)
					{
						// MOV EAX, [op1]
						// XXX EAX, op2
						// MOV [r], EAX
						i_dest_addr = new RegisterNasmAddress (REG_EAX);
						NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
						ilist.push_back (aux);
						i_opr_addr = op2_addr;
					}
					else
					{
						assert (op2_addr->getAddressType () == ADDR_REGISTER);

						if (IL_MAINTAIN_OPERAND_ORDER (op_type))
						{
							RegisterNasmAddress *reg = (RegisterNasmAddress *) op2_addr;
							if (reg->getRegister () == REG_EAX)
							{
								i_dest_addr = new RegisterNasmAddress (REG_EBX);
							}
							else
							{
								i_dest_addr = new RegisterNasmAddress (REG_EAX);
							}

							// MOV aux_reg, [op1]
							// XXX aux_reg, op2
							// MOV [r], aux_reg
							NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
							ilist.push_back (aux);
							i_opr_addr = op2_addr;
						}
						else
						{
							// Order doesn't matter
							// XXX op2, [op1]
							// MOV [r], op2
							i_dest_addr = op2_addr;
							i_opr_addr = op1_addr;
						}
					}
				}
				else
				{
					// MOV r, [op1]
					// XXX r, op2
					i_dest_addr = r_addr;
					NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
					ilist.push_back (aux);
					i_opr_addr = op2_addr;
				}
			}
		}
		else
		{
			if (op2_addr->isMemory ())
			{
				// Just OP2 in memory
				if (r_addr->isMemory())
				{
					// MOV EAX, op1
					// XXX EAX, [op2]
					// MOV [r], EAX
					i_dest_addr = new RegisterNasmAddress (REG_EAX);
					NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
					ilist.push_back (aux);
					i_opr_addr = op2_addr;
				}
				else
				{
					// MOV r, op1
					// XXX r, [op2]
					i_dest_addr = r_addr;
					NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
					ilist.push_back (aux);
					i_opr_addr = op2_addr;
				}
			}
			else
			{
				// Neither in memory
				if (r_addr->isMemory())
				{
					if (op1_addr->getAddressType () == ADDR_REGISTER)
					{
						// XXX op1, op2
						// MOV [r], op1
						i_dest_addr = op1_addr;
						i_opr_addr = op2_addr;
					}
					else if (op2_addr->getAddressType () == ADDR_REGISTER)
					{
						if (IL_MAINTAIN_OPERAND_ORDER (op_type))
						{
							RegisterNasmAddress *reg = (RegisterNasmAddress *) op2_addr;
							if (reg->getRegister () == REG_EAX)
							{
								i_dest_addr = new RegisterNasmAddress (REG_EBX);
							}
							else
							{
								i_dest_addr = new RegisterNasmAddress (REG_EAX);
							}

							// MOV aux_reg, op1
							// XXX aux_reg, op2
							// MOV [r], aux_reg
							NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
							ilist.push_back (aux);
							i_opr_addr = op2_addr;
						}
						else
						{
							// Order doens't matter
							// XXX op2, op1
							// MOV [r], op2
							i_dest_addr = op2_addr;
							i_opr_addr = op1_addr;
						}
					}
					else
					{
						// MOV EAX, op1
						// XXX EAX, op2
						// MOV [r], EAX
						i_dest_addr = new RegisterNasmAddress (REG_EAX);
						NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
						ilist.push_back (aux);
						i_opr_addr = op2_addr;
					}
				}
				else
				{
					bool use_aux = false;
					if (op2_addr->getAddressType () == ADDR_REGISTER)
					{
						RegisterNasmAddress *reg = (RegisterNasmAddress *) r_addr;
						RegisterNasmAddress *reg2 = (RegisterNasmAddress *) op2_addr;
						if (reg2->getRegister () == reg->getRegister ())
						{
							// "MOV r, op1" would overwrite op2
							use_aux = true;
							if (reg->getRegister () == REG_EAX)
							{
								i_dest_addr = new RegisterNasmAddress (REG_EBX);
							}
							else
							{
								i_dest_addr = new RegisterNasmAddress (REG_EAX);
							}
						}
					}

					if (use_aux)
					{
						// MOV aux_reg, op1
						// XXX aux_reg, op2
						// MOV r, aux_reg
						NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
						ilist.push_back (aux);
						i_opr_addr = op2_addr;
					}
					else
					{
						// MOV r, op1
						// XXX r, op2
						i_dest_addr = r_addr;
						NasmInstruction *aux = new MovNasmInstruction (i_dest_addr, op1_addr);
						ilist.push_back (aux);
						i_opr_addr = op2_addr;
					}
				}
			}
		}
		assert (i_dest_addr != nullptr);
		assert (i_dest_addr->getAddressType () == ADDR_REGISTER);
		assert (i_opr_addr != nullptr);

		// Add arithmetic instruction
		switch (op_type)
		{
		case ILOP_ADD:
			inst = new AddNasmInstruction (i_dest_addr, i_opr_addr);
			break;

		case ILOP_SUB:
			inst = new SubNasmInstruction (i_dest_addr, i_opr_addr);
			break;

		case ILOP_MUL:
			inst = new AddNasmInstruction (i_dest_addr, i_opr_addr);
			break;

		case ILOP_DIV:
		case ILOP_MOD:
			{
				//
				// IMUL is defined as EAX = EDX:EAX / operand
				//

				// Prepare EAX
				RegisterNasmAddress *dest_reg = (RegisterNasmAddress *) i_dest_addr;
				if (dest_reg->getRegister () != REG_EAX)
				{
					MovNasmInstruction *mov;

					// Check operand
					if (i_opr_addr->getAddressType () == ADDR_REGISTER)
					{
						RegisterNasmAddress *reg_opr = (RegisterNasmAddress *) i_opr_addr;
						if (reg_opr->getRegister () == REG_EAX)
						{
							NasmRegister auxr =
								(dest_reg->getRegister () == REG_EBX ? REG_ECX : REG_EBX);

							// Move operand to auxiliary register
							mov = new MovNasmInstruction (
										new RegisterNasmAddress (auxr),
										i_opr_addr
									);
							ilist.push_back (mov);
							i_opr_addr = new RegisterNasmAddress (auxr);
						}
					}

					// Move to EAX
					mov = new MovNasmInstruction (
								new RegisterNasmAddress (REG_EAX),
								i_dest_addr
							);
					ilist.push_back (mov);

					if (op_type == ILOP_DIV)
					{
						// Division, we take result from EAX
						i_dest_addr = new RegisterNasmAddress (REG_EAX);
					}
					else
					{
						// Modulo, we take remainder from EDX
						i_dest_addr = new RegisterNasmAddress (REG_EDX);
					}
				}

				// Move operand if it's in EDX
				if ((i_opr_addr->getAddressType () == ADDR_IMMEDIATE)
					|| (i_opr_addr->getAddressType () == ADDR_REGISTER
						&& ((RegisterNasmAddress *)i_opr_addr)->getRegister () == REG_EDX))
					{
						// Move operand to EBX
						MovNasmInstruction *mov = new MovNasmInstruction (
									new RegisterNasmAddress (REG_EBX),
									i_opr_addr
								);
						ilist.push_back (mov);
						i_opr_addr = new RegisterNasmAddress (REG_EBX);
					}

				// Clear EDX
				MovNasmInstruction *mov = new MovNasmInstruction (
							new RegisterNasmAddress (REG_EDX),
							new ImmediateNasmAddress ((unsigned int) 0)
						);
				ilist.push_back (mov);

				// Do operation
				inst = new IdivNasmInstruction (i_opr_addr);
			}
			break;

		default:
			Error::internalError ("[x86-nasm] invalid operator '" + IlOperatorAlias[op_type] + "' for integer arithmetic op");
			return ER_FAILED;
		}
		inst->setComment (instruction->toString ());
		ilist.push_back (inst);

		// Memory write
		if (i_dest_addr != r_addr)
		{
			MovNasmInstruction *mov = new MovNasmInstruction (r_addr, i_dest_addr);
			ilist.push_back (mov);
		}

		// All ok
		return NO_ERROR;
	}

	// All ok
	return NO_ERROR;
}

int X86NasmBackend::compileJumpInstruction (JumpIlInstruction *instruction, NasmInstructionList &ilist, NasmStackMap &stack)
{
	IlAddress *cond = instruction->getCondition ();
	LabelIlInstruction *target = instruction->getTarget();

	if (cond != nullptr)
	{
		NasmAddress *addr = NasmAddress::fromIl (cond, data_, bss_, stack);

		// If immediate, bring to register
		if (addr->getAddressType () == ADDR_IMMEDIATE)
		{
			MovNasmInstruction *mov = new MovNasmInstruction (
						addr,
						new RegisterNasmAddress (REG_EAX)
					);
			ilist.push_back (mov);
			addr = new RegisterNasmAddress (REG_EAX);
		}

		// Generate TEST instruction
		TestNasmInstruction *test = new TestNasmInstruction (
					addr,
					new ImmediateNasmAddress ((unsigned int) 0xFFFFFFFF)
				);
		ilist.push_back (test);

		// Generate jump
		NasmInstruction *jump = nullptr;
		if (instruction->negateCondition ())
		{
			jump = new JzNasmInstruction (target->getName ());
		}
		else
		{
			jump = new JnzNasmInstruction (target->getName ());
		}
		ilist.push_back (jump);
	}
	else
	{
		JmpNasmInstruction *jmp = new JmpNasmInstruction (target->getName ());
		ilist.push_back (jmp);
	}

	// All ok
	return NO_ERROR;
}

int X86NasmBackend::compileInstruction (IlInstruction *instruction, NasmInstructionList &ilist, NasmStackMap &stack)
{
	IlInstructionType itype = instruction->getInstructionType ();

	if (itype == ILI_ASSIGNMENT)
	{
		AssignmentIlInstruction *asi = (AssignmentIlInstruction *) instruction;
		return compileAssignmentInstruction (asi, ilist, stack);
	}
	else if (itype == ILI_LABEL)
	{
		LabelIlInstruction *li = (LabelIlInstruction *) instruction;
		LabelNasmInstruction *lni = new LabelNasmInstruction (li->getName ());
		ilist.push_back (lni);
	}
	else if (itype == ILI_JUMP)
	{
		JumpIlInstruction *jmp = (JumpIlInstruction *) instruction;
		return compileJumpInstruction (jmp, ilist, stack);
	}
	else if (itype == ILI_PARAM)
	{
		ParamIlInstruction *pi = (ParamIlInstruction *) instruction;
		NasmAddress *addr = NasmAddress::fromIl (pi->getParameter (), data_, bss_, stack);
		PushNasmInstruction *push = new PushNasmInstruction (addr);
		push->setComment (pi->toString ());
		ilist.push_back (push);
	}
	else if (itype == ILI_CALL)
	{
		CallIlInstruction *ci = (CallIlInstruction *) instruction;
		CallNasmInstruction *call = new CallNasmInstruction (ci->getFunction ());
		ilist.push_back (call);
	}
	else
	{
		Error::internalError ("[x86-nasm] unknown intermediate language instruction");
		return ER_FAILED;
	}

	// All ok
	return NO_ERROR;
}

int X86NasmBackend::compileBlock (IlBlock *block, NasmInstructionList &ilist)
{
	NasmInstruction *ins;

	// Stack definitions
	NasmStackMap stack;

	// Compile instructions
	IlBlockIterator block_it = block->getIterator ();
	for (IlInstructionIterator it = std::get<0> (block_it);
		 it != std::get<1> (block_it); it ++)
	{
		IlInstruction *ins = (*it);
		if (compileInstruction (ins, ilist, stack) != NO_ERROR)
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
				new ImmediateNasmAddress (NasmStackDefinition::getStackSize (stack))
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
		std::string text = ((*it)->getInstructionType () != NI_LABEL ? INDENT : "") + (*it)->toString ();
		if ((*it)->getComment ().compare ("") != 0)
		{
			if (text.length () < 30)
			{
				text += std::string (30 - text.length (), ' ');
			}

			// We have a comment, print it
			text += " ; " + (*it)->getComment ();
		}

		stream << text << std::endl;
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

	// Write header
	assembly << "bits 32" << std::endl;
	assembly << INDENT << "global " << ENTRY_POINT << std::endl;
	assembly << INDENT << "extern printf" << std::endl << std::endl;

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
	std::string list_file = output_file + ".lst";

	// Call NASM
	std::string nasm_command =
		"nasm -g -f elf32 -l " + list_file + " -o " + object_file + " " + assembly_file;
	std::cout << "[x86-nasm] running assembler: " << nasm_command << std::endl;
	int nasm_rc = system (nasm_command.c_str ());
	if (nasm_rc != NO_ERROR)
	{
		Error::internalError ("[x86-nasm] nasm exited with error code "	+ std::to_string (nasm_rc));
		return ER_FAILED;
	}

	// Call linker
	std::string ld_command =
		"ld -m elf_i386 -lc -dynamic-linker /usr/lib32/ld-linux.so.2 -L/usr/lib32 -e " ENTRY_POINT " -o " + output_file + " " + object_file;
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

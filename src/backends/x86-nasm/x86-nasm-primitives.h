#ifndef X86_NASM_PRIMITIVES_H_
#define X86_NASM_PRIMITIVES_H_

#include "ilang/il-address.h"
#include <string>
#include <unordered_map>
#include <list>

//
// Data section definition
//
class NasmDataDefinition
{
private:
	std::string label_;
	unsigned int size_;
	char *data_;

	// Hidden constructor
	NasmDataDefinition () { }

public:
	NasmDataDefinition (std::string label, int size, char *data);
	NasmDataDefinition (std::string label, std::string str);

	// Compilable string
	std::string toString ();

	// Get the label
	std::string getLabel () const { return label_; }
};

typedef std::unordered_map<ConstantIlAddress *, NasmDataDefinition *> NasmDataMap;

//
// Bss section definition
//
class NasmBssDefinition
{
private:
	std::string label_;
	unsigned int size_;
	// Hidden constructor
	NasmBssDefinition ();
public:
	NasmBssDefinition (std::string label, unsigned int size) : label_ (label), size_ (size) { }

	// Compilable string
	std::string toString () const { return label_ + ": resb " + std::to_string (size_); }

	// Get the label
	std::string getLabel () const { return label_; }
};

typedef std::unordered_map<std::string, NasmBssDefinition *> NasmBssMap;

//
// Stack variable definition
//
class NasmStackDefinition;

typedef std::unordered_map<std::string, NasmStackDefinition> NasmStackMap;

class NasmStackDefinition
{
public:
	unsigned int size_;
	unsigned int offset_;

	NasmStackDefinition (unsigned int size, unsigned int offs) : size_ (size), offset_ (offs) { }

	static unsigned int getStackSize (NasmStackMap &map);
};

//
// Registers
//
enum NasmRegister
{
	REG_EAX,
	REG_EBX,
	REG_ECX,
	REG_EDX,

	REG_ESP,
	REG_EBP,

	REG_ST0,		// x87 floating point registers
	REG_ST1
};

static std::string NasmRegisterAlias[] = { "eax", "ebx", "ecx", "edx", "esp", "ebp", "st0", "st1" };

//
// Data locations
//
enum NasmAddressType
{
	ADDR_IMMEDIATE,
	ADDR_REGISTER,
	ADDR_MEMORY_DIRECT,
	ADDR_MEMORY_BASED
};

class NasmAddress
{
protected:
	// Hidden constructor
	NasmAddress () { }
public:
	virtual ~NasmAddress () { }

	// Compilable string representation
	virtual std::string toString () = 0;

	// Type
	virtual NasmAddressType getAddressType () const = 0;

	// Is memory location?
	virtual bool isMemory () const = 0;

	// Translate an IL address to a NASM address
	static NasmAddress *fromIl (IlAddress *iladdr, NasmDataMap &data, NasmBssMap &bss, NasmStackMap &stack);
};

class ImmediateNasmAddress : public NasmAddress
{
private:
	unsigned int data_;
	// Hidden constructor
	ImmediateNasmAddress ();

public:
	ImmediateNasmAddress (unsigned int data) : data_ (data) { };
	ImmediateNasmAddress (int data);
	ImmediateNasmAddress (float data);

	std::string toString ();
	NasmAddressType getAddressType () const { return ADDR_IMMEDIATE; }
	bool isMemory () const { return false; }

	unsigned int getData () { return data_; }
};

class RegisterNasmAddress : public NasmAddress
{
private:
	NasmRegister reg_;
	// Hidden constructor
	RegisterNasmAddress () { };
public:
	RegisterNasmAddress (NasmRegister reg) : reg_ (reg) { }

	std::string toString () { return NasmRegisterAlias[reg_]; }
	NasmAddressType getAddressType () const { return ADDR_REGISTER; }
	bool isMemory () const { return false; }

	NasmRegister getRegister () const { return reg_; }
};

class MemoryDirectNasmAddress : public NasmAddress
{
private:
	// Only access by label is allowed
	std::string label_;
	// Hidden constructor
	MemoryDirectNasmAddress () { };
public:
	MemoryDirectNasmAddress (std::string label) : label_ (label) { };

	std::string toString () { return "[" + label_ + "]"; }
	NasmAddressType getAddressType () const { return ADDR_MEMORY_DIRECT; }
	bool isMemory () const { return true; }
};

class MemoryBasedNasmAddress : public NasmAddress
{
private:
	// Register
	NasmRegister reg_;
	// Offset from register
	unsigned int offset_;
	// Hidden constructor
	MemoryBasedNasmAddress () { }
public:
	MemoryBasedNasmAddress (NasmRegister reg, unsigned int offset) : reg_ (reg), offset_ (offset) { }

	std::string toString () { return "[" + NasmRegisterAlias[reg_] + (offset_ >= 0 ? "+" : "") + std::to_string (offset_) + "]"; }
	NasmAddressType getAddressType () const { return ADDR_MEMORY_BASED; }
	bool isMemory () const { return true; }
};

//
// Instruction types
//
enum NasmInstructionType
{
	NI_LABEL,			// Just a label, it does nothing

	NI_INT,
	NI_MOV,

	NI_ADD,
	NI_SUB,
	NI_IMUL,
	NI_IDIV,

	NI_FADD,
	NI_FSUB,
	NI_FMUL,
	NI_FDIV,

	NI_FLD,
	NI_FSTP,

	NI_PUSH,
	NI_POP,

	NI_TEST,
	NI_JMP,
	NI_JZ,
	NI_JNZ
};

//
// Instructions
//
class NasmInstruction
{
protected:
	// Comment that will be displayed before instruction
	std::string comment_;
	// Hidden constructor
	NasmInstruction () : comment_ ("") { }
public:
	virtual ~NasmInstruction () { };

	virtual std::string toString () = 0;
	virtual NasmInstructionType getInstructionType () const = 0;

	std::string getComment () const { return comment_; }
	void setComment (std::string comm) { comment_ = comm; }
};

class LabelNasmInstruction : public NasmInstruction
{
protected:
	// Comment that will be displayed before instruction
	std::string label_;
	// Hidden constructor
	LabelNasmInstruction () : label_ ("") { }
public:
	LabelNasmInstruction (std::string label) : label_ (label) { }

	std::string toString () { return label_ + ":"; }
	NasmInstructionType getInstructionType () const { return NI_LABEL; }
};

typedef std::list<NasmInstruction *> NasmInstructionList;

class IntNasmInstruction : public NasmInstruction
{
private:
	unsigned int interrupt_;
	// Hidden constructor
	IntNasmInstruction () { }

public:
	IntNasmInstruction (unsigned int interrupt) : interrupt_ (interrupt) { }

	std::string toString () { return "int  " + std::to_string (interrupt_); }
	NasmInstructionType getInstructionType () const { return NI_INT; }
};

class MovNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *dest_;
	NasmAddress *src_;
	// Hidden constructor
	MovNasmInstruction () { };
public:
	MovNasmInstruction (NasmAddress *dest, NasmAddress *src) : dest_ (dest), src_ (src) { };
	~MovNasmInstruction () { delete dest_; delete src_; }

	std::string toString () { return "mov  " + dest_->toString () + ", " + src_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_MOV; }
};

class AddNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *dest_;
	NasmAddress *opr_;
	// Hidden constructor
	AddNasmInstruction () { };

public:
	AddNasmInstruction (NasmAddress *dest, NasmAddress *opr) : dest_ (dest), opr_ (opr) { };
	~AddNasmInstruction () { delete dest_; delete opr_; }

	std::string toString () { return "add  " + dest_->toString () + ", " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_ADD; }
};

class SubNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *dest_;
	NasmAddress *opr_;
	// Hidden constructor
	SubNasmInstruction () { };

public:
	SubNasmInstruction (NasmAddress *dest, NasmAddress *opr) : dest_ (dest), opr_ (opr) { };
	~SubNasmInstruction () { delete dest_; delete opr_; }

	std::string toString () { return "sub  " + dest_->toString () + ", " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_SUB; }
};

class ImulNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *dest_;
	NasmAddress *opr_;
	// Hidden constructor
	ImulNasmInstruction () { };

public:
	ImulNasmInstruction (NasmAddress *dest, NasmAddress *opr) : dest_ (dest), opr_ (opr) { };
	~ImulNasmInstruction () { delete dest_; delete opr_; }

	std::string toString () { return "imul " + dest_->toString () + ", " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_IMUL; }
};

class IdivNasmInstruction : public NasmInstruction
{	// EAX = EDX:EAX / opr_
private:
	NasmAddress *opr_;
	// Hidden constructor
	IdivNasmInstruction () { };

public:
	IdivNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~IdivNasmInstruction () { delete opr_; }

	std::string toString () { return "idiv dword " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_IDIV; }
};

class FaddNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *opr_;
	// Hidden constructor
	FaddNasmInstruction () { };

public:
	FaddNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~FaddNasmInstruction () { delete opr_; }

	std::string toString () { return "fadd dword " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_FADD; }
};

class FsubNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *opr_;
	// Hidden constructor
	FsubNasmInstruction () { };

public:
	FsubNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~FsubNasmInstruction () { delete opr_; }

	std::string toString () { return "fsub dword " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_FSUB; }
};

class FmulNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *opr_;
	// Hidden constructor
	FmulNasmInstruction () { };

public:
	FmulNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~FmulNasmInstruction () { delete opr_; }

	std::string toString () { return "fmul dword " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_FMUL; }
};

class FdivNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *opr_;
	// Hidden constructor
	FdivNasmInstruction () { };

public:
	FdivNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~FdivNasmInstruction () { delete opr_; }

	std::string toString () { return "fdiv dword " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_FDIV; }
};

class FldNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *opr_;
	// Hidden constructor
	FldNasmInstruction () { };

public:
	FldNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~FldNasmInstruction () { delete opr_; }

	std::string toString () { return "fld  dword " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_FLD; }
};

class FstpNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *opr_;
	// Hidden constructor
	FstpNasmInstruction () { };

public:
	FstpNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~FstpNasmInstruction () { delete opr_; }

	std::string toString () { return "fstp dword " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_FSTP; }
};

class PushNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *opr_;
	// Hidden constructor
	PushNasmInstruction () { };

public:
	PushNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~PushNasmInstruction () { delete opr_; }

	std::string toString () { return "push " + opr_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_PUSH; }
};

class PopNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *opr_;
	// Hidden constructor
	PopNasmInstruction () { }
public:
	PopNasmInstruction (NasmAddress *opr) : opr_ (opr) { };
	~PopNasmInstruction () { if (opr_ != nullptr) delete opr_; }

	std::string toString () { return "pop  " + (opr_ != nullptr ? opr_->toString () : ""); }
	NasmInstructionType getInstructionType () const { return NI_POP; }
};

class TestNasmInstruction : public NasmInstruction
{
private:
	NasmAddress *op1_;
	NasmAddress *op2_;
	// Hidden constructor
	TestNasmInstruction () { };
public:
	TestNasmInstruction (NasmAddress *op1, NasmAddress *op2) : op1_ (op1), op2_ (op2) { };
	~TestNasmInstruction () { delete op1_; delete op2_; }

	std::string toString () { return "test " + op1_->toString () + ", " + op2_->toString (); }
	NasmInstructionType getInstructionType () const { return NI_TEST; }
};

class JmpNasmInstruction : public NasmInstruction
{
private:
	std::string target_;
	// Hidden constructor
	JmpNasmInstruction () { }

public:
	JmpNasmInstruction (std::string target) : target_ (target) { };

	std::string toString () { return "jmp  " + target_; }
	NasmInstructionType getInstructionType () const { return NI_JMP; }
};

class JzNasmInstruction : public NasmInstruction
{
private:
	std::string target_;
	// Hidden constructor
	JzNasmInstruction () { }

public:
	JzNasmInstruction (std::string target) : target_ (target) { };

	std::string toString () { return "jz   " + target_; }
	NasmInstructionType getInstructionType () const { return NI_JZ; }
};

class JnzNasmInstruction : public NasmInstruction
{
private:
	std::string target_;
	// Hidden constructor
	JnzNasmInstruction () { }

public:
	JnzNasmInstruction (std::string target) : target_ (target) { };

	std::string toString () { return "jnz  " + target_; }
	NasmInstructionType getInstructionType () const { return NI_JNZ; }
};

#endif

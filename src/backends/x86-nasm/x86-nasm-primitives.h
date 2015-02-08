#ifndef X86_NASM_PRIMITIVES_H_
#define X86_NASM_PRIMITIVES_H_

#include "ilang/il-address.h"
#include <string>
#include <unordered_map>

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
// Registers
//
enum NasmRegister
{
	REG_EAX,
	REG_EBX,
	REG_ECX,
	REG_EDX,

	REG_ESP,
	REG_EBP
};

static std::string NasmRegisterAlias[] = { "eax", "ebx", "ecx", "edx", "esp", "ebp" };

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
	virtual NasmAddressType getType () const = 0;

	// Translate an IL address to a NASM address
	static NasmAddress *fromIl (IlAddress *iladdr, NasmDataMap &data, NasmBssMap &bss);
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

	std::string toString () { return std::to_string (data_); }
	NasmAddressType getType () const { return ADDR_IMMEDIATE; }

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
	NasmAddressType getType () const { return ADDR_REGISTER; }
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
	NasmAddressType getType () const { return ADDR_MEMORY_DIRECT; }
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

	std::string toString () { return "[" + NasmRegisterAlias[reg_] + (offset_ > 0 ? "+" : "") + std::to_string (offset_); }
	NasmAddressType getType () const { return ADDR_MEMORY_BASED; }
};

//
// Instructions
//
class NasmInstruction
{
private:
	// Hidden constructor
	NasmInstruction () { }
public:
};

#endif

#include "x86-nasm-primitives.h"
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "error/error.h"

NasmDataDefinition::NasmDataDefinition (std::string label, int size, char *data)
{
	data_ = (char *) malloc (size);
	memcpy (data_, data, size);
	size_ = size;
	label_ = label;
}

NasmDataDefinition::NasmDataDefinition (std::string label, std::string str)
{
	int slen = str.length ();
	size_ = 1 + (slen > 255 ? 255 : slen);
	data_ = (char *) malloc (size_);
	memcpy (data_, str.c_str (), size_ - 1);
	data_[size_ - 1] = 0; // Terminator

	label_ = label;
}

std::string NasmDataDefinition::toString ()
{
	std::stringstream hx;
	hx << std::hex;

	for (int i = 0; i < size_; i ++)
	{
		hx << "0x" << (unsigned int) data_[i] << (i < size_-1 ? "," : "");
	}

	return label_ + " db " + hx.str();
}

NasmAddress *NasmAddress::fromIl (IlAddress *iladdr, NasmDataMap &data, NasmBssMap &bss, unsigned int &stack_offset)
{
	IlAddressType atype = iladdr->getAddressType ();
	NasmAddress *naddr = nullptr;

	if (atype == ILA_VARIABLE)
	{
		NasmBssDefinition *bd = nullptr;
		VariableIlAddress *va = (VariableIlAddress *) iladdr;
		std::string name = va->getSymbol ()->getName ();

		NasmBssMap::iterator fret = bss.find (name);
		if (fret != bss.end ())
		{
			// Found in bss, no need to reallocate
			return new MemoryDirectNasmAddress (name);
		}

		// New bss entry
		if (va->getType () == BT_INT || va->getType () == BT_FLOAT)
		{
			bd = new NasmBssDefinition (va->getSymbol ()->getName (), 4);
		}
		else if (va->getType () == BT_STRING)
		{
			bd = new NasmBssDefinition (va->getSymbol ()->getName (), 256);
		}
		else
		{
			Error::internalError ("[x86-nasm] unknown variable type");
			return nullptr;
		}

		// Add to bss
		bss.insert ( { name, bd } );
		return new MemoryDirectNasmAddress (name);
	}
	else if (atype == ILA_CONSTANT)
	{
		ConstantIlAddress *ca = (ConstantIlAddress *) iladdr;
		if (ca->getType () == BT_INT)
		{
			naddr = new ImmediateNasmAddress (ca->getInt ());
		}
		else if (ca->getType () == BT_FLOAT)
		{
			naddr = new ImmediateNasmAddress (ca->getFloat ());
		}
		else if (ca->getType() == BT_STRING)
		{
			NasmDataDefinition *ddef = nullptr;
			NasmDataMap::iterator fret = data.find (ca);

			if (fret != data.end ())
			{
				ddef = (*fret).second;
			}
			else
			{
				std::string label = "str_" + std::to_string ((unsigned long int) iladdr);
				ddef = new NasmDataDefinition (label, ca->getString ());
				data.insert ( { ca, ddef } );
			}

			naddr = new MemoryDirectNasmAddress (ddef->getLabel ());
		}
		else
		{
			Error::internalError ("[x86-nasm] unknown constant type");
			return nullptr;
		}
	}
	else if (atype == ILA_TEMPORARY)
	{
		TemporaryIlAddress *ta = (TemporaryIlAddress *) iladdr;
		if (ta->getType () == BT_INT || ta->getType () == BT_FLOAT)
		{
			stack_offset += 4;
			naddr = new MemoryBasedNasmAddress (REG_EBP, stack_offset);
		}
		else if (ta->getType () == BT_STRING)
		{
			stack_offset += 256;
			naddr = new MemoryBasedNasmAddress (REG_EBP, stack_offset);
		}
		else
		{
			Error::internalError ("[x86-nasm] unknown temporary type");
			return nullptr;
		}
	}
	else
	{
		Error::internalError ("[x86-nasm] unknown address type");
		return nullptr;
	}

	// All ok
	return naddr;
}

ImmediateNasmAddress::ImmediateNasmAddress (float data)
{
	unsigned int *addr = (unsigned int *)&data;
	data_ = *addr;
}

ImmediateNasmAddress::ImmediateNasmAddress (int data)
{
	unsigned int *addr = (unsigned int *)&data;
	data_ = *addr;
}

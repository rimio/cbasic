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

unsigned int NasmStackDefinition::getStackSize (NasmStackMap &map)
{
	unsigned int max_size = 0;

	for (NasmStackMap::iterator it = map.begin (); it != map.end (); it ++)
	{
		int size = (*it).second.offset_ + (*it).second.size_;
		max_size = (size > max_size ? size : max_size);
	}

	return max_size;
}

NasmAddress *NasmAddress::fromIl (IlAddress *iladdr, NasmDataMap &data, NasmBssMap &bss, NasmStackMap &stack)
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

		// If it's already allocated, get it from there
		NasmStackMap::iterator fret = stack.find (ta->getName ());
		if (fret != stack.end ())
		{
			// Found it!
			naddr = new MemoryBasedNasmAddress (REG_EBP, (*fret).second.offset_);
		}
		else
		{
			// Not found, add it
			unsigned int ssize = NasmStackDefinition::getStackSize (stack);
			unsigned int offset = 0;

			if (ta->getType () == BT_INT || ta->getType () == BT_FLOAT)
			{
				offset = ssize + 4;
				stack.insert ( { ta->getName (), NasmStackDefinition (offset, 4) } );
			}
			else if (ta->getType () == BT_STRING)
			{
				offset = ssize + 256;
				stack.insert ( { ta->getName (), NasmStackDefinition (offset, 256) } );
			}
			else
			{
				Error::internalError ("[x86-nasm] unknown temporary type");
				return nullptr;
			}

			// Get address
			naddr = new MemoryBasedNasmAddress (REG_EBP, offset);
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

std::string ImmediateNasmAddress::toString ()
{
	std::stringstream hx;
	hx << std::hex << data_;

	return "dword 0x" + hx.str ();
}

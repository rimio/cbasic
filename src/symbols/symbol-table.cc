#include "symbol-table.h"
#include <iostream>
#include <iomanip>
#include "error/error.h"

std::unordered_map<std::string, Symbol *> SymbolTable::table_;

std::string symbol_type_to_string (SymbolType st)
{
	switch (st)
	{
	case SY_VARIABLE:
		return "VARIABLE";

	default:
		return "unknown";
	}
}

void SymbolTable::clear ()
{
	table_.clear ();
}

bool SymbolTable::addSymbol (Symbol *sym)
{
	return table_.insert ( { sym->getName (), sym } ).second;
}

Symbol *SymbolTable::getSymbol (std::string name)
{
	std::unordered_map<std::string, Symbol *>::iterator ret = table_.find (name);
	if (ret != table_.end ())
	{
		return ret->second;
	}
	else
	{
		// Not found
		return nullptr;
	}
}

void SymbolTable::debugPrint ()
{
	std::cout << " Symbol Type           Symbol Name" << std::endl;
	std::cout << "=======================================================" << std::endl;

	for (std::unordered_map<std::string, Symbol *>::iterator it = table_.begin ();
		 it != table_.end (); it ++)
	{
		Symbol *sym = it->second;
		VariableSymbol *vs;

		switch (sym->getSymbolType ())
		{
		case SY_VARIABLE:
			vs = (VariableSymbol *) sym;
			std::cout << std::setw (22) << std::left << std::setfill('.');
			std::cout << " VARIABLE (" + basic_type_to_string (vs->getType ()) + ") ";
			std::cout << std::setw (0) << " " + sym->getName() << std::endl;
			break;

		default:
			Error::internalError ("unknown symbol type for symbol '" + sym->getName () + "'");
			break;
		}
	}
}

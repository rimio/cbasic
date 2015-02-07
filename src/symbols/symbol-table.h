#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <string>
#include <unordered_map>
#include <tuple>
#include "symbols/basic-types.h"

//
// Symbol types
//
enum SymbolType
{
	SY_VARIABLE = 0
};

//
// Symbol aliases
//
static std::string SymbolTypeAlias[] = { "VARIABLE" };

//
// Symbol base class
//
class Symbol
{
protected:
	// Symbol's name
	std::string name_;
	std::string scope_;

	// Hidden constructor
	Symbol () { };
	Symbol (std::string name, std::string scope) : name_ (name), scope_ (scope) { };

public:
	virtual ~Symbol () { };

	// Getter for name and scope
	std::string getName () const { return name_; }
	std::string getScope () const { return scope_; }

	// Get type of symbol
	virtual SymbolType getSymbolType () const = 0;
};

//
// Symbol of type variable
//
class VariableSymbol : public Symbol
{
private:
	// Hidden constructor
	VariableSymbol ();

	// Variable type
	BasicType type_;

public:
	VariableSymbol (std::string name, std::string scope, BasicType type) : Symbol (name, scope), type_ (type){ }

	BasicType getType () const { return type_; }
	SymbolType getSymbolType () const { return SY_VARIABLE; }
};

//
// Symbol table static class
//
class SymbolTable
{
private:
	// The actual hash
	static std::unordered_map<std::string, Symbol *> table_;

public:
	// Clear symbols table
	static void clear ();

	// Add and get a symbol from the table
	static bool addSymbol (Symbol *sym);
	static Symbol *getSymbol (std::string name);

	// Get iteration essentials for table
	static std::tuple< std::unordered_map<std::string, Symbol *>::iterator,
					   std::unordered_map<std::string, Symbol *>::iterator > getIterator ();

	// Print symbol tree
	static void debugPrint ();
};

#endif

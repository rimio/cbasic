#ifndef IL_ADDRESS_H_
#define IL_ADDRESS_H_

#include "symbols/basic-types.h"
#include <string>
#include "symbols/symbol-table.h"

//
// Address types
//
enum IlAddressType
{
	ILA_VARIABLE,
	ILA_CONSTANT,
	ILA_TEMPORARY
};

//
// Base class for intermediate language address
//
class IlAddress
{
private:
	BasicType type_;

	// REALLY hidden constructor
	IlAddress () { };

protected:
	// Hidden constructor
	IlAddress (BasicType ty) : type_ (ty) { };

public:
	virtual ~IlAddress () { };

	virtual std::string toString () = 0;
	virtual IlAddressType getAddressType () const = 0;

	// Get the type
	virtual BasicType getType () const { return type_; }
};

//
// Variable symbol
//
class VariableIlAddress : public IlAddress
{
private:
	// Hidden default constructor
	VariableIlAddress () : IlAddress (BT_UNKNOWN) { }	// We override getType ()

protected:
	VariableSymbol *var_;

public:
	VariableIlAddress (VariableSymbol *sym) : IlAddress (BT_UNKNOWN), var_ (sym) { }		// We override getType ()

	std::string toString ();
	IlAddressType getAddressType () const { return ILA_VARIABLE; }
	VariableSymbol *getSymbol () const { return var_; }

	// Override type getter
	BasicType getType () const { return var_->getType (); }
};

//
// Constant
//
class ConstantIlAddress : public IlAddress
{
private:
	// Hidden default constructor
	ConstantIlAddress () : IlAddress (BT_UNKNOWN) { }

protected:
	int ival_;
	float fval_;
	std::string sval_;

public:
	ConstantIlAddress (int val);
	ConstantIlAddress (float val);
	ConstantIlAddress (std::string val);

	std::string toString ();
	IlAddressType getAddressType () const { return ILA_CONSTANT; }
	int getInt () const { return ival_; }
	float getFloat () const { return fval_; }
	std::string getString () const { return sval_; }
};

//
// Temporary
//
class TemporaryIlAddress : public IlAddress
{
private:
	// Hidden default constructor
	TemporaryIlAddress () : IlAddress (BT_UNKNOWN) { }

protected:
	static int next_id_;
	std::string name_;

public:
	TemporaryIlAddress (BasicType type);

	std::string toString ();
	IlAddressType getAddressType () const { return ILA_TEMPORARY; }

	std::string getName () const { return name_; }
};

#endif

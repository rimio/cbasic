#include "il-address.h"
#include <cassert>
#include "error/error.h"

int TemporaryIlAddress::next_id_ = 0;

std::string VariableIlAddress::toString ()
{
	return var_->getName ();
}

ConstantIlAddress::ConstantIlAddress (int val) : IlAddress (BT_INT)
{
	ival_ = val;
}

ConstantIlAddress::ConstantIlAddress (float val) : IlAddress (BT_FLOAT)
{
	fval_ = val;
}

ConstantIlAddress::ConstantIlAddress (std::string val) : IlAddress (BT_STRING)
{
	sval_ = val;
}

std::string ConstantIlAddress::toString ()
{
	switch (getType ())
	{
	case BT_INT:
		return std::to_string (ival_);

	case BT_FLOAT:
		return std::to_string (fval_);

	case BT_STRING:
		return sval_;

	default:
		assert (false);
		return "???";
	}
}

TemporaryIlAddress::TemporaryIlAddress (BasicType type) : IlAddress (type)
{
	name_ = "t" + std::to_string (next_id_);
	next_id_ ++;
}

std::string TemporaryIlAddress::toString ()
{
	return name_;
}

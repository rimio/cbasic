#ifndef EXPRESSION_NODE_H_
#define EXPRESSION_NODE_H_

//
// Generic typed parser node
//
class ExpressionNode : public ParserNode
{
public:
	virtual BasicType getType () const = 0;
	virtual void setType (BasicType type) = 0;
	virtual int inferType () = 0;
};

#endif

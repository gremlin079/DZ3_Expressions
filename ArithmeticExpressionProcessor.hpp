#ifndef ARITHMETICEXPRESSIONPROCESSOR_HPP
#define ARITHMETICEXPRESSIONPROCESSOR_HPP
#include "Expression.hpp"

class ArithmeticExpressionProcessor
{
	Expression& expr;

public:
	ArithmeticExpressionProcessor(Expression&);
	void convertToStandartForm();
	void addConst(int);
};
#endif
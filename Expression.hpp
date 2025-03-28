#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include <string>


class Expression
{
protected:
	std::string rawExpression;
	std::string normolizedExpression;
	std::string type;

public:
	Expression(std::string&, std::string&);
	std::string getRawExpression();
	std::string getNormolizedExpression();
	std::string getType();
	void setNormolizedExpression(std::string&);
	void normalaize();
	void addConst(int);
};

#endif
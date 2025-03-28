#ifndef EXPRESSIONEDITOR_HPP
#define EXPRESSIONEDITOR_HPP
#include <string>
#include <vector>
#include "Expression.hpp"

class ExpressionEditor {
private:
	std::string filename;
	Expression expr;
	std::vector<int> constants = { 0 };

public:
	ExpressionEditor(const std::string&);
	void loadFromFile();
	void showMenu();
	void handleUserChoice(int);
	void displayExpression();
	void addConstant();
	void showResult();
};
#endif

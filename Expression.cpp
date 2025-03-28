#include "Expression.hpp"
#include <iostream>
#include <string>
#include "ArithmeticExpressionProcessor.hpp"
#include "LogicalExpressionProcessor.hpp"
//#include "" ��� ����������� ����


Expression::Expression(std::string& rawExpression, std::string& type)
{
	this->rawExpression = rawExpression;
	this->type = type;
}

std::string Expression::getRawExpression()
{
	return rawExpression;
}

std::string Expression::getNormolizedExpression()
{
	return normolizedExpression;
}

std::string Expression::getType()
{
	return type;
}

void Expression::setNormolizedExpression(std::string& normolizedExpression)
{
	this->normolizedExpression = normolizedExpression;
}

void Expression::normalaize()
{
	if (this->type == "��������������") {
		ArithmeticExpressionProcessor processor(*this);
		processor.convertToStandartForm();
	}
	else if (this->type == "����������")
	{
        // ����� ������� ��� ��������
        int pos = 0;
        // ������ ���������� ��������� �� ������ this->expression
        LogicalExpressionProcessor* processor = nullptr;
        try {
            processor = parseExpr(this->rawExpression);
        }
        catch (const std::exception& e) {
            std::cerr << "������ ��� �������� ����������� ���������: " << e.what() << std::endl;
            return;
        }

        // �������� ��� ����������, ������������ � ���������
        std::set<char> varSet;
        collectVariables(this->rawExpression, varSet);
        std::vector<char> variables(varSet.begin(), varSet.end());

        // ����������� ��������� (�������� ����� ���������)
        processor->normalizeExpression(variables);

        // ��������� ��������������� ��������� � ������ Expression
        this->normolizedExpression = processor->normalizedExpression;

        // ����������� ������
        delete processor;
	}
}

void Expression::addConst(int constanta)
{
	if (this->type == "��������������")
	{
		ArithmeticExpressionProcessor processor(*this);
		processor.addConst(constanta);
	}
}
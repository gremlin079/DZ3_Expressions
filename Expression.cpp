#include "Expression.hpp"
#include <iostream>
#include <string>
#include "ArithmeticExpressionProcessor.hpp"
#include "LogicalExpressionProcessor.hpp"
//#include "" для логического типа


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
	if (this->type == "Арифметическое") {
		ArithmeticExpressionProcessor processor(*this);
		processor.convertToStandartForm();
	}
	else if (this->type == "Логическое")
	{
        // Сброс позиции для парсинга
        int pos = 0;
        // Парсим логическое выражение из строки this->expression
        LogicalExpressionProcessor* processor = nullptr;
        try {
            processor = parseExpr(this->rawExpression);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка при парсинге логического выражения: " << e.what() << std::endl;
            return;
        }

        // Собираем все переменные, используемые в выражении
        std::set<char> varSet;
        collectVariables(this->rawExpression, varSet);
        std::vector<char> variables(varSet.begin(), varSet.end());

        // Нормализуем выражение (получаем сумму минтермов)
        processor->normalizeExpression(variables);

        // Сохраняем нормализованное выражение в объект Expression
        this->normolizedExpression = processor->normalizedExpression;

        // Освобождаем память
        delete processor;
	}
}

void Expression::addConst(int constanta)
{
	if (this->type == "Арифметическое")
	{
		ArithmeticExpressionProcessor processor(*this);
		processor.addConst(constanta);
	}
}
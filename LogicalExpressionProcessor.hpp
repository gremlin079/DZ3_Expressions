#ifndef LOGICAL_EXPRESSION_PROCESSOR_HPP
#define LOGICAL_EXPRESSION_PROCESSOR_HPP

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>

// Перечисление для типов узлов
enum NodeType { VARIABLE, AND, OR };

// Узел дерева выражения
class LogicalExpressionProcessor {
public:
    NodeType type; // Тип узла
    char name; // Имя переменной (для узлов переменной)
    LogicalExpressionProcessor* left; // Левый дочерний узел
    LogicalExpressionProcessor* right; // Правый дочерний узел
    std::string normalizedExpression; // Нормализованное выражение

    // Конструктор для узла переменной
    LogicalExpressionProcessor(char n);

    // Конструктор для узла операции
    LogicalExpressionProcessor(NodeType t, LogicalExpressionProcessor* l, LogicalExpressionProcessor* r);

    // Деструктор для удаления дочерних узлов
    ~LogicalExpressionProcessor();

    // Функция для вычисления значения узла
    bool eval(const std::map<char, bool>& vars) const;

    // Функция для нормализации выражения
    void normalizeExpression(const std::vector<char>& variables);
};

// Пропуск пробелов в строке
void skipSpaces(const std::string& s, size_t& pos);

// Функция для парсинга выражений
LogicalExpressionProcessor* parseExpr(const std::string& s, size_t& pos);

// Функция для парсинга фактора (переменная или подвыражение в скобках)
LogicalExpressionProcessor* parseFactor(const std::string& s, size_t& pos);

// Функция для парсигна терма (фактор с операциями И)
LogicalExpressionProcessor* parseTerm(const std::string& s, size_t& pos);

// Сбор все переменных
void collectVariables(const std::string& s, std::set<char>& vars);

// Формированиен минтрема
std::string buildMinterm(const std::vector<char>& variables, int mask);

#endif // LOGICAL_EXPRESSION_PROCESSOR_HPP
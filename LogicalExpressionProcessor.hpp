#ifndef LOGICAL_EXPRESSION_PROCESSOR_HPP
#define LOGICAL_EXPRESSION_PROCESSOR_HPP

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>

// ������������ ��� ����� �����
enum NodeType { VARIABLE, AND, OR };

// ���� ������ ���������
class LogicalExpressionProcessor {
public:
    NodeType type; // ��� ����
    char name; // ��� ���������� (��� ����� ����������)
    LogicalExpressionProcessor* left; // ����� �������� ����
    LogicalExpressionProcessor* right; // ������ �������� ����
    std::string normalizedExpression; // ��������������� ���������

    // ����������� ��� ���� ����������
    LogicalExpressionProcessor(char n);

    // ����������� ��� ���� ��������
    LogicalExpressionProcessor(NodeType t, LogicalExpressionProcessor* l, LogicalExpressionProcessor* r);

    // ���������� ��� �������� �������� �����
    ~LogicalExpressionProcessor();

    // ������� ��� ���������� �������� ����
    bool eval(const std::map<char, bool>& vars) const;

    // ������� ��� ������������ ���������
    void normalizeExpression(const std::vector<char>& variables);
};

// ������� �������� � ������
void skipSpaces(const std::string& s, size_t& pos);

// ������� ��� �������� ���������
LogicalExpressionProcessor* parseExpr(const std::string& s, size_t& pos);

// ������� ��� �������� ������� (���������� ��� ������������ � �������)
LogicalExpressionProcessor* parseFactor(const std::string& s, size_t& pos);

// ������� ��� �������� ����� (������ � ���������� �)
LogicalExpressionProcessor* parseTerm(const std::string& s, size_t& pos);

// ���� ��� ����������
void collectVariables(const std::string& s, std::set<char>& vars);

// ������������� ��������
std::string buildMinterm(const std::vector<char>& variables, int mask);

#endif // LOGICAL_EXPRESSION_PROCESSOR_HPP
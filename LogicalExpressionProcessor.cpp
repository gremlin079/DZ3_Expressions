#include "LogicalExpressionProcessor.hpp"

// ���������� ���������� ��� ������� ������� � ������
size_t pos = 0;

// ����������� ��� ���� ����������
LogicalExpressionProcessor::LogicalExpressionProcessor(char n) {
    type = VARIABLE;
    name = n;
    // � ���������� �� ����� ���� �����
    left = nullptr;
    right = nullptr;
}

// ����������� ��� ���� ��������
LogicalExpressionProcessor::LogicalExpressionProcessor(NodeType t, LogicalExpressionProcessor* l, LogicalExpressionProcessor* r) {
    type = t;
    left = l;
    right = r;
}

// ���������� ��� �������� �������� �����
LogicalExpressionProcessor::~LogicalExpressionProcessor() {
    delete left;
    delete right;
}

// ������� ��� ���������� �������� ����
bool LogicalExpressionProcessor::eval(const std::map<char, bool>& vars) const {
    if (type == VARIABLE) return vars.at(name);
    else if (type == AND) return left->eval(vars) && right->eval(vars);
    else if (type == OR) return left->eval(vars) || right->eval(vars);
    throw std::runtime_error("����������� ����!");
}

// ������� ��� ������������ ���������
void LogicalExpressionProcessor::normalizeExpression(const std::vector<char>& variables) {
    int n = variables.size();
    int total = 1 << n; // ��������� ����� 2^n
    std::vector<std::string> minterms; // �������� (������ �������� ����������, ����� ������ �� ������)

    for (int mask = 0; mask < total; mask++) {
        std::map<char, bool> varValues; // ����� ������� �������� ��� ������ �����������
        // ����������� ���������� ����� � ���� ��������������� ������ ����������
        for (int i = 0; i < n; i++) varValues[variables[i]] = (mask & (1 << i)) != 0;
        // ���� ��������� ������ - ��������� �������
        if (eval(varValues)) minterms.push_back(buildMinterm(variables, mask));
    }

    // ���� ��������� ��� - ���������� 0
    std::ostringstream oss;
    if (minterms.empty()) {
        oss << "0";
    }
    // �������� ��������
    else {
        for (size_t i = 0; i < minterms.size(); i++) {
            if (i > 0) oss << " | ";
            oss << minterms[i];
        }
    }
    normalizedExpression = oss.str();
}

// ������� �������� � ������
void skipSpaces(const std::string& s, size_t& pos) {
    while (pos < s.size() && isspace(s[pos])) pos++;
}

// ������� ������� (��������� ��� ���� � �������)
LogicalExpressionProcessor* parseFactor(const std::string& s, size_t& pos) {
    skipSpaces(s, pos);
    if (pos >= s.size()) throw std::runtime_error("������: ����������� ����� ���������");
    // ���� ��������� ������
    if (s[pos] == '(') {
        pos++; // ���������� ������
        LogicalExpressionProcessor* node = parseExpr(s, pos); // ������ �������� |
        skipSpaces(s, pos);
        if (pos >= s.size() || s[pos] != ')') throw std::runtime_error("������: ��������� ')'"); // ���� ������ �� �������
        pos++;
        return node; // ���������� �������������� ����
    }
    // ���� ��������� �����
    if (isalpha(s[pos])) return new LogicalExpressionProcessor(s[pos++]); // ��������� ����� � ���������� ���������
    throw std::runtime_error("������: �������� ������");
}

// ������� ����� (������ � ���������� &)
LogicalExpressionProcessor* parseTerm(const std::string& s, size_t& pos) {
    LogicalExpressionProcessor* node = parseFactor(s, pos); // ������ ����� ��� ��������� � �������
    skipSpaces(s, pos);
    // �������� �� ���� ���������� & (���� ��� ����������)
    while (pos < s.size() && s[pos] == '&') {
        pos++;
        // ������� ����� ���� and � ��������� ��� � ������� ������
        node = new LogicalExpressionProcessor(AND, node, parseFactor(s, pos));
        skipSpaces(s, pos);
    }
    return node;
}

// ������� ��������� (���� � ���������� ���)
LogicalExpressionProcessor* parseExpr(const std::string& s, size_t& pos) {
    LogicalExpressionProcessor* node = parseTerm(s, pos); // ������ ����� ��� ��������� � �������
    skipSpaces(s, pos);
    // �������� �� ���� ���������� | (���� ��� ����������)
    while (pos < s.size() && s[pos] == '|') {
        pos++;
        // ������� ����� ���� and � ��������� ��� � ������� ������
        node = new LogicalExpressionProcessor(OR, node, parseTerm(s, pos));
        skipSpaces(s, pos);
    }
    return node;
}

// ���� ���� ���������� � ���������
void collectVariables(const std::string& s, std::set<char>& vars) {
    for (char c : s) if (isalpha(c)) vars.insert(c);
}

// ������������ �������� � ���� "ABC" ��� "�A�BC"
std::string buildMinterm(const std::vector<char>& variables, int mask) {
    std::ostringstream oss;
    for (size_t i = 0; i < variables.size(); i++) {
        if (mask & (1 << i))
            oss << variables[i];
        else
            oss << "�" << variables[i];
    }
    return oss.str();
}
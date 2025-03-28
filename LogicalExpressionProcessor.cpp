#include "LogicalExpressionProcessor.hpp"

// Глобальная переменная для текущей позиции в строке
size_t pos = 0;

// Конструктор для узла переменной
LogicalExpressionProcessor::LogicalExpressionProcessor(char n) {
    type = VARIABLE;
    name = n;
    left = nullptr;
    right = nullptr;
}

// Конструктор для узла операции
LogicalExpressionProcessor::LogicalExpressionProcessor(NodeType t, LogicalExpressionProcessor* l, LogicalExpressionProcessor* r) {
    type = t;
    left = l;
    right = r;
}

// Деструктор для удаления дочерних узлов
LogicalExpressionProcessor::~LogicalExpressionProcessor() {
    delete left;
    delete right;
}

// Функция для вычисления значения узла
bool LogicalExpressionProcessor::eval(const std::map<char, bool>& vars) const {
    if (type == VARIABLE) return vars.at(name);
    else if (type == AND) return left->eval(vars) && right->eval(vars);
    else if (type == OR) return left->eval(vars) || right->eval(vars);
    throw std::runtime_error("Неизвестный узел!");
}

// Функция для нормализации выражения
void LogicalExpressionProcessor::normalizeExpression(const std::vector<char>& variables) {
    int n = variables.size();
    int total = 1 << n;
    std::vector<std::string> minterms;

    for (int mask = 0; mask < total; mask++) {
        std::map<char, bool> varValues;
        for (int i = 0; i < n; i++) varValues[variables[i]] = (mask & (1 << i)) != 0;
        if (eval(varValues)) minterms.push_back(buildMinterm(variables, mask));
    }

    std::ostringstream oss;
    if (minterms.empty()) {
        oss << "0";
    }
    else {
        for (size_t i = 0; i < minterms.size(); i++) {
            if (i > 0) oss << " | ";
            oss << minterms[i];
        }
    }
    normalizedExpression = oss.str();
}

// Пропуск пробелов в строке
void skipSpaces(const std::string& s) {
    while (pos < s.size() && isspace(s[pos])) pos++;
}

// Парсинг фактора (переменную или подвыражение в скобках)
LogicalExpressionProcessor* parseFactor(const std::string& s) {
    skipSpaces(s);
    if (pos >= s.size()) throw std::runtime_error("Ошибка: неожиданный конец выражения");
    if (s[pos] == '(') {
        pos++;
        LogicalExpressionProcessor* node = parseExpr(s);
        skipSpaces(s);
        if (pos >= s.size() || s[pos] != ')') throw std::runtime_error("Ошибка: ожидается ')'");
        pos++;
        return node;
    }
    if (isalpha(s[pos])) return new LogicalExpressionProcessor(s[pos++]);
    throw std::runtime_error("Ошибка: неверный символ");
}

// Парсинг трема (фактор с операциями И)
LogicalExpressionProcessor* parseTerm(const std::string& s) {
    LogicalExpressionProcessor* node = parseFactor(s);
    skipSpaces(s);
    while (pos < s.size() && s[pos] == '&') {
        pos++;
        node = new LogicalExpressionProcessor(AND, node, parseFactor(s));
        skipSpaces(s);
    }
    return node;
}

// Парсинг выражения (терм с операциями ИЛИ)
LogicalExpressionProcessor* parseExpr(const std::string& s) {
    LogicalExpressionProcessor* node = parseTerm(s);
    skipSpaces(s);
    while (pos < s.size() && s[pos] == '|') {
        pos++;
        node = new LogicalExpressionProcessor(OR, node, parseTerm(s));
        skipSpaces(s);
    }
    return node;
}

// Сбор всех переменных в выражение
void collectVariables(const std::string& s, std::set<char>& vars) {
    for (char c : s) if (isalpha(c)) vars.insert(c);
}

// Формирование минтрема в виде "ABC" или "!A!BC"
std::string buildMinterm(const std::vector<char>& variables, int mask) {
    std::ostringstream oss;
    for (size_t i = 0; i < variables.size(); i++) {
        if (mask & (1 << i))
            oss << variables[i];
        else
            oss << "¬" << variables[i];
    }
    return oss.str();
}
#include "LogicalExpressionProcessor.hpp"

// Глобальная переменная для текущей позиции в строке
size_t pos = 0;

// Конструктор для узла переменной
LogicalExpressionProcessor::LogicalExpressionProcessor(char n) {
    type = VARIABLE;
    name = n;
    // У переменной не может быть детей
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
    int total = 1 << n; // Получение числа 2^n
    std::vector<std::string> minterms; // Минтремы (наборы значений переменных, когда строка ТИ истина)

    for (int mask = 0; mask < total; mask++) {
        std::map<char, bool> varValues; // Карта булевых значений для каждой перемеенной
        // Поразрядная конъюнкция макси и бита соответсвующему номеру переменной
        for (int i = 0; i < n; i++) varValues[variables[i]] = (mask & (1 << i)) != 0;
        // Если выражение истино - формируем минтрем
        if (eval(varValues)) minterms.push_back(buildMinterm(variables, mask));
    }

    // Если минтремом нет - возвращаем 0
    std::ostringstream oss;
    if (minterms.empty()) {
        oss << "0";
    }
    // Собираем минтремы
    else {
        for (size_t i = 0; i < minterms.size(); i++) {
            if (i > 0) oss << " | ";
            oss << minterms[i];
        }
    }
    normalizedExpression = oss.str();
}

// Пропуск пробелов в строке
void skipSpaces(const std::string& s, size_t& pos) {
    while (pos < s.size() && isspace(s[pos])) pos++;
}

// Парсинг фактора (выражения или блок в скобках)
LogicalExpressionProcessor* parseFactor(const std::string& s, size_t& pos) {
    skipSpaces(s, pos);
    if (pos >= s.size()) throw std::runtime_error("Ошибка: неожиданный конец выражения");
    // Если встречаем скобку
    if (s[pos] == '(') {
        pos++; // Пропускаем скобку
        LogicalExpressionProcessor* node = parseExpr(s, pos); // Парсим опреатор |
        skipSpaces(s, pos);
        if (pos >= s.size() || s[pos] != ')') throw std::runtime_error("Ошибка: ожидается ')'"); // Если скобка не закрыта
        pos++;
        return node; // Возвращаем сформированный узел
    }
    // Если встретили букву
    if (isalpha(s[pos])) return new LogicalExpressionProcessor(s[pos++]); // Добавляем букву и перемещаем указатель
    throw std::runtime_error("Ошибка: неверный символ");
}

// Парсинг трема (фактор с операциями &)
LogicalExpressionProcessor* parseTerm(const std::string& s, size_t& pos) {
    LogicalExpressionProcessor* node = parseFactor(s, pos); // Парсим букву или выражение в скобках
    skipSpaces(s, pos);
    // Проходим по всем операторам & (если они существует)
    while (pos < s.size() && s[pos] == '&') {
        pos++;
        // Создаем новый узел and и связываем его с другими узлами
        node = new LogicalExpressionProcessor(AND, node, parseFactor(s, pos));
        skipSpaces(s, pos);
    }
    return node;
}

// Парсинг выражения (терм с операциями ИЛИ)
LogicalExpressionProcessor* parseExpr(const std::string& s, size_t& pos) {
    LogicalExpressionProcessor* node = parseTerm(s, pos); // Парсим букву или выражение в скобках
    skipSpaces(s, pos);
    // Проходим по всем операторам | (если они существует)
    while (pos < s.size() && s[pos] == '|') {
        pos++;
        // Создаем новый узел and и связываем его с другими узлами
        node = new LogicalExpressionProcessor(OR, node, parseTerm(s, pos));
        skipSpaces(s, pos);
    }
    return node;
}

// Сбор всех переменных в выражение
void collectVariables(const std::string& s, std::set<char>& vars) {
    for (char c : s) if (isalpha(c)) vars.insert(c);
}

// Формирование минтрема в виде "ABC" или "¬A¬BC"
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
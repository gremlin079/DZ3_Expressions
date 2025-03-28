#include "ExpressionEditor.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>


ExpressionEditor::ExpressionEditor(const std::string& filename) : filename(filename), expr(*(new std::string("ю")), *(new std::string("й"))) {}

void ExpressionEditor::loadFromFile() {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Ошибка: не удалось открыть файл!" << std::endl;
		exit(1);
	}

	std::string type, expression;
	std::getline(file, type);
	std::getline(file, expression);
	expr = Expression(expression, type);
	expr.normalaize();
	file.close();
}

void ExpressionEditor::showMenu() {
	while (true) {
		system("cls");
		std::cout << "Окно редактирования\n\n";
		std::cout << "Выберете один из пунктов и введите его цифру:\n\n";
		std::cout << "1) Показать введенные данные\n";
		std::cout << "2) Добавить константу\n";
		std::cout << "3) Показать результат\n";
		std::cout << "4) Выход\n";
		std::cout << "Выбор: ";

		int choice;
		std::cin >> choice;
		std::cin.ignore();
		handleUserChoice(choice);
	}
}

void ExpressionEditor::handleUserChoice(int choice) {
	switch (choice) {
	case 1:
		displayExpression();
		break;
	case 2:
		addConstant();
		break;
	case 3:
		showResult();
		break;
	case 4:
		exit(0);
	default:
		std::cout << "Ошибка: неправильный ввод!" << std::endl;
		break;
	}
}

void ExpressionEditor::displayExpression() {
	system("cls");
	std::cout << "Тип выражения: " << expr.getType() << std::endl;
	std::cout << "Выражение: " << expr.getRawExpression() << std::endl;
	std::cout << "Нажмите любую клавишу для выхода...";
	std::cin.get();
}

void ExpressionEditor::addConstant() {
	system("cls");
	if (expr.getType() == "Арифметическое")
	{
		std::cout << "Введите константу: ";
		int c;
		std::cin >> c;
		std::cin.ignore();
		constants.push_back(c);
		expr.addConst(c);

		std::cout << "Константа добавлена. Нажмите любую клавишу...";
		std::cin.get();
	}
	else if (expr.getType() == "Логическое")
	{
		std::cout << "Логические выражения не поддерживают константы. Нажмите любую клавишу...";
		std::cin.get();
	}
}

void ExpressionEditor::showResult() {
	system("cls");
	std::cout << "Обработка...\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	system("cls");

	expr.normalaize();
	std::cout << "Упрощенное выражение: " << expr.getNormolizedExpression() << std::endl;

	for (int c : constants) {
		expr.addConst(c);
	}

	if (expr.getType() == "Арифметическое")
	{
		std::cout << "С добавленными константами: " << expr.getNormolizedExpression() << std::endl;
	}

	std::cout << "Нажмите любую клавишу для выхода...";
	std::cin.get();
}

#include "ExpressionEditor.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>


ExpressionEditor::ExpressionEditor(const std::string& filename) : filename(filename), expr(*(new std::string("�")), *(new std::string("�"))) {}

void ExpressionEditor::loadFromFile() {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "������: �� ������� ������� ����!" << std::endl;
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
		std::cout << "���� ��������������\n\n";
		std::cout << "�������� ���� �� ������� � ������� ��� �����:\n\n";
		std::cout << "1) �������� ��������� ������\n";
		std::cout << "2) �������� ���������\n";
		std::cout << "3) �������� ���������\n";
		std::cout << "4) �����\n";
		std::cout << "�����: ";

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
		std::cout << "������: ������������ ����!" << std::endl;
		break;
	}
}

void ExpressionEditor::displayExpression() {
	system("cls");
	std::cout << "��� ���������: " << expr.getType() << std::endl;
	std::cout << "���������: " << expr.getRawExpression() << std::endl;
	std::cout << "������� ����� ������� ��� ������...";
	std::cin.get();
}

void ExpressionEditor::addConstant() {
	system("cls");
	if (expr.getType() == "��������������")
	{
		std::cout << "������� ���������: ";
		int c;
		std::cin >> c;
		std::cin.ignore();
		constants.push_back(c);
		expr.addConst(c);

		std::cout << "��������� ���������. ������� ����� �������...";
		std::cin.get();
	}
	else if (expr.getType() == "����������")
	{
		std::cout << "���������� ��������� �� ������������ ���������. ������� ����� �������...";
		std::cin.get();
	}
}

void ExpressionEditor::showResult() {
	system("cls");
	std::cout << "���������...\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	system("cls");

	expr.normalaize();
	std::cout << "���������� ���������: " << expr.getNormolizedExpression() << std::endl;

	for (int c : constants) {
		expr.addConst(c);
	}

	if (expr.getType() == "��������������")
	{
		std::cout << "� ������������ �����������: " << expr.getNormolizedExpression() << std::endl;
	}

	std::cout << "������� ����� ������� ��� ������...";
	std::cin.get();
}

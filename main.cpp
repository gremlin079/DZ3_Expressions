#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include "Expression.hpp"
#include "ArithmeticExpressionProcessor.hpp"
#include "ExpressionEditor.hpp"

int main() {
	setlocale(LC_ALL, "Russian");

	for (int i = 0; i <= 100; i += 1) {
		std::cout << "\rЗагрузка: " << i << "%";
		std::cout.flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(8));
	}
	system("cls");
	std::cout << "-Редактор выражений-\n\n";
	std::cout << "Введите любой символ для продолжения...";
	std::cin.get();
	system("cls");
	std::cout << "Введите имя файла(с расширением): ";
	std::string filename;
	std::cin >> filename;

	ExpressionEditor editor(filename);
	editor.loadFromFile();
	editor.showMenu();

	return 0;
}

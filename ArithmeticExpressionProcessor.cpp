#include "ArithmeticExpressionProcessor.hpp"
#include "Expression.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

//конструктор
ArithmeticExpressionProcessor::ArithmeticExpressionProcessor(Expression& expr) : expr(expr)
{
}
//метод приведения выражения к стандартному виду
void ArithmeticExpressionProcessor::convertToStandartForm()
{
	int colvo_x = 0; //количество иксов
	std::vector<int> coefficents; //массив констант, которые прибавляются к иксам в скобках
	int i = 0; 
	std::vector<int> poz_znak; //массив позиций знаков + и -
	std::vector<int> poz_scobka; //массив позиций скобок
	std::vector<int> x_coeff{ 1 }; //массив коэффицентов перед иксами уже в стандартном  виде

	//заполняем вышеуказанные переменные
	for (char x : expr.getRawExpression())
	{
		if (x == 'x')
		{
			colvo_x += 1;
		}

		if (x == '+' or x == '-')
		{
			poz_znak.push_back(i);
		}

		if (x == ')')
		{
			poz_scobka.push_back(i);
		}
		i += 1;
	}

	//цикл для нахождения всех коэффицентов, которые мы прибавляем или вычитаем
	for (int i = 0; i < colvo_x; i++)
	{
		std::string Help_string;
		for (int k = poz_znak[i]; k < poz_scobka[i]; k++)
		{
			Help_string += expr.getRawExpression()[k];
		}

		coefficents.push_back(std::stoi(Help_string));
	}

	//комбиноаторная формула для нахождения по имеющимся данным коээфицентов перед иксами в стандартной форме
	//массив заполняется в соответсвии с убыванием степеней иксов
	//емае, я допер как это правильно работает, напомни мне утром, я норм объясню, а то так не оч удобно
	//однако я гений, что такую тему сюда засунул
	for (int i = 1; i < colvo_x; i++)
	{
		int n = coefficents.size();
		std::vector<bool> mask(n, false);
		int total_sum = 0;

		std::fill(mask.end() - i, mask.end(), true);

		do {
			int product = 1;
			for (int k = 0; k < n; ++k)
			{
				if (mask[k])
				{
					product *= coefficents[k];
				}
			}

			total_sum += product;
		} while (std::next_permutation(mask.begin(), mask.end()));

		x_coeff.push_back(total_sum);
	}
	std::string return_this; //итоговое выражение, которое мы вернем
	//цикл, которой склеивает имеющиеся данный в единный стандартный вид
	for (int i = 0; i < colvo_x; i++)
	{
		//записываем первый икс
		if (i == 0)
		{
			return_this += "x^" + std::to_string(colvo_x);
		}
		//записываем дальше иксы, в зависимости от знака коэффицента при нем
		else
		{
			if (x_coeff[i] >= 0)
			{
				return_this += " + " + std::to_string(x_coeff[i]) + "x^" + std::to_string(colvo_x - i);
			}
			else
			{
				return_this += " - " + std::to_string(abs(x_coeff[i])) + "x^" + std::to_string(colvo_x - i);
			}
		}

	}

	int free_coef = 1;//считаем свободный член, есть формула для его вычисления, здесь она реализована
	for (int el : coefficents)
	{
		free_coef *= el;
	}
	//добавляем к стандартному виду свободный коэффицент в зависимости от его знака
	if (free_coef >= 0)
	{
		return_this += " + " + std::to_string(free_coef);
	}
	else
	{
		return_this += " - " + std::to_string(abs(free_coef));
	}

	//записываем наше выражение, через метод класса Expression
	expr.setNormolizedExpression(return_this);

}
//метод добавления константы к выражению в стандартном виде
void ArithmeticExpressionProcessor::addConst(int constant)
{
	std::string now_expression = expr.getNormolizedExpression();//получаем ввыражение в стандартном виде
	std::string changeme; //эту строку будем изменять и впоследствие вернем
	std::vector<int> poz_znak; // отслеживаем позиции знаков + и -
	std::string help_str;//сюда мы считаем свободный член выражения
	bool flag_polozh = false; //флаг для определения знака свободного члена
	//записываем позиции знаков
	for (int i = 0; i < now_expression.size(); i++)
	{
		if (now_expression[i] == '+' or now_expression[i] == '-')
		{
			poz_znak.push_back(i);

		}
	}
	//проверяем положителен ли свободный член
	if (now_expression[poz_znak[poz_znak.size() - 1]] == '+')
	{
		flag_polozh = true;
	}

	//считываем свободный член выражения
	for (int i = poz_znak[poz_znak.size() - 1]; i < now_expression.size(); i++)
	{
		if (now_expression[i] != ' ')
		{
			help_str += now_expression[i];
		}
	}

	//переносим наше выражение пока без свободного члена
	for (int i = 0; i < poz_znak[poz_znak.size() - 1]; i++)
	{
		changeme += now_expression[i];
	}

	//проверяем знак нового свободного члена, после добавления константы, и записываем в зависимости от результата
	if ((std::stoi(help_str) + constant) >= 0)
	{
		changeme += "+ " + std::to_string(std::stoi(help_str) + constant);
	}
	else
	{
		changeme += "- " + std::to_string(abs(std::stoi(help_str) + constant));
	}
	//возвращаем измененное выражение через метод класса Expression
	expr.setNormolizedExpression(changeme);

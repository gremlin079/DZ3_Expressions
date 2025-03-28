#include "ArithmeticExpressionProcessor.hpp"
#include "Expression.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

ArithmeticExpressionProcessor::ArithmeticExpressionProcessor(Expression& expr) : expr(expr)
{
}

void ArithmeticExpressionProcessor::convertToStandartForm()
{
	int colvo_x = 0;
	std::vector<int> coefficents;
	int i = 0;
	std::vector<int> poz_znak;
	std::vector<int> poz_scobka;
	std::vector<int> x_coeff{ 1 };

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

	for (int i = 0; i < colvo_x; i++)
	{
		std::string Help_string;
		for (int k = poz_znak[i]; k < poz_scobka[i]; k++)
		{
			Help_string += expr.getRawExpression()[k];
		}

		coefficents.push_back(std::stoi(Help_string));
	}

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
	std::string return_this;
	for (int i = 0; i < colvo_x; i++)
	{
		if (i == 0)
		{
			return_this += "x^" + std::to_string(colvo_x);
		}
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

	int free_coef = 1;
	for (int el : coefficents)
	{
		free_coef *= el;
	}

	if (free_coef >= 0)
	{
		return_this += " + " + std::to_string(free_coef);
	}
	else
	{
		return_this += " - " + std::to_string(abs(free_coef));
	}

	expr.setNormolizedExpression(return_this);

}

void ArithmeticExpressionProcessor::addConst(int constant)
{
	std::string now_expression = expr.getNormolizedExpression();
	std::string changeme;
	std::vector<int> poz_znak;
	std::string help_str;
	bool flag_polozh = false;
	for (int i = 0; i < now_expression.size(); i++)
	{
		if (now_expression[i] == '+' or now_expression[i] == '-')
		{
			poz_znak.push_back(i);

		}
	}

	if (now_expression[poz_znak[poz_znak.size() - 1]] == '+')
	{
		flag_polozh = true;
	}

	for (int i = poz_znak[poz_znak.size() - 1]; i < now_expression.size(); i++)
	{
		if (now_expression[i] != ' ')
		{
			help_str += now_expression[i];
		}
	}

	for (int i = 0; i < poz_znak[poz_znak.size() - 1]; i++)
	{
		changeme += now_expression[i];
	}

	if ((std::stoi(help_str) + constant) >= 0)
	{
		changeme += "+ " + std::to_string(std::stoi(help_str) + constant);
	}
	else
	{
		changeme += "- " + std::to_string(abs(std::stoi(help_str) + constant));
	}

	expr.setNormolizedExpression(changeme);
}

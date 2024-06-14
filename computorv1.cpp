#include "computorv1.hpp"

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool isValidEquation(std::string equation)
{
	std::string validChars = "0123456789.X^+-*/=() ";
	for (int i = 0; i < equation.size(); i++)
	{
		if (validChars.find(equation[i]) == std::string::npos)
			return false;	
	}
	//! std::vector<std::string> terms = split(equation,'=');
	// if (terms.size() != 2)
	// 	return false;
	return true;
}

void getDigit( std::string equation, int *i, double *digit, Possibilities parse)
{
	std::string temp = "";
	bool usedDot = false;
	while (isdigit(equation[*i]) || equation[*i] == '.')
	{
		temp += equation[*i];
		(*i)++;
		if (equation[*i] == '.' && usedDot)
		{
			std::cerr << "Invalid number" << std::endl;
			exit(1);
		}
		if (equation[*i] == '.')
			usedDot = true;
	}
	(*i)--; // to compensate for the incrementation in the while loop
	*digit = std::stod(temp);
	if (parse.isNegative)
		*digit *= -1;
	if (parse.isRightSide)
		*digit *= -1;
	// std::cout << "temp: " << *digit << std::endl;
}

void sendValue(Delta *save, Delta *temp, double *change)
{
	if (change != 0)
	{
		save->c.value += *change;
		*change = 0;
	}
	save->a.value += temp->a.value;
	save->b.value += temp->b.value;
	save->c.value += temp->c.value;
	std::cout << "save: " << save->a.value << " " << save->b.value << " " << save->c.value << std::endl;
	// std::cout << "temp: " << temp->a.value << " " << temp->b.value << " " << temp->c.value << std::endl;
	
	temp->a.value = 0;
	temp->b.value = 0;
	temp->c.value = 0;
}

void putToRightPower(Delta *temp, double *change, Possibilities parse, std::string equation, int *i)
{
	//5 * X^2,x^2 * 5, 5 * x, 5 * x^0, 5 * x^1
	if (parse.isMult && parse.isX)
	{
		if (equation[*i + 1] == '^')
		{
			(*i)++;
			double power = 0;
			getDigit(equation, i, &power, parse);
			if (power == 0)
				temp->c.value += *change;
			else if (power == 1)
				temp->b.value += *change;
			else if (power == 2)
				temp->a.value += *change;
			else
			{
				std::cerr << "Invalid power" << std::endl;
				exit(1);
			}
		}
		else
			temp->b.value += *change;
	}
	change = 0;
}

int main(int argc, char **argv)
{
	Delta temp, value;
	Possibilities parse;
	std::string equation = argv[1];
	double change = 0;

	if (argc != 2 || !isValidEquation(equation))
	{
		if (argc != 2)
			std::cerr << "Usage: " << argv[0] << " <equation>" << std::endl;
		else
			std::cerr << "Invalid equation" << std::endl;
		return 1;
	}
	int i = -1;
	while(++i < equation.size())
	{
		if (isdigit(equation[i])|| equation[i] == '.')
		{
			getDigit(equation, &i, &change,parse);
			putToRightPower(&temp, &change, parse, equation, &i);
			std::cout << "equation: " << temp.a.value << " "<< equation.substr(i, std::string::npos) << std::endl;
		}
		else
		{
			std::cout << "switch: " << equation[i] << std::endl;
			switch (equation[i])
			{
			case '+':
				parse.isNegative = false;
				if (!parse.isParenthesis)
					sendValue(&value, &temp,&change);
				break;
			case '-':
				parse.isNegative = true;
				if (!parse.isParenthesis)
					sendValue(&value, &temp,&change);
				break;
			case '=':
				parse.isRightSide = true;
				break;
			case '*':
				parse.isMult = true;
				parse.isDiv = false;
				break;
			case '/':
				parse.isDiv = true;
				parse.isMult = false;
				break;
			case '(':
				parse.isParenthesis = true;
				break;
			case ')':
				parse.isParenthesis = false;
				break;
			case 'X':
				parse.isX = true;
				// putToRightPower(&temp, &change, parse, equation, &i);
				break;
			case '^':
				parse.isPower = true;
				break;
			case ' ':
				break;
			default:
				break;
			}
		}
	}
	sendValue(&value, &temp,&change);
	putToRightPower(&temp, &change, parse, equation, &i);




	std::cout << "a: " << value.a.value << " b: " << value.b.value << " c: " << value.c.value << std::endl;

	return 0;
}



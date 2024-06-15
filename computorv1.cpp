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
	std::string validChars = "0123456789.X^+/-*=() ";
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

void getDigit( std::string equation, int *i, double *digit, Possibilities *parse)
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
	// std::cout << "temp: " << *digit << std::endl;
	double tempDigit = std::stod(temp);
	if (parse->isNegative)
	{
		tempDigit *= -1;
		parse->isNegative = false;
	}
	if (parse->isRightSide)
	{
		tempDigit *= -1;	
	}
	if (parse->isMult)
	{
		parse->isMult = false;
		*digit *= tempDigit;
	}
	else if (parse->isDiv)
	{
		parse->isDiv = false;
		*digit /= tempDigit;
	}
	else
		*digit = tempDigit;
	// std::cout << "temp: " << *digit << " rightside "<< parse.isRightSide << " Neg "<< parse.isNegative << std::endl;
}

void sendValue(Delta *save, Delta *temp, double *change)
{
	// std::cout << "cHANGE: " << *change << std::endl;

	if (change != 0)
	{
		temp->c.value += *change;
	}
	// std::cout << "save: " << save->a.value << " " << save->b.value << " " << save->c.value << std::endl;
	save->a.value += temp->a.value;
	save->b.value += temp->b.value;
	save->c.value += temp->c.value;
	// std::cout << "save after: " << save->a.value << " " << save->b.value << " " << save->c.value << std::endl;
	// std::cout << "temp: " << temp->a.value << " " << temp->b.value << " " << temp->c.value << std::endl;
	
	*change = 0;
	temp->a.value = 0;
	temp->b.value = 0;
	temp->c.value = 0;
}

void printPossibilities(Possibilities parse)
{
	std::cout << "isNegative: " << parse.isNegative << std::endl;
	std::cout << "isRightSide: " << parse.isRightSide << std::endl;
	std::cout << "isParenthesis: " << parse.isParenthesis << std::endl;
	std::cout << "isPower: " << parse.isPower << std::endl;
	std::cout << "isX: " << parse.isX << std::endl;
	std::cout << "isMult: " << parse.isMult << std::endl;
	std::cout << "isDiv: " << parse.isDiv << std::endl;
}

void putToRightPower(Delta *temp, double *change, Possibilities *parse, std::string equation, int *i)
{
	//5 * X^2,x^2 * 5, 5 * x, 5 * x^0, 5 * x^1
	// std::cout << "is mult: " << parse->isMult << " isX: " << parse->isX << std::endl;
	bool tempisNegative = parse->isNegative, tempisRightSide = parse->isRightSide;
	if (parse->isX)
	{
		parse->isX = false;
		if (*change == 0)
			*change = 1;
		if (equation[(*i) + 1] == '^')
		{
			parse->isPower = false;
			
			(*i)+= 2;
			double power = 0;
			parse->isNegative = false;
			parse->isRightSide = false;
			// std::cout << "equation: " << equation.substr(*i, std::string::npos) << std::endl;
			getDigit(equation, i, &power, parse);
			parse->isNegative = tempisNegative;
			parse->isRightSide = tempisRightSide;
			// std::cout << "power: " << power << " change "<< *change << std::endl;
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
		{
			// std::cout << " change power "<< *change << std::endl;
			temp->b.value += *change;
		}
		*change = 0;
	}
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
			if (!parse.isAdd)
			{
				std::cerr << "Invalid equation add" << std::endl;
				exit(1);	
			}
			parse.isAdd = false;
			getDigit(equation, &i, &change,&parse);
			putToRightPower(&temp, &change, &parse, equation, &i);
			// std::cout << "equation: " << change << " "<< equation.substr(i, std::string::npos) << std::endl;
		}
		else
		{
			// std::cout << "switch: " << equation[i] << std::endl;
			switch (equation[i])
			{
			case '+':
				parse.isAdd = true;
				parse.isNegative = false;

				if (!parse.isParenthesis)
					sendValue(&value, &temp,&change);
				break;
			case '-':
				parse.isAdd = true;
				parse.isNegative = true;
				if (!parse.isParenthesis)
					sendValue(&value, &temp,&change);
				break;
			case '=':
				parse.isNegative = false;
				parse.isAdd = true;
				parse.isRightSide = true;
				break;
			case '*':
				parse.isMult = true;
				parse.isAdd = true;
				parse.isDiv = false;
				break;
			case '/':
				parse.isAdd = true;
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
				parse.isAdd = false;
				parse.isMult = false;
				parse.isDiv = false;

				// std::cout << "value: " << value.a.value << " " << value.b.value << " " << value.c.value << std::endl;
				putToRightPower(&temp, &change, &parse, equation, &i);
				// std::cout << "value after: " << value.a.value << " " << value.b.value << " " << value.c.value << std::endl;

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
	putToRightPower(&temp, &change, &parse, equation, &i);




	std::cout << "a: " << value.a.value << " b: " << value.b.value << " c: " << value.c.value << std::endl;

	return 0;
}


 
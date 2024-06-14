#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int PowerOfX(std::string equation)
{
	if (equation.find("X^2") != std::string::npos)
		return 2;
	else if (equation.find("X^1") != std::string::npos)
		return 1;
	else if (equation.find("X^0") != std::string::npos)
		return 0;
	else if (equation.find("X") != std::string::npos)
		return 1;
	else
		return -1;
}

bool isValidEquation(std::string equation)
{
	std::string validChars = "0123456789X^+-*/= ";
	for (int i = 0; i < equation.size(); i++)
	{
		if (validChars.find(equation[i]) == std::string::npos)
			return false;	
	}
	std::vector<std::string> terms = split(equation,'=');
	if (terms.size() != 2)
		return false;
	return true;
}

bool isValidTerm(std::string equation)
{
	std::string validOperators = "+-*/=";
	bool usedOperator = false;
	bool usedNumber = false;
	bool usedX = false;
	bool usedPower = false;
	for (int i = 0; i < equation.size(); i++)
	{
		if (validOperators.find(equation[i]) != std::string::npos)
		{
			if (usedOperator || usedNumber|| usedX)
				return false;
			usedOperator = true;
		}
		else if (isdigit(equation[i]))
		{
			usedNumber = true;
		}
		else if (equation[i] == 'X')
		{
			if (usedX)
				return false;
			usedX = true;
		}
		else if (equation[i] == '^')
		{
			if (usedPower)
				return false;
			usedPower = true;
		}
	}
	return true;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <equation>" << std::endl;
		return 1;
	}
	// -x^2*-456+2*x^1-x+86=0
	std::string equation = argv[1];
	float a = 0, b = 0, c = 0, temp = 0;
	int power = 0;
	bool isEquals = false, isMultiply = false, isDivide = false, isAdd = false, isSubstract = false;
	std::vector<std::string> terms = split(equation,' ');
	if (!isValidEquation(equation))
	{
		std::cerr << "Invalid equation" << std::endl;
		return 1;
	}
	
	for (std::string term : terms)
	{
		if (!isValidTerm(term))
		{
			std::cerr << "Invalid term" << std::endl;
			return 1;
		}
		if (term.find("X") != std::string::npos)
		{
			power = PowerOfX(term);
			if (power == -1)
			{
				std::cerr << "Invalid term" << std::endl;
				return 1;
			}
			std::cout << "Power: " << power << "temp: "<< temp << std::endl;
			if (temp != 0)
			{
				switch (power)
				{
				case 0:
					c += temp;
					break;
				case 1:
					b += temp;
					break;
				case 2:
					a += temp;
					break;
				default:
					break;
				}
			}
		}
		else if (term.find("=") != std::string::npos)
		{
			isEquals = true;
			isAdd = false;
			isSubstract = false;
			isMultiply = false;
			isDivide = false;
		}
		else if (term.find("+") != std::string::npos)
		{
			isAdd = true;
			isSubstract = false;
			isMultiply = false;
			isDivide = false;
		}
		else if (term.find("-") != std::string::npos)
		{
			isSubstract = true;
			isAdd = false;
			isMultiply = false;
			isDivide = false;
		}
		else if (term.find("*") != std::string::npos)
		{
			isMultiply = true;
			isDivide = false;
			isAdd = false;
			isSubstract = false;
		}
		else if (term.find("/") != std::string::npos)
		{
			isDivide = true;
			isMultiply = false;
			isAdd = false;
			isSubstract = false;
		}


		else
		{
			std::cout << "term: " << term << std::endl;
			temp =std::stof(term);
			// std::cout << "temp: " << temp << std::endl;
			if (isEquals)
				temp *= -1;
			if (isSubstract)
			{
				c -= temp;
			}
			else if (isAdd)
			{
				c += temp;
			}
		}

	}
	/*if (temp != 0)
	{
		switch (power)
		{
		case 0:
			c += temp;
			break;
		case 1:
			b += temp;
			break;
		case 2:
			a += temp;
			break;
		default:
			break;
		}
	}*/

	std::cout << "a:" << a << " b:" << b << " c:" << c << std::endl;
	// ax^2 + bx + c = 0

	// Split equation into terms
	

	return 0;
}
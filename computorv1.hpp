#pragma once
#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> split(const std::string &s, char delimiter);

struct Power{
	int power=0;
	double value=0;
};

struct Delta {
	Power a,b,c;
	double delta=0, x1=0, x2=0;
};

struct Possibilities{
	bool isAdd=true;
	bool isNegative=false;
	bool isRightSide=false;
	bool isParenthesis=false;
	bool isPower=false;
	bool isX=false;
	bool isMult=false;
	bool isDiv=false;

};

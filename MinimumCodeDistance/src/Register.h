#pragma once
#include <vector>

class Register
{
public:
	Register(unsigned int field, unsigned int size, unsigned int initValue);

	unsigned int operator[](unsigned int index);

	Register& operator++();
	Register& operator++(int);

private:
	unsigned int _field;
	std::vector<unsigned int> _register;
};


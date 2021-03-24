#pragma once
#include "pch.h"
#include "Register.h"

Register::Register(unsigned int field, unsigned int size, unsigned int initValue = 0) : _field(field)
{
	unsigned int remainder = initValue;

	unsigned int index = 0;

	_register.reserve(size);

	do
	{
		_register.push_back(remainder);
		remainder = _register[index] / _field;
		_register[index] = _register[index] % _field;
		index++;
	} while (remainder != 0 && index < size);
}

unsigned int Register::operator[](unsigned int index)
{
	if (index < 0 || index >= _register.capacity())
	{
		throw std::invalid_argument("Trying to acces ivalid register segment");
	}
	return _register[index];
}

Register& Register::operator++()
{
	unsigned int index = 0, remainder;

	do
	{
		if (index >= _register.capacity())
		{
			_register.push_back(1);
		}
		_register[index]++;
		remainder = _register[index] / _field;
		_register[index] = _register[index] % _field;
		index++;
	} while (remainder != 0);

	return *this;
}

Register& Register::operator++(int)
{
	return ++(*this);
}

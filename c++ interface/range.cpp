
#include "stdafx.h"
#include "range.h"

RANGE::operator int()
{
	return val;
}

RANGE &RANGE::operator = (const int &arg)
{
	val = arg;
	return (*this);
}

RANGE &RANGE::operator ++ (int)
{
	val++;
	return (*this);
}

bool RANGE::operator == (int arg)
{
	bool result;
	if (val==arg)
		result=true;
	else
		result=false;
	return result;
}

bool RANGE::operator <= (int arg)
{
	bool result;
	if (val<=arg)
		result=true;
	else
		result=false;
	return result;
}

JTABRANGE &JTABRANGE::operator = (const int &arg)
{
	val = arg;
	return (*this);
}

CURSRANGE &CURSRANGE::operator = (const int &arg)
{
	val = arg;
	return (*this);
}

RANGE &DISPRANGE::operator = (const int &arg)
{
	val = arg;
	return (*this);
}

ADDRRANGE &ADDRRANGE::operator = (const int &arg)
{
	val = arg;
	return (*this);
}

RANGE &PROCRANGE::operator = (const int &arg)
{
	val = arg;
	return (*this);
}

SEGRANGE &SEGRANGE::operator = (const int &arg)
{
	val = arg;
	return (*this);
}

RANGE &LEVRANGE::operator = (const int &arg)
{
	val = arg;
	return (*this);
}
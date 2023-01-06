
#include "stdafx.h"
#include "defines.h"
#include "range.h"

#if 0
RANGE::RANGE(int min=0, int max=0x7FFFFFFF)
{
	m_min=min;
	m_max=max;
}
#endif

BITRANGE &BITRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

JTABRANGE &JTABRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

CURSRANGE &CURSRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

DISPRANGE &DISPRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

ADDRRANGE &ADDRRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

PROCRANGE &PROCRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

SEGRANGE &SEGRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

LEVRANGE &LEVRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

OPRANGE::OPRANGE ()
{
	val = 0;
}

OPRANGE::OPRANGE (int arg)
{
	val = arg;
}

OPRANGE &OPRANGE::operator = (int arg)
{
	val = arg;
	return *this;
}

OPRANGE OPRANGE::operator + (int arg)
{
	int result0;
	result0 = val+arg;
	OPRANGE result;
	result.val = result0;
	return result;
}

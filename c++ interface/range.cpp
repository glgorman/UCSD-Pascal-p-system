
#include "stdafx.h"
#include "defines.h"
#include "range.h"

#if 0
RANGE BITRANGE(0,BITSPERWD);
RANGE OPRANGE(0,80);
RANGE CURSRANGE(0,MAXCURSOR);
RANGE PROCRANGE(0,MAXPROCNUM);
RANGE LEVRANGE(0,MAXLEVEL);
RANGE ADDRRANGE(0,MAXADDR);
RANGE JTABRANGE(0,MAXJTAB);
RANGE SEGRANGE(0,MAXSEG);
RANGE DISPRANGE(0,DISPLIMIT);
#endif

#if 0
RANGE::RANGE(int min, int max)
{
	m_min=min;
	m_max=max;
}
#endif

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

bool RANGE::operator<=(int arg)
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
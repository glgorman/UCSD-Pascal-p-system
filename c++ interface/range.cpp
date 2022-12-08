
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
RANGE::RANGE(int min=0, int max=0x7FFFFFFF)
{
	m_min=min;
	m_max=max;
}
#endif

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

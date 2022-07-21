
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

RANGE &RANGE::operator = (int arg)
{
	val = arg;
	ASSERT(val>=0);
	return (*this);
}

RANGE &RANGE::operator ++ (int)
{
	val++;
	return (*this);
}

RANGE &RANGE::operator -- (int)
{
	ASSERT(val>0);
	val--;
	return (*this);
}

RANGE RANGE::operator + (int arg)
{
	RANGE result;
	result.val=val+arg;
	return result;
}

RANGE RANGE::operator - (int arg)
{
	RANGE result;
	result.val=val-arg;
	return result;
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

RANGE &DISPRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

ADDRRANGE &ADDRRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

RANGE &PROCRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

SEGRANGE &SEGRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}

RANGE &LEVRANGE::operator = (int arg)
{
	val = arg;
	return (*this);
}


OPRANGE::OPRANGE ()
{

}

OPRANGE::OPRANGE (int)
{

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

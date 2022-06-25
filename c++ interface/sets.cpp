
//
// functions needed to support "set" type in Pascal
// like languages.  Copyright 2021 Gerold Lee Gorman
// Permission to redistribute and make use of this
// software under GNU/MIT license.
//

#include "stdafx.h"
#include <stdarg.h>
#include "range.h"
#include "sets.h"

namespace chartype
{
	SET symbol(63,'0','1','2','3','4','5','6','7','8','9','_',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z');
	SET digits(10,'0','1','2','3','4','5','6','7','8','9');
	SET whitespace(4,' ','\t','\n','\r');
	SET alpha(52,'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z');
	SET punct1(12,'.',',',':','\\','"','\'','(',')','{','}','[',']');
	SET operat(11,'+','-','*','/','%','|','~','=','&','>','<');
}

SET::SET()
{
	memset(bits,0,sizeof(DWORD)*SETSZ);
}

SET::SET(int n, ...)
{
	int i,j,k,val;
	va_list vl;
	va_start(vl,n);
	memset(bits,0,sizeof(DWORD)*SETSZ);
	for (i=0;i<n;i++)
	{
		val=va_arg(vl,int);
		j = val>>5;
		k = val&0x1f;
		bits[j]|=(1<<k);
	}
	va_end(vl);
}

SET &SET::operator + (const SET &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]|=x.bits[i];
	return (*this);
}

SET &SET::operator - (const SET &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]&=(!x.bits[i]);
	return (*this);
}

#if 0
bool SET::in(int s)
{
	bool result;
	int i, j;
	i = s>>5;
	j = s&0x1f;
	if ((bits[i])&(0x01<<j))
		result = true;
	else
		result = false;
	return result;
}
#endif

SET &SET::UNION(const SET&S)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]|=S.bits[i];
	return (*this);
}

SET &SET::INTERSECT(const SET&S)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]&=S.bits[i];
	return (*this);
}

SETOFSYS::SETOFSYS()
{
}

SETOFSYS::SETOFSYS(const SET &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
}

SETOFSYS &SETOFSYS::operator + (const SETOFSYS &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]|=x.bits[i];
	return (*this);
}

SETOFSYS &SETOFSYS::operator + (const int &val)
{
	int j, k;
	j = val>>5;
	k = val&0x1f;
	bits[j]|=(1<<k);
	return (*this);
}

SETOFSYS &SETOFSYS::operator = (const SET &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
	return (*this);
}

SETOFIDS::SETOFIDS()
{

}

SETOFIDS::SETOFIDS(int n,...)
{
	int i,j,k,val;
	va_list vl;
	va_start(vl,n);
	memset(bits,0,sizeof(DWORD)*SETSZ);
	for (i=0;i<n;i++)
	{
		val=va_arg(vl,int);
		j = val>>5;
		k = val&0x1f;
		bits[j]|=(1<<k);
	}
	va_end(vl);
}

SETOFIDS &SETOFIDS::operator = (const SET &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
	return (*this);
}


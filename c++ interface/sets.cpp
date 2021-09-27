
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

#define	SETSZ (16)

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

bool SET::in(int s)
{
	bool result;
	int i, j;
	i = s>>5;
	j = s&0x1f;
	if ((bits[i]>>j)&0x01)
		result = true;
	else
		result = false;
	return result;
}

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


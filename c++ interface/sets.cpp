
//
// functions needed to support "set" type in Pascal
// like languages.  Copyright 2021 Gerold Lee Gorman
// Permission to redistribute and make use of this
// software under GNU/MIT license.
//

#include "stdafx.h"
#include <stdarg.h>
#include <vector>
#include "intrinsics.h"
#include "range.h"
#include "sets.h"

extern char *ID_NAMES1[];
extern char *SYMBOL_NAMES1[];

namespace chartypes
{
	SETOFCHAR ident(63,'0','1','2','3','4','5','6','7','8','9','_',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z');
	SETOFCHAR digits(10,'0','1','2','3','4','5','6','7','8','9');
	SETOFCHAR hex(22,'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','a','b','c','d','e','f');
	SETOFCHAR whitespace(4,' ','\t','\n','\r');
	SETOFCHAR alpha(52,'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z');
	SETOFCHAR punct1(12,'.',',',':','\\','"','\'','(',')','{','}','[',']');
	SETOFCHAR operat(11,'+','-','*','/','%','|','~','=','&','>','<');
}


SETOFSYS::SETOFSYS()
{
}

SETOFSYS::SETOFSYS(size_t n, ...)
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

SETOFSYS::SETOFSYS(const SET &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
}

SETOFSYS SETOFSYS::operator + (const SETOFSYS &x) const
{
	SETOFSYS result;
	result = *this;
	int i;
	for (i=0;i<SETSZ;i++)
		result.bits[i]|=x.bits[i];
	return result;
}

SETOFSYS SETOFSYS::operator + (int val) const
{
	SETOFSYS result;
	result = *this;
	int j, k;
	j = val>>5;
	k = val&0x1f;
	result.bits[j]|=(1<<k);
	return result;
}

SETOFSYS &SETOFSYS::operator += (int val)
{
	int j, k;
	j = val>>5;
	k = val&0x1f;
	bits[j]|=(1<<k);
	return (*this);
}

SETOFSYS &SETOFSYS::operator -= (int val)
{
	int j,k;
	DWORD s1, s2;
	j = val>>5;
	k = val&0x1f;
	s1 = bits[j];
	s2 = ~(1<<k);
	bits[j]=s1&s2;
	return (*this);
}

SETOFSYS &SETOFSYS::operator = (const SETOFSYS &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
	return (*this);
}

SETOFSYS SETOFSYS::range (int low, int high)
{
	SETOFSYS result;
	int i;
	for (i=low;i<=high;i++)
		result+=i;
	return result;
}

void SETOFSYS::debug_list (char *str1) const
{
	int n;
	char *str2;
	size_t sz = SETSZ*sizeof(DWORD)*8; 
	WRITE(OUTPUT,"SETOFSYS ",str1,": (");
	for (n=0;n<sz;n++)
	if (in(n))
	{
		str2 = SYMBOL_NAMES1[n];
		WRITE (OUTPUT,str2,",");
	}
	WRITELN(OUTPUT,")");
}

SETOFIDS::SETOFIDS()
{

}

SETOFIDS::SETOFIDS(size_t n,...)
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

void SETOFIDS::debug_list (char *str1) const
{
	int n;
	char *str2;
	size_t sz = SETSZ*sizeof(DWORD)*8; 
	WRITE(OUTPUT,"SETOFIDS ",str1,": (");
	for (n=0;n<sz;n++)
	if (in(n))
	{
		str2 = ID_NAMES1[n];
		WRITE (OUTPUT,str2,",");
	}
	WRITELN(OUTPUT,")");
}

SETOFIDS SETOFIDS::operator + (int val) const
{
	SETOFIDS result;
	result = *this;
	int j, k;
	j = val>>5;
	k = val&0x1f;
	result.bits[j]|=(1<<k);
	return result;
}

SETOFIDS &SETOFIDS::operator = (const SETOFIDS &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
	return (*this);
}

SETOFIDS SETOFIDS::operator + (const SETOFIDS &x) const
{
	SETOFIDS result;
	result = *this;
	int i;
	for (i=0;i<SETSZ;i++)
		result.bits[i]|=x.bits[i];
	return result;
}

SETOFIDS::SETOFIDS(const SETOFIDS &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
}

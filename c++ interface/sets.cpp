
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

extern char *SYMBOL_NAMES1[];

namespace chartype
{
	SET ident(63,'0','1','2','3','4','5','6','7','8','9','_',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z');
	SET digits(10,'0','1','2','3','4','5','6','7','8','9');
	SET hex(22,'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','a','b','c','d','e','f');
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

bool SET::operator == (const SET &x) const
{
	bool result = true;
	int i;
	for (i=0;i<SETSZ;i++)
	if (bits[i]!=x.bits[i])
	{
		result=false;
		break;
	}
	return result;
}

bool SET::operator != (const SET &x) const
{
	bool result = false;
	int i;
	for (i=0;i<SETSZ;i++)
	if (bits[i]!=x.bits[i])
	{
		result=true;
		break;
	}
	return result;
}

SET SET::operator + (const SET &x) const
{
	SET result;
	result = *this;
	int i;
	for (i=0;i<SETSZ;i++)
		result.bits[i]|=x.bits[i];
	return result;
}

SET SET::operator - (const SET &x) const
{
	SET result;
	int i;
	DWORD s1, s2;
	for (i=0;i<SETSZ;i++)
	{
		s1 = bits[i];
		s2 = ~(x.bits[i]);
		result.bits[i]=s1&s2;
	}
	return result;
}

SET SET::UNION(const SET&S) const
{
	SET result;
	result = *this;
	int i;
	for (i=0;i<SETSZ;i++)
		result.bits[i]|=S.bits[i];
	return result;
}

SET SET::INTERSECT(const SET&S) const
{
	SET result;
	result = *this;
	int i;
	for (i=0;i<SETSZ;i++)
		result.bits[i]&=S.bits[i];
	return result;
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

SETOFSYS &SETOFSYS::operator = (const SET &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
	return (*this);
}

void SETOFSYS::debug_list (char *str1) const
{
	int n;
	char *str2;
	size_t sz = SETSZ*sizeof(DWORD)*8; 
	WRITE(OUTPUT,"SET ",str1,": (");
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

SETOFIDS &SETOFIDS::operator = (const SET &x)
{
	int i;
	for (i=0;i<SETSZ;i++)
		bits[i]=x.bits[i];
	return (*this);
}


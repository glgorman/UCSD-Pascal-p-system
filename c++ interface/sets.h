
//////////////////////////////////////////////////////
//
//	sets.h - Copyright 2021 Gerold lee Gorman
//
//	This is free software.  Made availaible under
//	the LGPL library license or the MIT license
//	UCSD Pascal style implementation of sets	
//	but written in C++!
//
//////////////////////////////////////////////////////

#define MAX_SET (256)
//#define	SETSZ (MAX_SET>>5)

#define SETOFCHAR SET<256>

template<size_t SETSIZE>
class _SET_
{
protected:
	static const int SETSZ = (SETSIZE+(8*sizeof(DWORD)-1))/(8*sizeof(DWORD));
	DWORD bits[SETSZ];
};

template<size_t SETSIZE>
class SET: public _SET_<SETSIZE>
{
friend class SETOFSYS;
friend class SETOFIDS;
public:
	SET();
	SET(size_t,...);
	inline bool in(int s) const
	{
		bool result;
		if ((s>=SETSZ<<5)||(s<0))
			return false;
		int i, j;
		i = s>>5;
		j = s&0x1f;
		result = (((bits[i])&(0x01<<j))!=0?true:false);
		return result;
	};
	bool operator == (const SET &) const;
	bool operator != (const SET &) const;
	virtual SET UNION(const SET&) const;
	virtual SET INTERSECT(const SET&) const;
	virtual SET operator + (const SET &) const;
	virtual SET operator - (const SET &) const;
};

namespace chartypes
{
	extern SETOFCHAR ident;
	extern SETOFCHAR digits;
	extern SETOFCHAR whitespace;
	extern SETOFCHAR alpha;
	extern SETOFCHAR punct1;
	extern SETOFCHAR operat;
}

class SETOFPRIMES: public SET<65536> 
{
public:
	SETOFPRIMES();
	SETOFPRIMES(const SET&);
	SETOFPRIMES(size_t,...);
	SETOFPRIMES &operator = (const SETOFSYS&);
	SETOFPRIMES operator + (const SETOFSYS&) const;
	SETOFPRIMES &operator += (int);
	SETOFPRIMES &operator -= (int);
	SETOFPRIMES operator + (int) const;
	static SETOFPRIMES range (int low, int high);
	void debug_list (char *str="") const;
};

class SETOFSYS: public SET<128> 
{
public:
	SETOFSYS();
	SETOFSYS(const SET&);
	SETOFSYS(size_t,...);
	SETOFSYS &operator = (const SETOFSYS&);
	SETOFSYS operator + (const SETOFSYS&) const;
	SETOFSYS &operator += (int);
	SETOFSYS &operator -= (int);
	SETOFSYS operator + (int) const;
	static SETOFSYS range (int low, int high);
	void debug_list (char *str="") const;
};

class SETOFIDS: public SET<128>
{
public:
	SETOFIDS();
	SETOFIDS(const SETOFIDS&);
	SETOFIDS(size_t,...);
	SETOFIDS &operator = (const SETOFIDS&);
	SETOFIDS operator + (const SETOFIDS&) const;
	SETOFIDS operator + (int) const;
	SETOFSYS &operator += (int);
	void debug_list (char *str="") const;
};

template<size_t SETSIZE>
SET<SETSIZE>::SET(size_t n, ...)
{
	size_t i,j,k;
	int val;
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

template<size_t SETSIZE>
SET<SETSIZE>::SET()
{
	memset(bits,0,sizeof(DWORD)*SETSZ);
}

template<size_t SETSIZE>
bool SET<SETSIZE>::operator == (const SET &x) const
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

template<size_t SETSIZE>
bool SET<SETSIZE>::operator != (const SET &x) const
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

template<size_t SETSIZE>
SET<SETSIZE> SET<SETSIZE>::operator + (const SET &x) const
{
	SET result;
	result = *this;
	int i;
	for (i=0;i<SETSZ;i++)
		result.bits[i]|=x.bits[i];
	return result;
}

template<size_t SETSIZE>
SET<SETSIZE> SET<SETSIZE>::operator - (const SET &x) const
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

template<size_t SETSIZE>
SET<SETSIZE> SET<SETSIZE>::UNION(const SET&S) const
{
	SET result;
	result = *this;
	int i;
	for (i=0;i<SETSZ;i++)
		result.bits[i]|=S.bits[i];
	return result;
}

template<size_t SETSIZE>
SET<SETSIZE> SET<SETSIZE>::INTERSECT(const SET&S) const
{
	SET result;
	result = *this;
	int i;
	for (i=0;i<SETSZ;i++)
		result.bits[i]&=S.bits[i];
	return result;
}

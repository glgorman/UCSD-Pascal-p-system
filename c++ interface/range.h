
//////////////////////////////////////////////////////
//
//	range.h - Copyright 2021 Gerold lee Gorman
//
//	This is free software.  Made availaible under
//	the LGPL library license or the MIT license
//	
//
/////////////////////////////////////////////////////

#define BITSPERCHR	8
#define	DISPLIMIT	12
#define MAXADDR		65535
#define MAXJTAB		24
#define MAXLEVEL	8
#define MAXPROCNUM	255
#define MAXSEG		15
#define MAXOPCODE	255

template <size_t MIN, size_t MAX>
class RANGE
{
protected:
	int val;

public:
#if 0
	RANGE() {};
	RANGE(int min, int max);
#endif

	inline RANGE &operator = (int);
	inline RANGE operator + (int);
	inline RANGE operator - (int);
	inline RANGE &operator ++ (int);
	inline RANGE &operator -- (int);
	inline operator int ()
	{
		return val;
	}
	inline bool operator == (int);
	inline bool operator != (int);
	inline bool operator >= (int);
	inline bool operator <= (int);
};

template <size_t MIN, size_t MAX>
RANGE<MIN,MAX> &RANGE<MIN,MAX>::operator = (int arg)
{
	val = arg;
	ASSERT(val>=MIN);
	ASSERT(val<=MAX);
	return (*this);
}

template <size_t MIN, size_t MAX>
RANGE<MIN,MAX> &RANGE<MIN,MAX>::operator ++ (int)
{
	val++;
	ASSERT(val<=MAX);
	return (*this);
}

template <size_t MIN, size_t MAX>
RANGE<MIN,MAX> &RANGE<MIN,MAX>::operator -- (int)
{
	ASSERT(val>MIN);
	val--;
	return (*this);
}

template <size_t MIN, size_t MAX>
RANGE<MIN,MAX> RANGE<MIN,MAX>::operator + (int arg)
{
	RANGE result;
	result.val=val+arg;
	ASSERT(result.val<=MAX);
	return result;
}

template <size_t MIN, size_t MAX>
RANGE<MIN,MAX> RANGE<MIN,MAX>::operator - (int arg)
{
	RANGE result;
	result.val=val-arg;
	ASSERT(result.val>=MIN);
	return result;
}

template <size_t MIN, size_t MAX>
bool RANGE<MIN,MAX>::operator == (int arg)
{
	bool result;
	if (val==arg)
		result=true;
	else
		result=false;
	return result;
}

template <size_t MIN, size_t MAX>
bool RANGE<MIN,MAX>::operator != (int arg)
{
	bool result;
	if (val!=arg)
		result=true;
	else
		result=false;
	return result;
}

template <size_t MIN, size_t MAX>
bool RANGE<MIN,MAX>::operator>=(int arg)
{
	bool result;
	if (val>=arg)
		result=true;
	else
		result=false;
	return result;
}

template <size_t MIN, size_t MAX>
bool RANGE<MIN,MAX>::operator<=(int arg)
{
	bool result;
	if (val<=arg)
		result=true;
	else
		result=false;
	return result;
}

class OPRANGE: public RANGE<0,MAXOPCODE>
{
protected:
//	static int min, max;

public:
	OPRANGE ();
	OPRANGE (int);
	OPRANGE &operator = (int);
	OPRANGE operator + (int);
};

class BITRANGE: public RANGE<0,BITSPERCHR>
{
public:
	BITRANGE &operator = (int);
};

class CURSRANGE: public RANGE<0,65535>
{
public:
	CURSRANGE &operator = (int);
};

class JTABRANGE: public RANGE<0,MAXJTAB>
{
public:
	JTABRANGE &operator = (int);
};

class DISPRANGE: public RANGE<0,DISPLIMIT>
{
public:
	DISPRANGE &operator = (int);
	DISPRANGE &operator --(int)
	{
		val--;
		return (*this);
	}
};

class ADDRRANGE: public RANGE<0,MAXADDR>
{
public:
	ADDRRANGE &operator = (int);
};

class PROCRANGE: public RANGE<0,MAXPROCNUM>
{
public:
	PROCRANGE &operator = (int);
};

class SEGRANGE: public RANGE<0,MAXSEG>
{
public:
	SEGRANGE &operator = (int);
};

class LEVRANGE: public RANGE<0,MAXLEVEL>
{
public:
	LEVRANGE &operator = (int);
};

//////////////////////////////////////////////////////
//
//	range.h - Copyright 2021 Gerold lee Gorman
//
//	This is free software.  Made availaible under
//	the LGPL library license or the MIT license
//	
//

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

	RANGE &operator = (int);
	RANGE operator + (int);
	RANGE operator - (int);
	RANGE &operator ++ (int);
	RANGE &operator -- (int);
	inline operator int ()
	{
		return val;
	}
	bool operator == (int);
	bool operator <= (int);
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
bool RANGE<MIN,MAX>::operator<=(int arg)
{
	bool result;
	if (val<=arg)
		result=true;
	else
		result=false;
	return result;
}

class OPRANGE: public RANGE<0,255>
{
protected:
//	static int min, max;

public:
	OPRANGE ();
	OPRANGE (int);
	OPRANGE &operator = (int);
	OPRANGE operator + (int);
};

class CURSRANGE: public RANGE<0,65535>
{
protected:
//	static int min, max;

public:
	CURSRANGE &operator = (int);
};

class JTABRANGE: public RANGE<0,65535>
{
protected:
//	static int min, max;

public:
	JTABRANGE &operator = (int);
};

class DISPRANGE: public RANGE<0,65535>
{
protected:
//	static int min, max;

public:
	DISPRANGE &operator = (int);
	DISPRANGE &operator --(int)
	{
		val--;
		return (*this);
	}// post fix
};

class ADDRRANGE: public RANGE<0,65535>
{
protected:
//	static int min, max;

public:
	ADDRRANGE &operator = (int);
};

class PROCRANGE: public RANGE<0,65535>
{
protected:
//	static int min, max;

public:
	PROCRANGE &operator = (int);
};

class SEGRANGE: public RANGE<0,65535>
{
protected:
//	static int min, max;

public:
	SEGRANGE &operator = (int);
};

class LEVRANGE: public RANGE<0,65535>
{
protected:
//	static int min, max;

public:
	LEVRANGE &operator = (int);
};

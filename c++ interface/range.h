
//////////////////////////////////////////////////////
//
//	range.h - Copyright 2021 Gerold lee Gorman
//
//	This is free software.  Made availaible under
//	the LGPL library license or the MIT license
//	
//

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

class OPRANGE: public RANGE
{
protected:
	static int min, max;

public:
	OPRANGE ();
	OPRANGE (int);
	OPRANGE &operator = (int);
	OPRANGE operator + (int);
};

class CURSRANGE: public RANGE
{
protected:
	static int min, max;

public:
	CURSRANGE &operator = (int);
};

class JTABRANGE: public RANGE
{
protected:
	static int min, max;

public:
	JTABRANGE &operator = (int);
};

class DISPRANGE: public RANGE
{
protected:
	static int min, max;

public:
	RANGE &operator = (int);
	RANGE &operator --(int)
	{
		val--;
		return (*this);
	}// post fix
};

class ADDRRANGE: public RANGE
{
protected:
	static int min, max;

public:
	ADDRRANGE &operator = (int);
};

class PROCRANGE: public RANGE
{
protected:
	static int min, max;

public:
	RANGE &operator = (int);
};

class SEGRANGE: public RANGE
{
protected:
	static int min, max;

public:
	SEGRANGE &operator = (int);
};

class LEVRANGE: public RANGE
{
protected:
	static int min, max;

public:
	RANGE &operator = (int);
};

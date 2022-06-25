
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

	RANGE &operator = (const int arg);
	RANGE operator + (const int arg);
	RANGE &operator ++ (int);
	operator int ();
	bool operator == (int arg);
	bool operator <= (int arg);
};

class OPRANGE: public RANGE
{
protected:
	static int min, max;

public:
	OPRANGE &operator = (const int &OPRANGE);
};

class CURSRANGE: public RANGE
{
protected:
	static int min, max;

public:
	CURSRANGE &operator = (const int &CURSRANGE);
};

class JTABRANGE: public RANGE
{
protected:
	static int min, max;

public:
	JTABRANGE &operator = (const int &JTABRANGE);
};

class DISPRANGE: public RANGE
{
protected:
	static int min, max;

public:
	RANGE &operator = (const int &arg);
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
	ADDRRANGE &operator = (const int &ADDRRANGE);
};

class PROCRANGE: public RANGE
{
protected:
	static int min, max;

public:
	RANGE &operator = (const int &arg);
};

class SEGRANGE: public RANGE
{
protected:
	static int min, max;

public:
	SEGRANGE &operator = (const int &SEGRANGE);
};

class LEVRANGE: public RANGE
{
protected:
	static int min, max;

public:
	RANGE &operator = (const int &arg);
};

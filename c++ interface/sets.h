
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
#define	SETSZ (MAX_SET>>5)

class SET 
{
friend class SETOFSYS;
friend class SETOFIDS;
protected:
	DWORD bits[SETSZ];

public:
	inline bool in(int s)
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
	};
	virtual SET &UNION(const SET&);
	virtual SET &INTERSECT(const SET&);
	virtual SET &operator + (const SET &);
	virtual SET &operator - (const SET &);
	SET();
	SET(int,...);
};

namespace chartype
{
	extern SET symbol;
	extern SET digits;
	extern	SET whitespace;
	extern	SET alpha;
	extern	SET punct1;
	extern	SET operat;
}

class SETOFSYS: public SET 
{
public:
	SETOFSYS &operator + (const int&);
	SETOFSYS &operator + (const SETOFSYS&);
	SETOFSYS &operator = (const SET&);
	SETOFSYS();
	SETOFSYS(const SET&);
};

class SETOFIDS: public SET 
{
public:
	SETOFIDS &operator = (const SET &);
	SETOFIDS();
	SETOFIDS(int,...);
};

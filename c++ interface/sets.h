
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
	inline bool in(int s) const
	{
		bool result;
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
	SET();
	SET(int,...);
};

namespace chartype
{
	extern SET ident;
	extern SET digits;
	extern	SET whitespace;
	extern	SET alpha;
	extern	SET punct1;
	extern	SET operat;
}

class SETOFSYS: public SET 
{
public:
	SETOFSYS operator + (int) const;
	SETOFSYS operator + (const SETOFSYS&) const;
	SETOFSYS &operator += (int);
	SETOFSYS &operator = (const SET&);
	SETOFSYS();
	SETOFSYS(const SET&);
	void debug_list (char *str="") const;
};

class SETOFIDS: public SET 
{
public:
	SETOFIDS operator + (int) const;
	SETOFSYS &operator += (int);
	SETOFIDS &operator = (const SET &);
	SETOFIDS();
	SETOFIDS(int,...);
};

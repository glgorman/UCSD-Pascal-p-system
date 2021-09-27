
//////////////////////////////////////////////////////
//
//	sets.h - Copyright 2021 Gerold lee Gorman
//
//	This is free software.  Made availaible under
//	the LGPL library license or the MIT license
//	UCSD Pascal style implementation of sets	
//	but written in C++!
//

class SET 
{
friend class SETOFSYS;
friend class SETOFIDS;
protected:
	DWORD bits[16];

public:
	bool in(int s);
	virtual SET &UNION(const SET&);
	virtual SET &INTERSECT(const SET&);
	virtual SET &operator + (const SET &);
	virtual SET &operator - (const SET &);
	SET();
	SET(int,...);
};

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

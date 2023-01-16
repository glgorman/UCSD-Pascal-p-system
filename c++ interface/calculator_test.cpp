
#include "stdafx.h"
#include "math.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "expression.h"
#include "calculator_test.h"

class silly_rabbit
{
public:
	char *val;
};

class real
{
protected:
	union
	{
		int		i;
		DWORD	dw;
		struct
		{
			unsigned int  f:23;
			unsigned int  e:8;
			unsigned int  s:1;
		};
	};
	short	sign();
	short	exp();
	int		frac();

	static real add(real,real);
	static real sub(real,real);

public:
	bool operator ==(real arg);
	bool operator !=(real arg);
	bool operator >=(real arg);
	bool operator <=(real arg);
	bool operator >(real arg);
	bool operator <(real arg);
	real operator +(real arg);
	real operator -(real arg);
	real operator *(real arg);
	real operator /(real arg);
	real operator =(real arg);
	real () { dw = 0; }
	real (float);
	operator float ();
	operator silly_rabbit ();
};

ftable calculator::ftab =
{
  &calculator::fadd,
  &calculator::fsub,
  &calculator::fmult,
  &calculator::fdiv,
};

real::real (float arg)
{
	dw = *((DWORD*)&arg);
}

real real::operator = (real arg)
{
	dw = arg.dw;
	return *this;
}

bool real::operator == (real arg)
{
	bool result = (arg.dw==dw?true:false);
	return result;
}

bool real::operator != (real arg)
{
	bool result = (dw==arg.dw?false:true);
	return result;
}

bool real::operator>=(real arg)
{
	bool result = (i>=arg.i?true:false);
	return result;
}

bool real::operator<=(real arg)
{
	bool result = (i<arg.i?true:false);
	return result;
}

bool real::operator>(real arg)
{
	bool result = (i>arg.i?true:false);
	return result;
}

bool real::operator<(real arg)
{
	bool result = (i<arg.i?true:false);
	return result;
}

real::operator float ()
{
	float result = *((float*)(&dw));
	return result;
}

short real::exp()
{
	short result;
	result = (short)((dw&0x7f800000)>>23);
	return result;
}

short real::sign()
{
	short result;
	result = ((dw&(0x1<<31))==0?0:1);
	return result;
}

int real::frac()
{
	int result;
	if (dw==0)
		return 0;
	result = (dw&0x7fffff);
	if (e>0)
		result |= (0x800000);
	return result;
}
// FIXME - figuure out how to 
// handle underflow when handling
// values of different signs.
real real::add (real arg1, real arg2)
{
	real v1, v2, result;
	int frac1, frac2, frac3, expr;
	int d1 = arg1.e-arg2.e;
	if (d1<0) {
		v1 = arg2;
		v2 = arg1;
	}
	else {
		v1 = arg1;
		v2 = arg2;
	}
	frac1 = v1.f;
	frac2 = ((1<<23)+v2.f)>>abs(d1);
	frac3 = frac1+frac2;
	expr = v1.e;
	if (frac3>0x007FFFFF)
	{
		frac3 = ((frac3+(1<<23))>>1);
		expr++;
	}
	result.dw = (frac3&0x007FFFFF)|(expr<<23)|(arg1.s<<31);
	return result;
}

real real::sub (real arg1, real arg2)
{
	real result;
	DWORD abs1, abs2, max, min;
	unsigned short exp1, exp2, adjust, expr;
	unsigned int frac1, frac2, frac3;
	bool sign1, sign2, sr, sflip;
//	break off the sign bit into two bools
	sign1 = ((*((DWORD*)(&arg1)))&(0x1<<31)?true:false);
	sign2 = ((*((DWORD*)(&arg2)))&(0x1<<31)?true:false);
//	strip off the sign bit of both numbers
	abs1 = (*((DWORD*)(&arg1)))&0x7fffffff;
	abs2 = (*((DWORD*)(&arg2)))&0x7fffffff;
	if (abs1>abs2)
	{
		max = abs1;
		min = abs2;
		sflip = false;
	}
	else
	{
		max = abs2;
		min = abs1;
		sflip = true;
	}
	sr = arg1.s^sflip;
//	now extract the respective exponents
	exp1 = (unsigned short)((max&0x7f800000)>>23);
	exp2 = (unsigned short)((min&0x7f800000)>>23);
//	finally get the fractional parts
	frac1 = (1<<23)|(max&0x7fffff);
	frac2 = (1<<23)|(min&0x7fffff);
	adjust = exp1-exp2;
	expr = exp1;
	frac2>>=adjust;
	frac3 = frac1-frac2;
	if (frac3==0)
	{
		return 0;
	}
	while (frac3<1<<23)
	{
		frac3<<=1;
		expr--;
	}
	result.dw = (frac3&0x007FFFFF)|(expr<<23)|(sr<<31);
	return result;
}

real real::operator + (real arg)
{
	real result;
	if (*((DWORD*)(&arg))==0)
		return (*this);
	if (*((DWORD*)(this))==0)
		return arg;
	if (s==arg.s)
		result = add(*this,arg);
	else
		result = sub(*this,arg);	
	return result;
}

real real::operator - (real arg)
{
	real result;
	if (*((DWORD*)(&arg))==0)
		return (*this);
	if (*((DWORD*)(this))==0)
	{
		result = arg;
		result.s=~result.s;
		return result;
	}
	if (s==arg.s)
		result = sub(*this,arg);
	else
		result = add(*this,arg);
	return result;
}

real real::operator / (real arg)
{
	real result;
	return result;
}

real real::operator * (real arg)
{
	short _sign;
	real result;
	short exp1 = this->exp()+arg.exp()-127;
	unsigned int frac1, frac2, frac3, frac4, fracr;
	unsigned short s1, s2;
	unsigned char c1, c2;
	_sign = this->s^arg.s;
	frac1 = this->f;
	frac2 = arg.f;
	s1 = (frac1>>7);
	s2 = (frac2>>7);
	c1 = (frac1&0x7f);
	c2 = (frac2&0x7f);
	frac3 = (c1*s2+c2*s1)>>16;
	frac4 = (s1*s2)>>9;
	fracr = frac1+frac2+frac3+frac4;
	if (fracr>0x007FFFFF)
	{
		fracr = ((fracr+(1<<23))>>1);
		exp1++;
	}
	result.dw = (fracr&0x007FFFFF)|(exp1<<23)|(_sign<<31);
	return result;
}


double calculator::fadd(const double &arg1, const double &arg2)
{
	double result;
	result = arg1+arg2;
	return result;
}

double calculator::fsub(const double &arg1, const double &arg2)
{
	double result;
	result = arg1-arg2;
	return result;
}

double calculator::fmult(const double &arg1, const double &arg2)
{
 	double result;
	result = arg1*arg2;
	return result;
} 

double calculator::fdiv(const double &arg1, const double &arg2)
{
	double result;
	result = arg1/arg2;
	return result;
}

double calculator::exec(opcode op, const double &arg1, const double &arg2)
{ 
	double result;
	result = (ftab[op])(arg1,arg2);
	return result;
}

int calculator::main()
{
	double val1, val2;
	val1 = exec(mult,2,PI);
	val2 = exec(mult,exec(add,3,4),exec(add,4,6));
	int i, j;
	float fi, fj, fs, fs1;
	real ri, rj, rs;

	// If you have read old Pascal code, you will
	// get this.  So weird.  So cool.  Be very afraid.
	silly_rabbit trix;
	trix = (silly_rabbit) ri;

	for (i=-256;i<256;i++)
	for (j=-256;j<256;j++)
	{
		fi = i;
		fj = j;
		fs = fi+fj;
		ri = fi;
		rj = fj;
		rs = ri+rj;
		fs1 = rs;
		ASSERT(fs1==fs);
	}
	real x1,x2,x3,x4,y0;;
	x1 = 3.0;
	x2 = 4.0;
	x3 = 15.0;
	x4 = 6.0;
	y0 = (x1+x2)*(x4-x3);
	float y1 = y0;
	
	
  return 0;
}











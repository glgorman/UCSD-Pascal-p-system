
#define MATH_TYPE float

namespace FRACTIONS
{
//class wordList;
//class text_object;
char *long2Text (long argument);

class fraction
{
private:
	static long GCF (long M, long N);
	static void reduce (fraction &arg);
	static void signadjust (fraction &arg);

public:
	fraction ();
	fraction (int);
	fraction(int, int);
	fraction (MATH_TYPE);
	bool operator != (int arg);
	long numerator;
	long denominator;

	fraction operator * (int arg);
	fraction operator * (MATH_TYPE arg);
	fraction operator + (fraction arg);
	fraction operator - (fraction arg);
	fraction operator * (fraction arg);
	fraction operator % (fraction arg);
	fraction operator / (fraction arg);
	fraction operator / (int arg);
	fraction &operator += (fraction arg);
	fraction &operator -= (fraction arg);
	fraction &operator - (void);

	bool operator<(fraction arg);
	bool operator>(fraction arg);
	bool operator<(int arg);
	bool operator>(int arg); 

	operator MATH_TYPE ();
	fraction &operator = (fraction &arg);
	long operator = (long arg);
};

class rational
{
public:
	int mantissa, numerator, denominator;
	rational (fraction);
};

typedef enum operation
{	
	add,subtract,multiply,divide,
	modulus,symbol,none,left,right,null

} operation;

class EXPRESSION
{
	char *algStr,*polStr;
};

class EQUATIOM
{
	EXPRESSION *expr1, *expr2;
};

class mathObj
{
	text_object *result;

private:
	void put_to_polish (char the_token);
	int lex_level, output_count;

public:
	mathObj ();
	~mathObj ();
	operation detect (char *theToken);
	void push (operation &the);
	fraction evaluate (text_object program);
	fraction calculate (fraction arg1, fraction arg2, operation opCode);
	text_object alg2polish (text_object source);
	text_object text2numeric (text_object source);
};
}
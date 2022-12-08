
#include "stdafx.h"
//#include <cstring>
#include "math.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "expression.h"

using namespace FRACTIONS;

#if 0
bool isNum (char theChar)
{
	if (theChar>='0'&&theChar<='9')
		return true;
	else
		return false;
}
#endif

char *hyphen = "-";

char *numText1 [] =
{
	"zero","one","two","three","four","five",
	"six","seven","eight","nine","ten","eleven",
	"twelve","thirteen","fourteen","fifteen",
	"sixteen","seventeen","eighteen","nineteen"
	"twenty"
};

char *words[] = 
{
	"zero","one","two","three","four","five","six","seven",
	"eight","nine","ten","eleven","twelve","thir","four","fif",
	"six","seven","eigh","nine","twenty","ty","and","negative ","-"
};

char *magnifier[] = 
{
	"-hundred","","-thousand","-million","-billion"
};

char *long2Text (long argument)
{
	size_t length;
	char scratch [256], text [256], *result;
	long group1, these_three_digits, h_digit,
		t_digit, u_digit, triplet_label;

	strcpy_s (text,"");
	strcpy_s (scratch,"");	
	if (argument>0)
		group1 = argument;
	else group1 = -argument;	 
	triplet_label = 1;	
	while (group1>0)
	{
		these_three_digits = group1%1000;
		group1 = group1/1000;
		if (these_three_digits!=0)  {
			h_digit = these_three_digits/100;
			t_digit = (these_three_digits%100)/10;
			u_digit = (these_three_digits%10);
		
			if (h_digit!=0) {
				strncat_s(scratch,words[h_digit],strlen(words[h_digit]));
				strncat_s(scratch,magnifier[0],strlen(magnifier[0]));
				if ((t_digit!=0)||(u_digit!=0))
					switch (triplet_label) {
						case 1: case 3: strncat_s(scratch," and ",5);
										break;		
						case 2: case 4: strncat_s(scratch," ",1);
										break;	}				
			}
			if (t_digit!=0) {
				switch (t_digit)
				case 1:
				{	strncat_s(scratch,words[these_three_digits%100],
					strlen(words[these_three_digits%100]));
					if (u_digit>2)
						strncat_s(scratch,"teen",4);
					break;	
				case 2:
					strncat_s(scratch,words[20],strlen(words[20]));
					break;	
				default:
					strncat_s(scratch,words[t_digit+10],
						strlen(words[t_digit+10]));
					strncat_s(scratch,words[21],strlen(words[21]));
					break;}
				if (((t_digit!=1)&&(u_digit!=0)))
					strncat_s(scratch,hyphen,1);
			}
			if (u_digit!=0) {
				switch (t_digit)
				case 1:
				{	break;		
				default:
					strncat_s(scratch,words[u_digit],strlen(words[u_digit]));
					break;	}
			}
		}
		strncat_s(scratch, magnifier[triplet_label],
			strlen(magnifier[triplet_label]));
			
		if ((triplet_label>1)&&(these_three_digits>99))
			strncat_s(scratch,", ",2);
		else
			strncat_s(scratch," ",1);
			
		triplet_label++;
		strncat_s(scratch, text, strlen(text));
		strcpy_s(text,scratch);
		strcpy_s(scratch,"");
	}
	if (argument==0) {
		strncat_s(scratch,words[0],strlen(words[0]));
		strncat_s(scratch, text, strlen(text));
		strcpy_s(text,scratch);
	}
	if (argument<0) {
		strncat_s(scratch,words[23],strlen(words[23]));
		strncat_s(scratch, text, strlen(text));
		strcpy_s(text,scratch);
	}
	length = strlen(text);
	result = new char [length+1];
	strcpy_s (result,length+1,text);
	if (result [length-1]==' ')
		result [length-1]='\0';
	return result;
}

math_object::math_object ()
{
	result = new text_object;
}

math_object::~math_object ()
{
	delete result;
}

operation math_object::detect (char *theToken)
{
	operation result;
	if (theToken==NULL)
		result = null;
	else if (strcmp (theToken,"+")==0)
		result = add;
	else if (strcmp (theToken,"-")==0)
		result = subtract;
	else if (strcmp (theToken,"*")==0)
		result = multiply;
	else if (strcmp (theToken,"/")==0)
		result = divide;
	else if (strcmp (theToken,"%")==0)
		result = modulus;
	else if ((strcmp (theToken,"[")==0)
		||(strcmp (theToken,"(")==0))
		result = left;
	else if ((strcmp (theToken,"]")==0)
		||(strcmp (theToken,")")==0))
		result = right;
	else result = symbol;
	return result;
}

fraction math_object::calculate (fraction arg1, fraction arg2, operation opCode)
{
	fraction result;
	if (opCode==add)
		result = arg1 + arg2;
	if (opCode==subtract)
		result = arg1 - arg2;
	if (opCode==multiply)
		result = arg1 * arg2;
	if ((opCode==divide)&&(arg2!=0))
		result = arg1/arg2;
	if (opCode==modulus)
		result = arg1%arg2;
	return result;
}

fraction math_object::evaluate (text_object program)
{
	char *ascii;
	fraction stack [8];
	fraction arg1, arg2, result = 0;
	int n = 0;
	operation token; 
	program.rewind ();
	while (program.m_bEnd==false) {
		program.get (ascii);
		token = detect (ascii);
		if (token==symbol) {
			stack [n] = atol (ascii);
			n++; }
		else if (n>1) {
			arg2 = stack [n-1];
			arg1 = stack [n-2];
			result = calculate (arg1,arg2,token);
			stack [n-2] = result;
			n--; } }
	result = stack[0];
	return result;
}

void math_object::push (operation &opcode)
{
	if (opcode==add)
		result->append ("+");
	else if (opcode==subtract)
		result->append ("-");
	else if (opcode==multiply)
		result->append ("*");
	else if (opcode==divide)
		result->append ("/");
	else if (opcode==left)
		result->append ("(");
	else if (opcode==right)
		result->append (")");

	opcode = null;
}

text_object  math_object::alg2polish (text_object theInput)
{
	char *theToken;
	operation pending, deferred, token;
	bool set_exit = false;	
	pending = null;
	deferred = null;
	
// Start scanning the algebraic expression 
	
	theInput.rewind ();
	theInput.get (theToken);
	token = detect (theToken);
 
//	handle negative number at beginning of
//	expression by subtracting from zero!

	if (token==subtract) {
			result->append ("0");
			pending=subtract; }

	while (theToken!=NULL)
	{
//	handle parenthesis by finding right
//	parenthesis and extracting resultant
//	object

		if (token==left) {
			text_object temp;
			while (token!=right) {
				theInput.get (theToken);
				if (theToken==NULL)
					break;
				token = detect (theToken);
				if (token!=right)
					temp.append (theToken); }
			alg2polish (temp);
		}
		else if (token==symbol)
			result->append  (theToken);

//	multiplication and divisions are carried out
//	immediately

		else if ((token==multiply)||(token==divide)
			||(token==modulus)) {								
				if ((pending==add)||(pending==subtract))
					deferred = pending;
				else if (pending!=null)
					push (pending);
			pending=token; }

//	addition and subtraction are always deferred
//	until the next argument is obtained

		else if ((token==add)||(token==subtract)) {			
			if (pending!=null)
				push (pending);			
			if (deferred!=null)
				push (deferred);
			pending = token; 
		}
		theInput.get (theToken);
		token = detect (theToken);
		if (token==right)
			break;
	}
	if (pending!=null)
		push (pending);
	if (deferred!=null)
		push (deferred);
	return *result;
}

#if 0
long abs (long arg)
{
	long result;
	if (arg<0)
		result = -arg;
	else
		result = arg;
	return result;
}
#endif

long fraction::GCF (long M, long N)
{
//	TRACE ("Entering GCF M=%d, N=%d\n",M,N);
	long diff, factor;
	if ((N==0)||(M==0))
		return 1;
	if (M<N) {
		diff = N-M;
		N = M;
		M = N+diff; }		
	if ((M==N)||(M%N==0))
		factor = N;
	else {
		diff = M%N;
		factor = GCF (N,diff); }

//	TRACE ("Exiting GCF factor = %d\n",factor);
	return factor;
}

fraction::fraction ()
{
	numerator = 0;
	denominator = 0;
}

fraction::fraction (int arg)
{
	numerator = arg;
	denominator = 1;
}
/*
fraction::fraction (MATH_TYPE arg)
{
	MATH_TYPE i = floor (arg);
	MATH_TYPE f = (arg-i);
	numerator = i*1000 + (long)(f*1000);
	denominator = 1000;
	reduce (*this);
}
*/
bool fraction::operator!=(int arg)
{
	bool result;
	if (numerator!=denominator*arg)
		result=true;
	else
		result=false;
	return result;
}

bool fraction::operator<(fraction arg)
{
	bool result;
	if ((numerator*arg.denominator)<(arg.numerator*denominator))
		result = true;
	else
		result = false;
	return result;
}

bool fraction::operator>(fraction arg)
{
	bool result;
	if ((numerator*arg.denominator)<(arg.numerator*denominator))
		result = true;
	else
		result = false;
	return result;
}

bool fraction::operator>(int arg)
{
	bool result;
	if ((numerator)>(arg*denominator))
		result = true;
	else
		result = false;
	return result;
}

bool fraction::operator<(int arg)
{
	bool result;
	if ((numerator)<(arg*denominator))
		result = true;
	else
		result = false;
	return result;
}

fraction &fraction::operator = (fraction &arg)
{
	numerator = arg.numerator;
	denominator = arg.denominator;
	return *this;
}

fraction &fraction::operator - ()
{
	numerator = -numerator;
	return *this;
}

fraction fraction::operator / (int arg)
{
	fraction result;
	result.numerator = numerator;
	result.denominator /= arg;
	return result;
}

fraction fraction::operator * (int arg)
{
	fraction result;
	result.numerator *= arg;
	result.denominator = denominator;
	return result;
}
/*
fraction fraction::operator * (MATH_TYPE arg)
{
	fraction result;
	result.numerator *= arg;
	result.denominator = denominator;
	return result;
}
*/
long fraction::operator = (long arg)
{
	numerator = arg;
	denominator = 1;
	return arg;
}

fraction::operator MATH_TYPE ()
{
	MATH_TYPE result;
	result = numerator / (MATH_TYPE) denominator;
	return result;
}

void fraction::reduce (fraction &arg)
{
	long factor, x, y;
	x = arg.numerator;
	y = arg.denominator;
	if (y==1 || y==0)
		return;
	factor = GCF(abs(x),abs(y));
	arg.numerator = arg.numerator/factor;
	arg.denominator = arg.denominator/factor;
}

void fraction::signadjust (fraction &arg)
{
	if (arg.denominator<0) {
		arg.numerator = -arg.numerator;
		arg.denominator = -arg.denominator; }
}

fraction fraction::operator%(fraction arg)
{
	fraction result,temp1,temp2;
	long quotient;
	temp1 = *this/arg;
	quotient = temp1.numerator/temp1.denominator;
	temp2 = quotient;
	result = *this-(temp1*temp2);
	return result;
}

fraction fraction::operator+(fraction arg)
{
	fraction result;
	result.numerator = numerator*arg.denominator+
		arg.numerator*denominator;

	result.denominator = denominator*arg.denominator;
	signadjust (result);
	reduce (result);
	return result;
}

fraction fraction::operator-(fraction arg)
{
	fraction result;
	result.numerator = numerator*arg.denominator-
		arg.numerator*denominator;

	result.denominator = denominator*arg.denominator;
	signadjust (result);
	reduce (result);
	return result;
}

fraction &fraction::operator+=(fraction arg)
{
	fraction result;
	result.numerator = numerator*arg.denominator+
		arg.numerator*denominator;

	result.denominator = denominator*arg.denominator;
	signadjust (result);
	reduce (result);
	*this = result;
	return *this;
}

fraction &fraction::operator-=(fraction arg)
{
	fraction result;
	result.numerator = numerator*arg.denominator-
		arg.numerator*denominator;

	result.denominator = denominator*arg.denominator;
	signadjust (result);
	*this = result;
	return *this;
}

fraction fraction::operator*(fraction arg)
{
	fraction result;
	result.numerator = numerator*arg.numerator;
	result.denominator = denominator*arg.denominator;
	signadjust (result);
	reduce (result);
	return result;
}

fraction fraction::operator/(fraction arg)
{
	fraction result;
	result.numerator = numerator*arg.denominator;
	result.denominator = denominator*arg.numerator;
	signadjust (result);
	reduce (result);
	return result;
}

rational::rational (fraction arg)
{
	numerator = denominator = mantissa = 0;
	if (arg.denominator!=0) {
		mantissa = arg.numerator/arg.denominator;
		numerator = arg.numerator%arg.denominator;
		denominator = arg.denominator; }
};

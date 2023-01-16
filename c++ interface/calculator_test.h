
#define PI (3.14159265358979323)
typedef enum { add=0, sub=1, mult=2, divi=3, }  opcode;
typedef double (*ftable[])(const double&, const double&);

class calculator
{
protected:
	double reg[16];
	static double (*ftab[4])(const double&, const double&);
	static double fadd(const double &, const double &);
	static double fsub(const double &, const double &);
	static double fmult(const double &, const double &);
	static double fdiv(const double &, const double &);

public:
	static double exec(opcode, const double &arg1, const double &arag2);
	static int main();
};

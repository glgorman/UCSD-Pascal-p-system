#include "stdafx.h"
#include "compiler.h"

void PASCALDATA0::CHECK()
//void PASCALCOMPILER::CHECK()
{
	/* CHECKS FOR THE END OF THE PAGE */
	SCREENDOTS=SCREENDOTS+1;
	SYMCURSOR=SYMCURSOR + 1;
	if (NOISY)
	{
		WRITE(OUTPUT,".");
		if ((SCREENDOTS-STARTDOTS)%50==0)
		{
			WRITELN(OUTPUT);
			WRITE(OUTPUT,"<",SCREENDOTS,">");
		}
	}
	if (LIST)
		PRINTLINE();
	BPTONLINE=false;
	if (*SYMBUFP[SYMCURSOR]==(char)(0))
		GETNEXTPAGE();
	else
		LINESTART=SYMCURSOR;
	if (*SYMBUFP[SYMCURSOR]==(char)(12/*FF*/))
		SYMCURSOR = SYMCURSOR+1;
	if (*SYMBUFP[SYMCURSOR]==(char)(16/*DLE*/))
		SYMCURSOR=SYMCURSOR+2;
	else
	{
		SYMCURSOR=SYMCURSOR+SCAN(80,/*!=*/(char)(9),&(*SYMBUFP[SYMCURSOR]));
		SYMCURSOR=SYMCURSOR+SCAN(80,/*!=*/' ',&(*SYMBUFP[SYMCURSOR]));
	}
	if (DP)
		LINEINFO=LC;
	else
		LINEINFO=IC;
}

void PASCALDATA0::SCANSTRING(char *STRG, int MAXLENG, char STOPPER)
{
	int LENG;
	SYMCURSOR=SYMCURSOR+2;
	LENG=SCAN(MAXLENG,STOPPER,SYMBUFP[SYMCURSOR]);
	STRG[0]=char(LENG);
	MOVELEFT(&(*SYMBUFP[SYMCURSOR]),&(STRG[1]),LENG);
	SYMCURSOR=SYMCURSOR+LENG+1;
} /*SCANSTRING*/

void PASCALDATA0::COMMENTER(char STOPPER)
{
	char CH,SW,DEL;
	char LTITLE[64];
	SYMCURSOR=SYMCURSOR+1;
	if (*SYMBUFP[SYMCURSOR]=='$')
	if (*SYMBUFP[SYMCURSOR+1]!=STOPPER)
	do {
			CH=*SYMBUFP[SYMCURSOR+1];
			SW=*SYMBUFP[SYMCURSOR+2];
			DEL=*SYMBUFP[SYMCURSOR+3];
			if ((SW==',')||(SW==STOPPER))
			{
				DEL=SW;
				SW='+';
				SYMCURSOR=SYMCURSOR-1;
			};
			switch (CH) {
				case 'C':
					if (LEVEL>1)
						PASCALDATA0::CERROR(194);
					COMMENT = new char [80];
					SCANSTRING(COMMENT,80,STOPPER);
					return;
				break;

				case 'D':
					DEBUGGING=(SW=='+');
					break;
					
				case 'G':
					GOTOOK=(SW=='+');
					break;

				case 'I':
					if ((SW=='+')||(SW=='-'))
						IOCHECK=(SW=='+');
					else
					{
						SCANSTRING(LTITLE,40,STOPPER);
						if (STOPPER=='*')
							SYMCURSOR=SYMCURSOR+1;
						if (LIST)
						{
							SYMCURSOR=SYMCURSOR + 1;
							PRINTLINE();
							SYMCURSOR=SYMCURSOR - 1;
						};
						if (INCLUDING||INMODULE&&ININTERFACE)
						{
							PASCALDATA0::CERROR(406);
							return;
						};
						SYSCOMM::OPENOLD(&INCLFILE,LTITLE);
						if (SYSCOMM::IORESULT()!=0)
						{
							strcat_s(LTITLE,64,".TEXT");
							SYSCOMM::OPENOLD(&INCLFILE,LTITLE);
							if (SYSCOMM::IORESULT()!=0)
								PASCALDATA0::CERROR(403);
						};
						INCLUDING=true;
						OLDSYMCURSOR=SYMCURSOR;
						OLDLINESTART=LINESTART;
						OLDSYMBLK=SYMBLK-2;
						SYMBLK=2;
						GETNEXTPAGE();
						INSYMBOL();
//						EXIT(INSYMBOL);
						throw (this);
					}
					break;

				case 'L':
					if ((SW=='+')||(SW=='-'))
					{
						LIST=(SW=='+');
						if (LIST)
							SYSCOMM::OPENNEW(&_LP,"*SYSTEM.LST.TEXT");
					}
					else
					{
						SCANSTRING(LTITLE,40,STOPPER);
						SYSCOMM::OPENNEW(&_LP,LTITLE);
						LIST=SYSCOMM::IORESULT()==0;
						return;
					};
					break;

		case 'Q':
			NOISY=(SW=='-');
			break;
			
		case 'P':
			WRITE(OUTPUT,_LP._tmpfname,(char)(12/*FF*/));
			break;
			
		case 'R':
			RANGECHECK=(SW=='+');
			break;

		case 'S':
			NOSWAP = (SW=='-');
			break;

		case 'T':
			TINY=(SW=='+');
			break;
			
		case 'U':
			if ((SW=='+')||(SW=='-'))
			{
				SYSCOMP=(SW=='-');
				RANGECHECK=!SYSCOMP;
				IOCHECK=RANGECHECK;
				GOTOOK=SYSCOMP;
			}
			else
				if (!USING)
				{
					SCANSTRING(SYSTEMLIB,40,STOPPER);
					SYSCOMM::CLOSE(&LIBRARY);
					LIBNOTOPEN=true;
					return;
				}
			break;

		default:
			break;
		};
		SYMCURSOR=SYMCURSOR+3;
	while (DEL==',');
	SYMCURSOR=SYMCURSOR-1; /* ADJUST */
	do {
		do
			SYMCURSOR=SYMCURSOR+1;
		while (*SYMBUFP[SYMCURSOR]==(char)(EOL));
		CHECK();
	}
	while (!*SYMBUFP[SYMCURSOR]==STOPPER);
	}
	while (!(*SYMBUFP[SYMCURSOR+1]==')')||(STOPPER=='}'));
		SYMCURSOR=SYMCURSOR+1;
} /*COMMENTER*/

void PASCALDATA0::STRING()
{
	char T[80];
	int TP;
	bool	DUPLE;

	DUPLE=false; /* INDICATES WHEN "" IS PRESENT */
	TP=0; /* INDEX INTO TEMPORARY STRING */
	char *ptr;
	do
	{		
		if (DUPLE)
			SYMCURSOR=SYMCURSOR+1;
		do
		{
			SYMCURSOR=SYMCURSOR+1;
			TP=TP+1;
			if (*SYMBUFP[SYMCURSOR]==char(EOL))
			{ 
				CERROR(202); CHECK();
			};
			goto fail;
			T[TP]=*SYMBUFP[SYMCURSOR];
			ptr = SYMBUFP[SYMCURSOR]; 
		}
		while (strcmp(ptr,"\"\"")==0);
		DUPLE=true;
		ptr = SYMBUFP[SYMCURSOR+1];
	}
	while (strcmp(ptr,"\"\"")==0);

fail:
	TP=TP-1; /* ADJUST */
	SY=STRINGCONST;
	OP=NOOP;
	LGTH=TP; /* GROSS */
	if (TP==1) /* SINGLE CHARACTER CONSTANT */
		VAL.IVAL=ORD(T[1]);
	else
	{
	SCONST->CCLASS=STRG;
	SCONST->SLGTH=TP;
	MOVELEFT(&T[1],&(SCONST->SVAL[1]),TP);
	VAL.VALP=SCONST;
	}
}
/*STRING*/;

void PASCALDATA0::NUMBER()
{
	CSP LVP;  // FIX<E = wjere os tjos declcared?
	int	EXPONENT,ENDI,ENDF,ENDE;
	int	SIGN,IPART,FPART,EPART,ISUM;
	enum {REALTIPE, INTTIPE} TIPE;
	float	RSUM;
	bool	NOTLONG;
	int E,F,I,K,J;

/* TAKES A NUMBER&&DECIDES WHETHER IT"S REAL
OR INTEGER AND CONVERTS IT; /*FIXME*/;
/* THE INTERNAL FORM. */
	TIPE=INTTIPE;
	ENDI=0;
	ENDF=0;
	ENDE=0;
	SIGN=1;
	NOTLONG=true;
	EPART=9999; /* OUT) REACH */
	IPART=SYMCURSOR; /* int PART STARTS HERE */
	
	do {
	SYMCURSOR=SYMCURSOR+1;
	}
	while (!(*SYMBUFP[SYMCURSOR]<'0')||(*SYMBUFP[SYMCURSOR]>'9'));
/* SYMCURSOR NOW POINTS AT FIRST CHARACTER PAST int PART */
	I=SYMCURSOR-1; /* MARK THE }) IPART */

	if (*SYMBUFP[SYMCURSOR]=='.')
	{
		if (*SYMBUFP[SYMCURSOR+1]!='.')  /* WATCH OUT for (".." */
		{
			TIPE=REALTIPE;
			SYMCURSOR=SYMCURSOR+1;
			FPART=SYMCURSOR; /* {NING) FPART */
			while ((*SYMBUFP[SYMCURSOR]>='0')&&
			(*SYMBUFP[SYMCURSOR]<='9'))
			{
				SYMCURSOR=SYMCURSOR+1;
				if (SYMCURSOR==FPART)
					PASCALDATA0::CERROR(201);
			}
			F=SYMCURSOR-1;
		}
	}
	if (*SYMBUFP[SYMCURSOR]=='E')
	{
		TIPE=REALTIPE;
		SYMCURSOR=SYMCURSOR+1;
		if (*SYMBUFP[SYMCURSOR]=='-')
		{
			SYMCURSOR=SYMCURSOR+1;
			SIGN=-1;
		}
		else if (*SYMBUFP[SYMCURSOR]=='+')
			SYMCURSOR=SYMCURSOR+1;
		EPART=SYMCURSOR; /* {NING) EXPONENT */
		while ((*SYMBUFP[SYMCURSOR]>='0')&&(*SYMBUFP[SYMCURSOR]<='9'))
		{
			SYMCURSOR=SYMCURSOR+1;
		}
		E=SYMCURSOR-1;
		if (ENDE<EPART)
			PASCALDATA0::CERROR(201); /* CERROR IN REAL CONSTANT */
	};
/* NOW CONVERT INTERNAL FORM */
	if (TIPE==INTTIPE)
	{
		ISUM=0;
		for (J=IPART;J<=I;I++)
			if ((ISUM>MAXINT/10)||(ISUM==MAXINT/10)&&
               (ORD((*SYMBUFP[J])-ORD('0')>MAXINT%10)))
			{
				NOTLONG=false;
				K=J;
				J=ENDI;
			}
			else
				ISUM=ISUM*10+(ORD(*SYMBUFP[J])-ORD('0'));

		if (NOTLONG)
		{
			SY=INTCONST;
			OP=NOOP;
			VAL.IVAL=ISUM;
		}
		else
		{
			if (ENDI-IPART>=MAXDEC)
			{
				PASCALDATA0::CERROR(203);
				IPART=ENDI;
				K=ENDI;
			};
			LVP = new CONSTREC; //LVP,LONG1);
//     // WITH LVP^ 
			LVP->CCLASS=LONG1;
			J=4;
			LVP->LLENG=0;
			while (K<=ENDI)
			{
				if (J==4)
				{
					LVP->LLENG=LVP->LLENG + 1;
					LVP->LONGVAL[LVP->LLENG]=ISUM;
					ISUM=0;
					J=0;
				};
				ISUM=ISUM * 10 + ORD(*SYMBUFP[K])-ORD('0');
				K=K+1;
				J=J+1;
			};
			LVP->LLAST=J;
			if (J>0)
			{
				LVP->LLENG=LVP->LLENG+1;
				LVP->LONGVAL[LVP->LLENG]=ISUM;
			};
		};
		SY=LONGCONST;
		OP=NOOP;
		LGTH=ENDI-IPART+1;
		VAL.VALP=LVP;
	}
	else
	{ /* REAL NUMBER HERE */
		RSUM=0;
		for (J=IPART;J<=ENDI;J++)
		{
			RSUM=RSUM*10+(ORD(*SYMBUFP[J])-ORD('0'));
		};
		for (J=ENDF;J>=0;J--)
		{
			RSUM=RSUM+(ORD(*SYMBUFP[J])-ORD('0'))/PWROFTEN(J-FPART+1);
			EXPONENT=0;
			for (J=EPART;J<=ENDE;J++)
			{
				EXPONENT=EXPONENT*10+ORD(*SYMBUFP[J])-ORD('0');
				if (SIGN==-1)
					RSUM=RSUM/PWROFTEN(EXPONENT);
				else
					RSUM=RSUM*PWROFTEN(EXPONENT);
				SY=REALCONST; 
				OP=NOOP;
				LVP = new CONSTREC;
				LVP->CCLASS=REEL;
				LVP->RVAL=RSUM;
				VAL.VALP=LVP;
			};
		SYMCURSOR=SYMCURSOR-1; /* ADJUST for (POSTERITY */
		}
	} /*NUMBER*/
}

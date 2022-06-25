#include "stdafx.h"
#include "compiler.h"

//#define DEBUG_INSYMBOL	1
//#define DEBUG_RETRY		1
//#define DEBUG_PEEK0	1
//#define DEBUG_PEEK1	1
//#define DEBUG_GETC	

char PASCALSOURCE::PEEK()
{
	if (SYMCURSOR>1023)
		GETNEXTPAGE();

	char c = (*SYMBUFP)[SYMCURSOR];
	ASSERT(c>=0);

#ifdef DEBUG_PEEK0
	if (c>=32)
		WRITE(OUTPUT,"\nPEEK(void) SYMCURSOR = ",(int)SYMCURSOR," CH = \"",c,"\"");
	else
		WRITE(OUTPUT,"\nPEEK(void) SYMCURSOR = ",(int)SYMCURSOR," CH = \"",c,"\"");
#endif

	return c;
}

char PASCALSOURCE::PEEK(int i)
{
	CURSRANGE POS;
	POS = SYMCURSOR+i;
	char c = (*SYMBUFP)[POS];

#ifdef DEBUG_PEEK1
	if (c>=32)
		WRITE(OUTPUT,"\nPEEK(i) SYMCURSOR = ",(int)SYMCURSOR," CH = \"",c,"\"");
	else
		WRITE(OUTPUT,"\nPEEK(i) SYMCURSOR = ",(int)SYMCURSOR," CH = \"",c,"\"");
#endif

	return c;
}

char PASCALSOURCE::GETC()
{
	char c = (*SYMBUFP)[SYMCURSOR];

#ifdef DEBUG_GETC
	if (c>=32)
		WRITELN(OUTPUT,"GETC(void) '",c,"'");
	else
		WRITELN(OUTPUT,"GETC(void) ",(int)c);
#endif

	SYMCURSOR++;
	return c;
}

void PASCALSOURCE::CERROR(int ERRORNUM)
{
//	int ERRSTART;
	char A[128];
	char CH;
//	// // WITH USERINFO
	if ((USERINFO.ERRSYM!=SYMCURSOR)||(USERINFO.ERRBLK!=SYMBLK))
	{
		USERINFO.ERRBLK=SYMBLK;
		USERINFO.ERRSYM=SYMCURSOR;
		USERINFO.ERRNUM=ERRORNUM;
		if (USERINFO.STUPID)
			CH='E';
		else
		{
			if (NOISY)
				WRITELN(OUTPUT);
			else if (LIST&&(ERRORNUM<=400))
				return;
			if (LINESTART==0)
				WRITE(OUTPUT,*SYMBUFP,(int)SYMCURSOR);
			else
			{
#if 0
				ERRSTART=SCAN(
					-(LINESTART-1),true,
					char(EOL),*(&(SYMBUFP[LINESTART-2])+ (size_t)(LINESTART)-1));
				if (ERRSTART<0)
					ERRSTART=0;
				MOVELEFT(&(*SYMBUFP[ERRSTART]),&A[0],SYMCURSOR-ERRSTART);
				WRITE(OUTPUT,A /*SYMCURSOR-ERRSTART*/ );
#endif
			};
			WRITELN(OUTPUT," <<<<");
			WRITE(OUTPUT,"Line ",SCREENDOTS,", error ",ERRORNUM,":");
			if (NOISY)
				WRITE(OUTPUT," <sp>(continue), <esc>(terminate), E(dit");
			char param[] = ":";
			WRITE(OUTPUT,param);
		}
		do
		{
			SYSCOMM::READ(KEYBOARD,CH);
		}
		while (!
			((CH==' ')||(CH=='E')||(CH=='e')
			||(CH==USERINFO.ALTMODE))
			);
	};
	if ((CH=='E')||(CH=='e'))
	{
		USERINFO.ERRBLK=SYMBLK-2;
		throw(this);
	};
	if ((ERRORNUM>400)||(CH==(char)(27)))
	{
		USERINFO.ERRBLK=0;
#if 0
		throw(this);
#endif
	};
	WRITELN(OUTPUT);
	if (NOISY)
		WRITE(OUTPUT,"<",SCREENDOTS/*4*/,'>');

} /*CERROR*/ ;

void PASCALSOURCE::WRITETEXT()
{
	MOVELEFT(SYMBUFP[SYMCURSOR],CODEP[0],1024);
	if (USERINFO.ERRNUM==0)
       if (SYSCOMM::BLOCKWRITE(USERINFO.WORKCODE,*CODEP,2,CURBLK)!=2)
         CERROR(402);
     CURBLK=CURBLK+2;
} /*WRITETEXT*/ ;

void PASCALSOURCE::GETNEXTPAGE()
{
//	WRITELN(OUTPUT,"\nPASCALCOMPILER::GETNEXTPAGE()");
	SYMCURSOR=0;
	LINESTART=0;
	if (USING)
	{
		if (USEFILE==WORKCODE)
		{
			SYSCOMM::BLOCKREAD(USERINFO.WORKCODE,*SYMBUFP,2,SYMBLK);
			if (SYMBLK!=2)
				USING=false;
		}
		else
			if (USEFILE==SYSLIBRARY)
			{
				SYSCOMM::BLOCKREAD(&LIBRARY,*SYMBUFP,2,SYMBLK);
				if (SYMBLK!=2)
					USING=false;
			}			
		if (!USING)
		{
			SYMBLK=PREVSYMBLK;
			SYMCURSOR=PREVSYMCURSOR;
			LINESTART=PREVLINESTART;
		}
	};
	if (!USING)
	{
		if (INCLUDING)
		{
			SYSCOMM::BLOCKREAD(&INCLFILE,*SYMBUFP,2,SYMBLK);
			if (SYMBLK!=2)
			{
				SYSCOMM::CLOSE((FILE*)(&INCLFILE));
				INCLUDING=false;
				SYMBLK=OLDSYMBLK;
				SYMCURSOR=OLDSYMCURSOR;
				LINESTART=OLDLINESTART;
			}
		}
	};
	if (!(INCLUDING||USING))
	{
		SYSCOMM::BLOCKREAD(USERINFO.WORKSYM,*SYMBUFP,2,SYMBLK);
		if (SYMBLK!=2)
			CERROR(401);
	}
	if (SYMCURSOR==0)
	{
		if (INMODULE)
			if (ININTERFACE&&!USING)
				WRITETEXT();
		if ((*SYMBUFP)[0]==(char)(16))
			SYMCURSOR=2;
		SYMBLK=SYMBLK+2;
	};
}

void PASCALSOURCE::CHECK()
{
//	WRITELN(OUTPUT,"PASCALCOMPILER::CHECK()");
	/* CHECKS FOR THE END OF THE PAGE */
	char ch;
	SCREENDOTS++;
	SYMCURSOR++;
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
	if (PEEK()=='\0')
		GETNEXTPAGE();
	else
		LINESTART=SYMCURSOR;
	if (PEEK()==12/*FF*/)
		SYMCURSOR++;
	if (PEEK()==16/*DLE*/)
		SYMCURSOR=SYMCURSOR+2;
	while(true)
	{
		ch=PEEK();
		if (!chartype::whitespace.in(ch));
	}	
	if (DP)
		LINEINFO=LC;
	else
		LINEINFO=IC;
}

void PASCALSOURCE::SCANSTRING(char *STRG, int MAXLENG, char STOPPER)
{
	int LENG;
	SYMCURSOR=SYMCURSOR+2;
	LENG=SCAN(MAXLENG,true,STOPPER,&((*SYMBUFP)[SYMCURSOR]));
	STRG[0]=char(LENG);
	MOVELEFT(&(*SYMBUFP[SYMCURSOR]),&(STRG[1]),LENG);
	SYMCURSOR=SYMCURSOR+LENG+1;
} /*SCANSTRING*/


void PASCALCOMPILER::COMMENTER(char STOPPER)
{
//	WRITELN(OUTPUT,"PASCALCOMPILER::COMMENTER()");
	char CH,SW,DEL;
	char LTITLE[64];
	SYMCURSOR++;
	if (PEEK()=='$')
	if (PEEK(1)!=STOPPER)
	do {
		CH = PEEK(1);
		SW = PEEK(2);
		DEL= PEEK(3);
		if ((SW==',')||(SW==STOPPER))
		{
			DEL=SW;
			SW='+';
			SYMCURSOR++;
		};
		switch (CH) {
			case 'C':
				if (LEVEL>1)
					PASCALCOMPILER::CERROR(194);
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
						SYMCURSOR++;
					if (LIST)
					{
						SYMCURSOR++;
						PRINTLINE();
						SYMCURSOR=SYMCURSOR - 1;
					};
					if (INCLUDING||INMODULE&&ININTERFACE)
					{
						PASCALCOMPILER::CERROR(406);
						return;
					};
					SYSCOMM::OPENOLD(&INCLFILE,LTITLE);
					if (SYSCOMM::IORESULT()!=0)
					{
						strcat_s(LTITLE,64,".TEXT");
						SYSCOMM::OPENOLD(&INCLFILE,LTITLE);
						if (SYSCOMM::IORESULT()!=0)
							PASCALCOMPILER::CERROR(403);
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
	}
	while (DEL==',');
	do {
		do {		
			while (GETC()==(char)(EOL))
				CHECK();
		}
		while (PEEK()!=STOPPER);
		CH = PEEK(1);
		SYMCURSOR++;
	}
	while ((CH!=')')&&(STOPPER!='}'));
	SYMCURSOR++;
} /*COMMENTER*/

void PASCALSOURCE::STRING()
{
	char ch;
	char T[80];
	int TP,NBLANKS,L;
	bool	DUPLE;
 	T[0] = 0;
	memset((char*)(SCONST->SVAL),0,256);

	DUPLE=false; /* INDICATES WHEN "" IS PRESENT */
	TP=0; /* INDEX INTO TEMPORARY STRING */
	do
	{		
		if (DUPLE)
			SYMCURSOR++;
		do
		{
			SYMCURSOR++;
			TP++;
			if (PEEK()==char(EOL))
			{ 
				CERROR(202);
				CHECK();
				goto fail;
			}
			T[TP]=ch=PEEK();
		}
		while (ch!='\'');
		ch=PEEK(1);
		SYMCURSOR++;
		DUPLE=true;
	}
	while (ch=='\'');

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

void PASCALSOURCE::NUMBER()
{
	CSP LVP;  // FIX<E = wjere os tjos declcared?
	int	EXPONENT,ENDI,ENDF,ENDE;
	int	SIGN,IPART,FPART,EPART,ISUM;
	enum {REALTIPE, INTTIPE} TIPE;
	float	RSUM;
	bool	NOTLONG;
	int K,J;
	char CH;

/* TAKES A NUMBER&& DECIDES WHETHER IT"S REAL
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
	
	do
	{
		SYMCURSOR++;
	}
	while (chartype::digits.in(PEEK()));
	ENDI=SYMCURSOR-1;
	
/* SYMCURSOR NOW POINTS AT FIRST CHARACTER PAST int PART */

	if (PEEK()=='.')
	{
		CH = PEEK(1);
		SYMCURSOR++;
		if (CH!='.')  /* WATCH OUT for (".." */
		{
			TIPE=REALTIPE;
			SYMCURSOR++;
			FPART=SYMCURSOR; /* {NING) FPART */
			while (chartype::digits.in(PEEK()))
			{
				SYMCURSOR++;
				if (SYMCURSOR==FPART)
					PASCALSOURCE::CERROR(201);
			}
			ENDF=SYMCURSOR-1;
		}
	}
	if (PEEK()=='E')
	{
		TIPE=REALTIPE;
		SYMCURSOR++;
		if (PEEK()=='-')
		{
			SYMCURSOR++;
			SIGN=-1;
		}
		else if (PEEK()=='+')
			SYMCURSOR++;
		EPART=SYMCURSOR; /* {NING) EXPONENT */
		while (chartype::digits.in(PEEK()))
		{
			SYMCURSOR++;
		}
		ENDE=SYMCURSOR-1;
		if (ENDE<EPART)
			PASCALSOURCE::CERROR(201); /* CERROR IN REAL CONSTANT */
	}
/* NOW CONVERT INTERNAL FORM */
	if (TIPE==INTTIPE)
	{
		ISUM=0;
		SYMCURSOR=IPART;
		for (J=0;J<=ENDI-IPART;J++)
		{
			char C = PEEK(J);
			int val = ORD(C)-ORD('0'); 
			if ((ISUM>MAXINT/10)||(ISUM==MAXINT/10)&&
               (val>MAXINT%10))
			{
				NOTLONG=false;
				K=J;
				J=ENDI;
			}
			else
				ISUM=ISUM*10+val;
		}
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
				PASCALSOURCE::CERROR(203);
				IPART=ENDI;
				K=ENDI;
			}
			LVP = new CONSTREC; //LVP,LONG1); 
			LVP->CCLASS=LONG1;
			J=4;
			LVP->LLENG=0;
			while (K<=ENDI)
			{
				if (J==4)
				{
					LVP->LLENG++;
  					LVP->LONGVAL[LVP->LLENG]=ISUM;
					ISUM=0;
					J=0;
				};
				ISUM=ISUM * 10 + ORD(PEEK(K))-ORD('0');
				K++;
				J++;
			};
			LVP->LLAST=J;
			if (J>0)
			{
				LVP->LLENG++;
				LVP->LONGVAL[LVP->LLENG]=ISUM;
			};
			SY=LONGCONST;
			OP=NOOP;
			LGTH=ENDI-IPART+1;
			VAL.VALP=LVP;
		}
	}
	else
	{ /* REAL NUMBER HERE */
		RSUM=0;
		for (J=IPART;J<=ENDI;J++)
		{
			RSUM=RSUM*10+(ORD(PEEK(J))-ORD('0'));
		};
		for (J=ENDF;J>=FPART;J--)
		{
			RSUM=RSUM+(ORD(PEEK(J))-ORD('0'))/PWROFTEN(J-FPART+1);
			EXPONENT=0;
			for (K=EPART;K<=ENDE;K++)
			{
				EXPONENT=EXPONENT*10+ORD(PEEK(K))-ORD('0');
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

void PASCALCOMPILER::INSYMBOL() /* COMPILER VERSION 3.4 06-NOV-76 */
{
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"\nPASCALCOMPILER::INSYMBOL() SYMCURSOR = ",(int)SYMCURSOR," ");
#endif
	if (GETSTMTLEV)
	{
		BEGSTMTLEV=STMTLEV;
		GETSTMTLEV=false;
	};
	OP=NOOP;
	goto start;
retry:
#ifdef DEBUG_RETRY
	WRITE(OUTPUT,"\nretry ... SYMCURSOR = ",(int)SYMCURSOR," ");
#endif
start:

	SY=(SYMBOL)-1; /* if (NO CASES EXERCISED BLOW UP */
	char CH = PEEK();

	if (chartype::alpha.in(CH)==true)
	{
		char c;
		int index,len,i;
		key_info *key;
		int val = SYMCURSOR;
		index = SEARCH::IDSEARCH(val,(char*&)(SYMBUFP));
		if (index!=-1)
		{
			key = SEARCH::get_key_info (index);
			ASSERT((key->SY>=0)&&(key->SY<MAXSYMBOL));
			SY = key->SY;
			OP = key->OP;
			len = strlen(key->ID);
			strcpy_s(ID,16,key->ID);
 			SYMCURSOR = SYMCURSOR+len;
#ifdef DEBUG_INSYMBOL
			WRITELN(OUTPUT,"FOUND: \"",ID,"\" ");
#endif
		}
		else
		{
			SY = IDENT;
			OP = NOOP;
	 		ID[0]=GETC();
			for (i=1;i<16;i++)
			{
				c = GETC();
				if (chartype::symbol.in(c))
					ID[i]=c;
				else
					break;
			}
			ID[i]=0;
			SYMCURSOR = SYMCURSOR-1;
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"IDENTIFIER: \"",ID,"\" ");
#endif
		}
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"\nPASCALCOMPILER::INSYMBOL() SYMCURSOR = ",(int)SYMCURSOR," ");
#endif
	}
	else if (chartype::digits.in(CH)==true)
	{
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"\nPASCALCOMPILER::INSYMBOL() SYMCURSOR = ",(int)SYMCURSOR," ");
#endif
		NUMBER();
		SYMCURSOR++;
	}
	else
	{
		switch (CH)
		{
		case '}':
			SY=SEPARATSY;
			break;

		case ';':
			SY=SEMICOLON;
			break;
		
		case (int)'\'':
	#ifdef DEBUG_INSYMBOL
		WRITE(OUTPUT,"\nPASCALCOMPILER::INSYMBOL() SYMCURSOR = ",(int)SYMCURSOR," ");
	#endif
			STRING();
			break;

		case '^':
			SY=ARROW;
			break;

		case '[':
			SY=LBRACK;
			break;
				
		case ']':
			SY=RBRACK;
			break;

		case ')':
			SY=RPARENT;
			break;
			
		case ',':
			SY=COMMA;
			break;

		case '{':
			COMMENTER('}');
			goto retry;
			break;

		case '(':
			if (PEEK(1)=='*')
			{
				COMMENTER('*');
				goto retry; /* GET ANOTHER TOKEN */
			}
			else
				SY=LPARENT;
			break;

		case '.':
			if (PEEK(1)=='.')
			{
				SYMCURSOR++;
				SY=COLON;
			}
			else
				SY=PERIOD;
			break;
			
		case ':':
			if (PEEK(1)=='=')
			{
				SYMCURSOR++;
				SY=BECOMES;
			}
			else
				SY=COLON;
			break;

		case '=':
			SY=RELOP;
			OP=EQOP;
			break;

		case '*':
			SY=MULOP;
			OP=MUL;
			break;

		case '+':
			SY=ADDOP;
			OP=PLUS;
			break;

		case '-':
			SY=ADDOP;
			OP=MINUS;
			break;

		case '/':
			SY=MULOP;
			OP=RDIV;
			break;

		case '<':
			SY=RELOP;
			OP=LTOP;
				switch (PEEK(1))
				{
					case '>':
						OP=NEOP;
						SYMCURSOR++;
						break;

					case '=':
						OP=LEOP;
						SYMCURSOR++;
						break;

					default:
						break;
				}
				
		case '>':
			SY=RELOP;
			if (PEEK(1)=='=')
			{
				OP=GEOP;
				SYMCURSOR++;
			}
			else
				OP=GTOP;
			break;

		default:
			SY=OTHERSY;
			break;	
		}
		SYMCURSOR++;
		if (chartype::whitespace.in(CH)==true)
			goto retry;
	}
	if (SY==OTHERSY)
	{
		CH = PEEK();
		if ((CH==(char)(EOL))||(CH==0x0a))
		{
			CHECK();
			GETSTMTLEV=true;
			goto retry;
		}
		else
			PASCALCOMPILER::CERROR(400);
	}
	
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"\nSYMCURSOR = ",(int)SYMCURSOR," SY = ",SYMBOL_NAMES1[SY]);
#endif
} /*INSYMBOL*/


#include "stdafx.h"
#include "insymbol.h"
//#include "compiler.h"

//#define DEBUG_INSYMBOL	1
//#define DEBUG_RETRY		1
//#define DEBUG_PEEK0	1
//#define DEBUG_PEEK1	1
//#define DEBUG_GETC	

char PASCALSOURCE::PEEK()
{
	if (SYMCURSOR>1023)
		GETNEXTPAGE();
//		throw (SYMCURSOR);

	ASSERT(SYMCURSOR>=0);
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
	if (POS>1023)
		throw (SYMCURSOR);

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
	if (SYMCURSOR>1023)
		throw (SYMCURSOR);

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
			if (options.NOISY)
				WRITELN(OUTPUT);
			else if (options.LIST&&(ERRORNUM<=400))
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
			if (options.NOISY)
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
	if (options.NOISY)
		WRITE(OUTPUT,"<",SCREENDOTS/*4*/,'>');

} /*CERROR*/ ;

void PASCALSOURCE::GETNEXTPAGE()
{
//	WRITELN(OUTPUT,"\nPASCALCOMPILER::GETNEXTPAGE()");
	SYMCURSOR=0;
	LINESTART=0;
	int read;
	if (options.USING) {
		if (USEFILE==WORKCODE) {
			read = SYSCOMM::BLOCKREAD(USERINFO.WORKCODE,*SYMBUFP,2,SYMBLK);
			if (read!=2)
				options.USING=false; }
		else if (USEFILE==SYSLIBRARY) {
			read = SYSCOMM::BLOCKREAD(&LIBRARY,*SYMBUFP,2,SYMBLK);
			if (read!=2)
				options.USING=false; }			
		if (!options.USING) {
			SYMBLK=PREVSYMBLK;
			SYMCURSOR=PREVSYMCURSOR;
			LINESTART=PREVLINESTART; }
	}
	if (!options.USING)
	{
		if (options.INCLUDING) {
			read = SYSCOMM::BLOCKREAD(&INCLFILE,*SYMBUFP,2,SYMBLK);
			if (read!=2) {
				SYSCOMM::CLOSE((FILE*)(&INCLFILE));
				options.INCLUDING=false;
				SYMBLK=OLDSYMBLK;
				SYMCURSOR=OLDSYMCURSOR;
				LINESTART=OLDLINESTART; } }
	}
	if (!(options.INCLUDING||options.USING)) {
		read = SYSCOMM::BLOCKREAD(USERINFO.WORKSYM,*SYMBUFP,2,SYMBLK);
		if (read!=2)
			CERROR(401);
	}
	if (SYMCURSOR==0)
	{
		if (options.INMODULE)
			if (options.ININTERFACE&&!options.USING)
				WRITETEXT();
		if ((*SYMBUFP)[0]==(char)(16))
			SYMCURSOR=2;
		SYMBLK=SYMBLK+2;
	};
}

void PASCALSOURCE::PRINTLINE()
{
	char	DORLEV,STARORC;
	int LENG;
	char A[100];
	STARORC=':';
	if (options.DP)
		DORLEV='D';
	else
		DORLEV=(char)((BEGSTMTLEV%10) + ORD('0'));

   if (options.BPTONLINE)
	   STARORC='*';
   WRITE(OUTPUT,_LP._tmpfname,SCREENDOTS/*6**/,
	   (int)SEG/*4*/,
	   (int)CURPROC,/*5,*/
	   STARORC,
	   DORLEV,
	   LINEINFO/*,6*/," ");

   LENG=SYMCURSOR-LINESTART;
   if (LENG>100)
	   LENG=100;
   MOVELEFT((const char*)&(SYMBUFP[LINESTART]),A,LENG);
   if (A[0]==(char)(16/*DLE*/))
   {
       if (A[1]>' ')
		   WRITE(OUTPUT,0,_LP._tmpfname," ",ORD(A[1])-ORD(' '));
       LENG=LENG-2;
       MOVELEFT((const char*)&A[2],A,LENG);
   };
   A[LENG-1]=char(EOL);
   WRITE(OUTPUT,0,_LP._tmpfname,&A[0],LENG);
   if ((USERINFO.ERRBLK==SYMBLK)&&(USERINFO.ERRSYM>LINESTART))
	   WRITELN(OUTPUT,0,_LP._tmpfname,">>>>>> Error # ",USERINFO.ERRNUM);

} /*PRINTLINE*/ ;

void PASCALSOURCE::CHECK()
{
//	WRITELN(OUTPUT,"PASCALCOMPILER::CHECK()");
	/* CHECKS FOR THE END OF THE PAGE */
	char CH;
	SCREENDOTS++;
	CH = GETC();
	if (options.NOISY)
	{
		WRITE(OUTPUT,".");
		if ((SCREENDOTS-STARTDOTS)%50==0)
		{
			WRITELN(OUTPUT);
			WRITE(OUTPUT,"<",SCREENDOTS,">");
		}
	}
	if (options.LIST)
		PRINTLINE();
	options.BPTONLINE=false;
	if (PEEK()=='\0')
		GETNEXTPAGE();
	else
		LINESTART=SYMCURSOR;
	if (PEEK()==12/*FF*/)
		CH = GETC();
	if (PEEK()==16/*DLE*/)
		SYMCURSOR=SYMCURSOR+2;
	
	bool skip=true;
	CH=PEEK();
	while(skip)
	{
		if (chartype::whitespace.in(CH))
			CH=GETC();
		else
			skip=false;
	}	
	if (options.DP)
		LINEINFO=LC;
	else
		LINEINFO=IC;
}

void PASCALSOURCE::PARSEOPTION(char STOPPER)
{
	char CH,CH1,SW,DEL;
	char LTITLE[64];
	do {
		CH = PEEK(1);
		SW = PEEK(2);
		DEL= PEEK(3);
		if ((SW==',')||(SW==STOPPER))
		{
			DEL=SW;
			SW='+';
			CH1 = GETC();
		};
WRITELN(OUTPUT,"PASCALCOMPILER::COMMENTER() - PARSING OPTION ",CH);
		switch (CH) {
			case 'C':
				if (LEVEL>1)
					PASCALSOURCE::CERROR(194);
				COMMENT = new char [80];
				SCANSTRING(COMMENT,80,STOPPER);
				return;
			break;

			case 'D':
				options.DEBUGGING=(SW=='+');
				break;
				
			case 'G':
				options.GOTOOK=(SW=='+');
				break;

			case 'I':
#if 0
				if ((SW=='+')||(SW=='-'))
					options.IOCHECK=(SW=='+');
				else
				{
					SCANSTRING(LTITLE,40,STOPPER);
					if (STOPPER=='*')
						CH1 = GETC();
					if (options.LIST)
					{
						CH = GETC();
						PRINTLINE();
						SYMCURSOR--;
					};
					if (options.INCLUDING||options.INMODULE&&options.ININTERFACE)
					{
						PASCALSOURCE::CERROR(406);
						return;
					};
					SYSCOMM::OPENOLD(&INCLFILE,LTITLE);
					if (SYSCOMM::IORESULT()!=0)
					{
						strcat_s(LTITLE,64,".TEXT");
						SYSCOMM::OPENOLD(&INCLFILE,LTITLE);
						if (SYSCOMM::IORESULT()!=0)
							PASCALSOURCE::CERROR(403);
					};
					options.INCLUDING=true;
					OLDSYMCURSOR=SYMCURSOR;
					OLDLINESTART=LINESTART;
					OLDSYMBLK=SYMBLK-2;
					SYMBLK=2;
					GETNEXTPAGE();
					INSYMBOL();
					throw (SYMCURSOR);
				}
#endif
				break;

			case 'L':
				if ((SW=='+')||(SW=='-'))
				{
					options.LIST=(SW=='+');
					if (options.LIST)
						SYSCOMM::OPENNEW(&_LP,"*SYSTEM.LST.TEXT");
				}
				else
				{
					SCANSTRING(LTITLE,40,STOPPER);
					SYSCOMM::OPENNEW(&_LP,LTITLE);
					options.LIST=SYSCOMM::IORESULT()==0;
					return;
				};
				break;

			case 'Q':
				options.NOISY=(SW=='-');
				break;
				
			case 'P':
				WRITE(OUTPUT,_LP._tmpfname,(char)(12/*FF*/));
				break;
				
			case 'R':
				options.RANGECHECK=(SW=='+');
				break;

			case 'S':
				options.NOSWAP = (SW=='-');
				break;

			case 'T':
				options.TINY=(SW=='+');
				break;
				
			case 'U':
				if ((SW=='+')||(SW=='-'))
				{
					options.SYSCOMP=(SW=='-');
					options.RANGECHECK=!options.SYSCOMP;
					options.IOCHECK=options.RANGECHECK;
					options.GOTOOK=options.SYSCOMP;
				}
				else
					if (!options.USING)
					{
						SCANSTRING(SYSTEMLIB,40,STOPPER);
						SYSCOMM::CLOSE(&LIBRARY);
						options.LIBNOTOPEN=true;
						return;
					}
				break;

			default:
				break;
			};
			SYMCURSOR=SYMCURSOR+3;
	}
	while (DEL==',');
}

void PASCALSOURCE::COMMENTER(char STOPPER)
{
#if 0
	WRITELN(OUTPUT,"PASCALCOMPILER::COMMENTER()");
#endif
	char CH, CH1;
	CH1 = GETC();
	if (PEEK()=='$')
	if (PEEK(1)!=STOPPER)
		PARSEOPTION(STOPPER);

	do {
		do
		{		
			while (GETC()==(char)(EOL))
				CHECK();
		}
		while (PEEK()!=STOPPER);
		CH = PEEK(1);
		CH1 = GETC();
	}
	while ((CH!=')')&&(STOPPER!='}'));
	CH1 = GETC();
} /*COMMENTER*/

void PASCALSOURCE::SCANSTRING(char *STRG, int MAXLENG, char STOPPER)
{
	int LENG;
	SYMCURSOR=SYMCURSOR+2;
	LENG=SCAN(MAXLENG,true,STOPPER,&((*SYMBUFP)[SYMCURSOR]));
	STRG[0]=char(LENG);
	MOVELEFT(&(*SYMBUFP[SYMCURSOR]),&(STRG[1]),LENG);
	SYMCURSOR=SYMCURSOR+LENG+1;
} /*SCANSTRING*/

void PASCALSOURCE::STRING()
{
	char CH;
	char T[80];
	int TP;
	bool	DUPLE;
 	T[0] = 0;
	memset((char*)(&T[0]),0,80);
	memset((char*)(SCONST->SVAL),0,256);

	DUPLE=false; /* INDICATES WHEN "" IS PRESENT */
	TP=0; /* INDEX INTO TEMPORARY STRING */
	do
	{		
		if (DUPLE)
			CH = GETC();
		do
		{
			TP++;
			if (TP==80)
				goto done;
			CH = GETC();
			if (PEEK()==char(EOL))
			{ 
				CERROR(202);
				CHECK();
				goto done;
			}
			T[TP]=CH=PEEK();
		}
		while (CH!='\'');
		CH=PEEK(1);
		DUPLE=true;
	}
	while (CH=='\'');

done:
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
	CH=GETC();
}
/*STRING*/;

void PASCALSOURCE::NUMBER()
{
	CSP LVP;
	int	EXPONENT,ENDI,ENDF,ENDE;
	int	SIGN,IPART,FPART,EPART,ISUM;
	enum {REALTIPE, INTTIPE} TIPE;
	float	RSUM;
	bool	NOTLONG;
	int K,J;
	char CH,CH1;

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
	while (chartype::digits.in(PEEK()))
	{
		CH1 = GETC();	
	}
	ENDI=SYMCURSOR-1;

/* SYMCURSOR NOW POINTS AT FIRST CHARACTER PAST int PART */

	if (PEEK()=='.')
	{
		CH = PEEK(1);
		CH1 = GETC();
		if (CH!='.')  /* WATCH OUT for (".." */
		{
			TIPE=REALTIPE;
			CH1 = GETC();
			FPART=SYMCURSOR; /* {NING) FPART */
			while (chartype::digits.in(PEEK()))
			{
				CH1 = GETC();
				if (SYMCURSOR==FPART)
					PASCALSOURCE::CERROR(201);
			}
			ENDF=SYMCURSOR-1;
		}
	}
	if (PEEK()=='E')
	{
		TIPE=REALTIPE;
		CH1 = GETC();
		if (PEEK()=='-')
		{
			CH1 = GETC();
			SIGN=-1;
		}
		else if (PEEK()=='+')
			CH1 = GETC();
		EPART=SYMCURSOR; /* {NING) EXPONENT */
		while (chartype::digits.in(PEEK()))
		{
			CH1 = GETC();
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
		SYMCURSOR=ENDI;
	}
	else
	{ /* REAL NUMBER HERE */
		LVP = new CONSTREC;
		LVP->CCLASS=REEL;

		RSUM=0;
		SY=REALCONST; 
		OP=NOOP;
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
			};
			SYMCURSOR--; /* ADJUST for (POSTERITY */
		}
		LVP->RVAL=RSUM;
		VAL.VALP=LVP;
	} /*NUMBER*/
	SYMCURSOR++;
}

void PASCALSOURCE::WRITETEXT()
{
	MOVELEFT(SYMBUFP[SYMCURSOR],CODEP[0],1024);
	if (USERINFO.ERRNUM==0)
       if (SYSCOMM::BLOCKWRITE(USERINFO.WORKCODE,*CODEP,2,CURBLK)!=2)
         CERROR(402);
     CURBLK=CURBLK+2;
} /*WRITETEXT*/

void PASCALSOURCE::GETIDENT()
{
	char c;
	size_t	len;
	int index,i;
	pascal0::key_info *key;
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
 		SYMCURSOR = SYMCURSOR+(int)len;
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
		SYMCURSOR--;
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"IDENTIFIER: \"",ID,"\" ");
#endif
	}
}

bool PASCALSOURCE::GETOPERATOR()
{
	pascal0::key_info *key;
	bool found = false;
	bool digram = false;
	char ch0, ch1, ch2, ch3;
	ch0 = PEEK();
	if (ch0=='\'')
	{
		SY = STRINGCONST;
		OP = NOOP;
		return found;
	}
	ch0 = GETC();
	ch1 = PEEK();
	SY=OTHERSY;
	OP=NOOP;
	int index = 0;
	for(index=0;;index++) {
		key = &(pascal0::operators[index]);
		SY=key->SY;
		OP=key->OP;
		if (key->SY==OTHERSY)
			break;
		ch2 = key->ID[0];
		ch3 = key->ID[1];
		if (ch0!=ch2)
			continue;
		if ((ch3==0)||(ch1==ch3))
		{
			found = true;
			strcpy_s (ID,16,key->ID);
#ifdef DEBUG_INSYMBOL
			WRITELN (OUTPUT,"GETOPERATOR () - found SY = ",SYMBOL_NAMES1[SY]," \"",ID,"\"");
#endif
			break;
		}
	}
	if ((found==true)&&(ch1==ch3))
		ch0 = GETC();
	return found;
}

void PASCALSOURCE::GETSYMBOL() /* COMPILER VERSION 3.4 06-NOV-76 */
{
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"\nPASCALCOMPILER::INSYMBOL() SYMCURSOR = ",(int)SYMCURSOR," ");
#endif
	if (options.GETSTMTLEV)
	{
		BEGSTMTLEV=STMTLEV;
		options.GETSTMTLEV=false;
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
		GETIDENT();
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
	}
	else if (CH=='\'')
	{
		STRING();
	}
	else
	{
		bool result;
		result = GETOPERATOR();
		if (result==false)
			SY=OTHERSY;
	}
	if (SY==COMMENTSY)
	{
		COMMENTER('*');
		goto retry;
	}
	if (chartype::whitespace.in(CH)==true)
	{
#ifdef DEBUG_INSYMBOL
	WRITELN(OUTPUT,"\nPASCALCOMPILER::INSYMBOL() SYMCURSOR = ",(int)SYMCURSOR," CH= \'",CH,"\'");
#endif
		goto retry;
	}
	if (SY==OTHERSY)
	{
		CH = PEEK();
		if ((CH==(char)(EOL))||(CH==0x0a))
		{
			CHECK();
			options.GETSTMTLEV=true;
			goto retry;
		}
		else
			PASCALSOURCE::CERROR(400);
	}
	
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"\nSYMCURSOR = ",(int)SYMCURSOR," SY = ",SYMBOL_NAMES1[SY]);
#endif
} /*INSYMBOL*/

void PASCALSOURCE::INSYMBOL()
{
	bool status = false;
	OLDSYMCURSOR = PREVSYMCURSOR;
	PREVSYMCURSOR = SYMCURSOR;
	while (status==false)
	{	
		try
		{
			GETSYMBOL();
			status = true;
		}
		catch (CURSRANGE c)
		{
			GETNEXTPAGE();
		}
	}
}

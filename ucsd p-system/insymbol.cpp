
#include "stdafx.h"
#include <vector>
#include "../Frame Lisp/btreetype.h"
#include "compilerdata.h"

//#define DEBUG_INSYMBOL	1
//#define DEBUG_RETRY		1
//#define DEBUG_PEEK0	1
//#define DEBUG_PEEK1	1
//#define DEBUG_GETC	

//vector<PSYMBOL> PASCALSOURCE::m_symbols;


char PASCALSOURCE::PEEK()
{
	if (m_src.SYMCURSOR>1023)
		return 0;

//		GETNEXTPAGE();

	ASSERT(m_src.SYMCURSOR>=0);
	char c = (*m_src.SYMBUFP)[m_src.SYMCURSOR];
//	ASSERT(c>=0);
	if (c<0)
		c=0;

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
	POS = m_src.SYMCURSOR+i;
	if (POS>1023)
		return 0;

//		throw (m_src.SYMCURSOR);

	char c = (*m_src.SYMBUFP)[POS];

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
	if (m_src.SYMCURSOR>1023)
		throw (m_src.SYMCURSOR);
//		GETNEXTPAGE();
		
	char c = (*m_src.SYMBUFP)[m_src.SYMCURSOR];

#ifdef DEBUG_GETC
	if (c>=32)
		WRITELN(OUTPUT,"GETC(void) '",c,"'");
	else
		WRITELN(OUTPUT,"GETC(void) ",(int)c);
#endif

	m_src.SYMCURSOR++;
	return c;
}

void PASCALSOURCE::CERROR(int ERRORNUM)
{
//	int ERRSTART;
//	char A[128];
	char CH;
//	// // WITH USERINFO
	if ((USERINFO.ERRSYM!=m_src.SYMCURSOR)||(USERINFO.ERRBLK!=SYMBLK))
	{
		USERINFO.ERRBLK=SYMBLK;
		USERINFO.ERRSYM=m_src.SYMCURSOR;
		USERINFO.ERRNUM=ERRORNUM;
	
		if (options.NOISY)
			WRITELN(OUTPUT);
		else if (options.LIST&&(ERRORNUM<=400))
			return;
		WRITELN(OUTPUT," >>>>");
		if (m_src.LINESTART==0)
		{
			char *str1 = (char*)m_src.SYMBUFP;
			char *str2 = &(str1[m_src.SYMCURSOR]);
			WRITE(OUTPUT,str2);
		}
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
		WRITELN(OUTPUT,"Line ",SCREENDOTS,", error ",ERRORNUM,":");
	}
	if (USERINFO.STUPID)
		CH='E';

	else if (options.NOISY)
	{
		WRITE(OUTPUT,"<",SCREENDOTS/*4*/,'>');
		WRITELN(OUTPUT," <sp>(continue), <esc>(terminate), E(dit");
		WRITE(OUTPUT,":");
		do
		{
			SYSCOMM::READ(KEYBOARD,CH);
			WRITE(OUTPUT,CH);
		}
		while (!((CH==' ')||(CH=='E')||(CH=='e')||(CH==USERINFO.ALTMODE)));
		WRITELN(OUTPUT);
	}
	if ((CH=='E')||(CH=='e'))
	{
		EXIT_CODE E(ERRORNUM,true);
		USERINFO.ERRBLK=SYMBLK-2;
		throw(E);
	}
	if ((ERRORNUM>400)||(CH==(char)(27)))
	{
		EXIT_CODE E(ERRORNUM,false);
		USERINFO.ERRBLK=0;
		throw(E);
	}
} /*CERROR*/

void PASCALSOURCE::GETNEXTPAGE()
{
//	WRITELN(OUTPUT,"\nPASCALCOMPILER::GETNEXTPAGE()");
	m_src.SYMCURSOR=0;
	m_src.LINESTART=0;
	int read;
	if (options.USING) {
		if (USEFILE==WORKCODE) {
			read = SYSCOMM::BLOCKREAD(USERINFO.WORKCODE,*m_src.SYMBUFP,2,SYMBLK);
			if (read!=2)
				options.USING=false; }
		else if (USEFILE==SYSLIBRARY) {
			read = SYSCOMM::BLOCKREAD(&LIBRARY,*m_src.SYMBUFP,2,SYMBLK);
			if (read!=2)
				options.USING=false; }			
		if (!options.USING) {
			SYMBLK=PREVSYMBLK;
			m_src.SYMCURSOR=m_src.PREVSYMCURSOR;
			m_src.LINESTART=m_src.PREVLINESTART; }
	}
	if (!options.USING)
	{
		if (options.INCLUDING) {
			read = SYSCOMM::BLOCKREAD(&INCLFILE,*m_src.SYMBUFP,2,SYMBLK);
			if (read!=2) {
				SYSCOMM::CLOSE((FILE*)(&INCLFILE));
				options.INCLUDING=false;
				SYMBLK=OLDSYMBLK;
				m_src.SYMCURSOR=m_src.OLDSYMCURSOR;
				m_src.LINESTART=m_src.OLDLINESTART; } }
	}
	if (!(options.INCLUDING||options.USING)) {
		read = SYSCOMM::BLOCKREAD(USERINFO.WORKSYM,*m_src.SYMBUFP,2,SYMBLK);
		if (read!=2)
			CERROR(401);
	}
	if (m_src.SYMCURSOR==0)
	{
		if (options.INMODULE)
			if (options.ININTERFACE&&!options.USING)
				WRITETEXT();
		if ((*m_src.SYMBUFP)[0]==(char)(16))
			m_src.SYMCURSOR=2;
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

   LENG=m_src.SYMCURSOR-m_src.LINESTART;
   if (LENG>100)
	   LENG=100;
   MOVELEFT((const char*)&(m_src.SYMBUFP[m_src.LINESTART]),A,LENG);
   if (A[0]==(char)(16/*DLE*/))
   {
       if (A[1]>' ')
		   WRITE(OUTPUT,0,_LP._tmpfname," ",ORD(A[1])-ORD(' '));
       LENG=LENG-2;
       MOVELEFT((const char*)&A[2],A,LENG);
   };
   A[LENG-1]=char(EOL);
   WRITE(OUTPUT,0,_LP._tmpfname,&A[0],LENG);
   if ((USERINFO.ERRBLK==SYMBLK)&&(USERINFO.ERRSYM>m_src.LINESTART))
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
		m_src.LINESTART=m_src.SYMCURSOR;
	if (PEEK()==12/*FF*/)
		CH = GETC();
	if (PEEK()==16/*DLE*/)
		m_src.SYMCURSOR=m_src.SYMCURSOR+2;
	
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
				if ((SW=='+')||(SW=='-'))
					options.IOCHECK=(SW=='+');
#if 0			
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
			m_src.SYMCURSOR=m_src.SYMCURSOR+3;
	}
	while (DEL==',');
}

void PASCALSOURCE::COMMENTER(char STOPPER)
{
#if 0
	WRITELN(OUTPUT,"PASCALCOMPILER::COMMENTER()");
#endif
	char CH, CH1;
	CH1 = PEEK();
	if (CH1=='$')
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
	m_src.SYMCURSOR=m_src.SYMCURSOR+2;
	LENG=SCAN(MAXLENG,true,STOPPER,&((*m_src.SYMBUFP)[m_src.SYMCURSOR]));
	STRG[0]=char(LENG);
	MOVELEFT(&(*m_src.SYMBUFP[m_src.SYMCURSOR]),&(STRG[1]),LENG);
	m_src.SYMCURSOR=m_src.SYMCURSOR+LENG+1;
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
	SY=SYMBOLS::STRINGCONST;
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

	IPART=m_src.SYMCURSOR; /* int PART STARTS HERE */
	while (chartype::digits.in(PEEK()))
	{
		CH1 = GETC();	
	}
	ENDI=m_src.SYMCURSOR-1;

/* SYMCURSOR NOW POINTS AT FIRST CHARACTER PAST int PART */

	if (PEEK()=='.')
	{
		CH = PEEK(1);
		CH1 = GETC();
		if (CH!='.')  /* WATCH OUT for (".." */
		{
			TIPE=REALTIPE;
			CH1 = GETC();
			FPART=m_src.SYMCURSOR; /* {NING) FPART */
			while (chartype::digits.in(PEEK()))
			{
				CH1 = GETC();
				if (m_src.SYMCURSOR==FPART)
					PASCALSOURCE::CERROR(201);
			}
			ENDF=m_src.SYMCURSOR-1;
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
		EPART=m_src.SYMCURSOR; /* {NING) EXPONENT */
		while (chartype::digits.in(PEEK()))
		{
			CH1 = GETC();
		}
		ENDE=m_src.SYMCURSOR-1;
		if (ENDE<EPART)
			PASCALSOURCE::CERROR(201); /* CERROR IN REAL CONSTANT */
	}
/* NOW CONVERT INTERNAL FORM */
	if (TIPE==INTTIPE)
	{
		ISUM=0;
		m_src.SYMCURSOR=IPART;
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
			SY=SYMBOLS::INTCONST;
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
			LVP = new CONSTREC(LONG1); //LVP,LONG1); 
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
			SY=SYMBOLS::LONGCONST;
			OP=NOOP;
			LGTH=ENDI-IPART+1;
			VAL.VALP=LVP;
		}
		m_src.SYMCURSOR=ENDI;
	}
	else
	{ /* REAL NUMBER HERE */
		LVP = new CONSTREC(REEL);
		RSUM=0;
		SY=SYMBOLS::REALCONST; 
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
			m_src.SYMCURSOR--; /* ADJUST for (POSTERITY */
		}
		LVP->RVAL=RSUM;
		VAL.VALP=LVP;
	} /*NUMBER*/
	m_src.SYMCURSOR++;
}

void PASCALSOURCE::WRITETEXT()
{
	MOVELEFT(m_src.SYMBUFP[m_src.SYMCURSOR],CODEP[0],1024);
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
	int val = m_src.SYMCURSOR;
	index = SEARCH::IDSEARCH(val,(char*&)(m_src.SYMBUFP));
	if (index!=-1)
	{
		key = SEARCH::get_key_info (index);
		ASSERT((key->SY>=0)&&(key->SY<SYMBOLS::MAXSYMBOL));
		SY = key->SY;
		OP = key->OP;
		len = strlen(key->ID);
		strcpy_s(ID,IDENTSIZE,key->ID);
 		m_src.SYMCURSOR = m_src.SYMCURSOR+(int)len;
#ifdef DEBUG_INSYMBOL
		WRITELN(OUTPUT,"FOUND: \"",ID,"\" ");
#endif
	}
	else
	{
		SY = SYMBOLS::IDENT;
		OP = NOOP;
		memset(ID,0,IDENTSIZE);
	 	ID[0]=GETC();
		for (i=1;i<16;i++)
		{
			c = GETC();
			if (chartype::ident.in(c))
				ID[i]=c;
			else
				break;
		}
		m_src.SYMCURSOR--;
#ifdef DEBUG_INSYMBOL
	WRITE(OUTPUT,"identifier: \"",ID,"\" ");
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
		SY = SYMBOLS::STRINGCONST;
		OP = NOOP;
		return found;
	}
	ch0 = GETC();
	ch1 = PEEK();
	SY=SYMBOLS::OTHERSY;
	OP=NOOP;
	int index = 0;
	for(index=0;;index++) {
		key = &(pascal0::operators[index]);
		SY=key->SY;
		OP=key->OP;
		if (key->SY==SYMBOLS::OTHERSY)
			break;
		ch2 = key->ID[0];
		ch3 = key->ID[1];
		if (ch0!=ch2)
			continue;
		if ((ch3==0)||(ch1==ch3))
		{
			found = true;
			strcpy_s (ID,IDENTSIZE,key->ID);
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
	if (options.GETSTMTLEV) {
		BEGSTMTLEV=STMTLEV;
		options.GETSTMTLEV=false;
	}

retry:
	SY=(SYMBOLS::SYMBOL)-1;
	OP=NOOP;
	char CH = PEEK();
	if (chartype::alpha.in(CH))
		GETIDENT();
	else if (chartype::digits.in(CH))
		NUMBER();
	else if (CH=='\'')
		STRING();
	else if (chartype::whitespace.in(CH)) {
		CH = GETC();
		goto retry;
	}
	else {
		bool result;
		result = GETOPERATOR();
		if (result==false)
			SY=SYMBOLS::OTHERSY;
		if (SY==SYMBOLS::COMMENTSY) {
			COMMENTER('*');
			goto retry;
		}
	}
	if (SY==SYMBOLS::OTHERSY) {
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
}

#if 0
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
	else if (chartype::whitespace.in(CH)==true)
	{
#ifdef DEBUG_INSYMBOL
	WRITELN(OUTPUT,"\nPASCALCOMPILER::INSYMBOL() SYMCURSOR = ",(int)SYMCURSOR," CH= \'",CH,"\'");
#endif
		CH = GETC();
		goto retry;
	}
	else {
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
#endif

void PASCALSOURCE::INSYMBOL()
{
	bool status = false;
	m_src.OLDSYMCURSOR = m_src.PREVSYMCURSOR;
	m_src.PREVSYMCURSOR = m_src.SYMCURSOR;
	while (status==false)
	{	
		try
		{
 			GETSYMBOL();
			if (SY!=SYMBOLS::SEPARATSY)
				status = true;

		}
		catch (CURSRANGE c)
		{
// todo - figure out continuation
// if a symbol is split by a page
// break - maybe modify PEEK and GETC
// to preprocess using text_object class
// then grab characters from a shadow
// node_list<char*>
			GETNEXTPAGE();
		}
		catch (EXIT_CODE E)
		{
			ASSERT(false);
		}
	}
}

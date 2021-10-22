
#include "stdafx.h"
#include "compiler.h"
#include "declarationpart.h"

#define EXIT(A) throw;

DECLARATIONPART::DECLARATIONPART(SETOFSYS FSYS)
{
	if ((NOSWAP)&&(STARTINGUP))
	{
		STARTINGUP = FALSE; /* ALL SEGMENTS ARE IN BY THIS TIME */
		BLOCK(FSYS);
		EXIT(DECLARATIONPART);
	};
	if (NOISY)
		SYSCOMM::UNITWRITE(3,&DUMMYVAR[-1600],35); /*ADJUST DISPLAY STACK AND HEAP*/

	do {
		NOTDONE = FALSE;
		if (USERINFO.STUPID)
			if (!CODEINSEG)
				if ((LEVEL==1)&&(NEXTSEG==10))
					if (!(INMODULE||USING))
						USESDECLARATION(true);
		/*To get turtle graphics*/
		if (SY==USESSY)
		{
			INSYMBOL();
			USESDECLARATION(false);
		};
		if (SY==LABELSY)
		{
			if (INMODULE&&ININTERFACE)
			{
				CERROR(186);
				SKIP(FSYS-LABELSY);
			}
			else
				INSYMBOL();
			LABELDECL();
		};
		if (SY==CONSTSY)
		{
			INSYMBOL();
			CONSTDECL();
		};
		if (SY==TYPESY)
		{
			INSYMBOL();
			TYPEDECL();
		};
		if (SY==VARSY)
		{
			INSYMBOL();
			VARDECL();
		};
			if (LEVEL==1)
				GLEV=TOP;
			if (SET(3,PROCSY,FUNCSY,PROGSY).in(SY))
			{
				if (INMODULE)
					if (ININTERFACE&&!USING)
						PUBLICPROCS=true;
					do {
						LSY=SY;
						INSYMBOL();
						if (LSY==PROGSY)
							if (INMODULE)
							{
								CERROR(185 /*SEG DEC !ALLOWED IN UNIT*/);
								PROCDECLARATION(PROCSY,false);
							}
							else 
								PROCDECLARATION(LSY,true);
						else
							PROCDECLARATION(LSY,false);
					}
					while (SET(3,PROCSY,FUNCSY,PROGSY).in(SY));
				}
				if ((SY!=BEGINSY))
					if (!((USING||INMODULE)&&(SET(2,IMPLESY,ENDSY).in(SY)))
						&&!(SET(2,SEPARATSY,UNITSY).in(SY)))
						if ((!(INCLUDING||NOTDONE))
							||(!BLOCKBEGSYS.in(SY)))
						{
							CERROR(18);
							SKIP(FSYS - SET(2,UNITSY,INTERSY));
						}
	}
	while (!((STATBEGSYS+SEPARATSY+UNITSY+IMPLESY+ENDSY).in(SY)));
	NEWBLOCK = FALSE;
} /*DECLARATIONPART*/

void DECLARATIONPART::TYPED(SETOFSYS FSYS, STP &FSP, ADDRRANGE &FSIZE)
//void SANDBOX::TYPED(SETOFSYS FSYS, STP &FSP, ADDRRANGE &FSIZE)
{
	STP LSP,LSP1,LSP2;
	DISPRANGE OLDTOP;
	ADDRRANGE LSIZE,DISPL;
	int LMIN,LMAX;

	BITRANGE NEXTBIT,NUMBITS;
	/*TYP*/
	PACKING=false;
	if (!(TYPEBEGSYS.in(SY)))
	{
		CERROR(10);
		SKIP(FSYS + TYPEBEGSYS);
	}
	if (TYPEBEGSYS.in(SY))
	{
		if (SIMPTYPEBEGSYS.in(SY))
			SIMPLETYPE(FSYS,FSP,FSIZE);
		else if (SY==ARROW)
			POINTERTYPE(FSP,LSP);
		else
		{
			if (SY==PACKEDSY)
			{
				INSYMBOL();
				PACKING=true;
				if (!(TYPEDELS.in(SY)))
				{
					CERROR(10);
					SKIP(FSYS + TYPEDELS);
				}
			}
			if (SY==ARRAYSY)
			{
				INSYMBOL();
				if (SY==LBRACK)
					INSYMBOL();
				else
					CERROR(11);
				LSP1=NULL;
			do 
			{
				if (PACKING)
					LSP = (STRUCTURE*) new STRUCTURE;
				//	NEW(LSP,ARRAYS,true,false);
				else
					LSP = (STRUCTURE*) new STRUCTURE;
				//	NEW(LSP,ARRAYS,false);
				// WITH LSP^
				LSP->AELTYPE=LSP1;
				LSP->INXTYPE=NULL;
				if (PACKING)
					LSP->AISSTRNG=false;
				LSP->AISPACKD=false;
				LSP->FORM=ARRAYS;
				LSP1=LSP;
				SIMPLETYPE(FSYS+COMMA+RBRACK+OFSY,LSP2,LSIZE);
				LSP1->SIZE=LSIZE;
				if (LSP2!=NULL)
					if (LSP2->FORM<=SUBRANGE)
					{
						if (LSP2==REALPTR)
						{
							CERROR(109);
							LSP2=NULL;
						}
						else if (LSP2==INTPTR)
						{
							CERROR(149);
							LSP2=NULL;
						}
						LSP->INXTYPE=LSP2;
					}
					else
					{
						CERROR(113);
						LSP2=NULL;
					}
					TEST=SY!=COMMA;
					if (!TEST)
						INSYMBOL();
				}
				while (!TEST);

				if (SY==RBRACK)
					INSYMBOL();
				else
					CERROR(12);
				if (SY==OFSY)
					INSYMBOL();
				else
					CERROR(8);
				TYP(FSYS,LSP,LSIZE);
				if (LSP!=NULL)
					if (LSP->FORM==FILES)
						CERROR(108);
				if (PACKABLE(LSP,NUMBITS))
					if (NUMBITS + NUMBITS<=BITSPERWD)
						// WITH LSP1^
					{
						LSP1->AISPACKD=true;
						LSP1->ELSPERWD=BITSPERWD/NUMBITS;
						LSP1->ELWIDTH=NUMBITS;
					}
			do 
			{
				// WITH LSP1^
				LSP2=LSP1->AELTYPE;
				LSP1->AELTYPE=LSP;
				if (LSP1->INXTYPE!=NULL)
				{
					GETBOUNDS(LSP1->INXTYPE,LMIN,LMAX);
					if (LSP1->AISPACKD)
						LSIZE=(LMAX-LMIN+LSP1->ELSPERWD)/LSP1->ELSPERWD;
					else
						LSIZE=LSIZE*(LMAX-LMIN+1);
					if (LSIZE<=0)
					{
						CERROR(398);
						LSIZE=1;//fixme?
					}
					LSP1->SIZE=LSIZE;
				}
				LSP=LSP1;
				LSP1=LSP2;
			}
			while (!LSP1==NULL);
			}
			else if (SY==RECORDSY)
			{
				INSYMBOL();
				OLDTOP=TOP;
				if (TOP<DISPLIMIT)
				{
					TOP=TOP + 1;
					// WITH DISPLAY[TOP]
					DISPLAY[TOP].FNAME=NULL;
					DISPLAY[TOP].OCCUR=REC;
				}
				else
					CERROR(250);
				DISPL=0;
				NEXTBIT=0;
				FIELDLIST(FSYS-SEMICOLON+ENDSY,LSP1,LSP,NUMBITS);
				DISPL=DISPL + ORD(NEXTBIT>0);
				LSP = (STRUCTURE*) new STRUCTURE;
				// NEW(LSP,RECORDS);
				// WITH LSP^
				LSP->FSTFLD=DISPLAY[TOP].FNAME;
				LSP->RECVAR=LSP1;
				LSP->SIZE=DISPL;
				LSP->FORM=RECORDS;
				TOP=OLDTOP;
				if (SY==ENDSY)
					INSYMBOL();
				else
					CERROR(13);
			}
			else if (SY==SETSY)
			{
				INSYMBOL();
				if (SY==OFSY)
					INSYMBOL();
				else
					CERROR(8);
				SIMPLETYPE(FSYS,LSP1,LSIZE);
				if (LSP1!=NULL)
					if ((LSP1->FORM>SUBRANGE)||
						(LSP1==INTPTR)||(LSP1==REALPTR))
					{
						CERROR(115);
						LSP1=NULL;
					}
					else if (LSP1==REALPTR)
					{
						CERROR(114);
						LSP1=NULL;
					}
				LSP = (STRUCTURE*) new STRUCTURE;
				// NEW(LSP,POWER);
				// WITH LSP^
				LSP->ELSET=LSP1;
				LSP->FORM=POWER;
				if (LSP1!=NULL)
				{
					GETBOUNDS(LSP1,LMIN,LMAX);
					LSP->SIZE=(LMAX+BITSPERWD)/BITSPERWD;
					if (LSP->SIZE>255)
					{
						CERROR(169);
						LSP->SIZE=1;
					}
				}
				else
					LSP->SIZE=0;
			}
			else if (SY==FILESY)
			{
				if (INMODULE)
					if (!ININTERFACE)
						CERROR(191); /*NO PRIVATE FILES*/
				INSYMBOL();
				// NEW(LSP,FILES);
				LSP = (STRUCTURE*) new STRUCTURE;
				// WITH LSP^
				LSP->FORM=FILES;
				LSP->FILTYPE=NULL;
				if (SY==OFSY)
				{
					INSYMBOL();
					TYP(FSYS,LSP1,LSIZE);
				}
				else
					LSP1=NULL;
				LSP->FILTYPE=LSP1;
				if (LSP1!=NULL)
					LSP->SIZE=FILESIZE + LSP1->SIZE;
				else
					LSP->SIZE=NILFILESIZE;
			}
			FSP=LSP;

		}
		if (!(FSYS.in(SY)))
		{
			CERROR(6);
			SKIP(FSYS);
		}
	}
	else
		FSP=NULL;
	if (FSP==NULL)
		FSIZE=1;
	else
		FSIZE=FSP->SIZE;
} /*TYP*/

void DECLARATIONPART::SIMPLETYPE(SETOFSYS FSYS, STP &FSP, ADDRRANGE &FSIZE)
{
	STP LSP,LSP1;
	CTP LCP,LCP1;
	DISPRANGE TTOP;
	int LCNT;
	VALU LVALU;
	FSIZE=1;
	if (!SIMPTYPEBEGSYS.in(SY))
	{
		CERROR(1);
		SKIP(SIMPTYPEBEGSYS+FSYS);
		if (SIMPTYPEBEGSYS.in(SY))
		{
			if (SY==LPARENT)
			{
				TTOP=TOP;
				while (DISPLAY[TOP].OCCUR!=BLCK)
				{
					TOP=TOP - 1;
					//					NEW(LSP,SCALAR,DECLARED);
					LSP = (STRUCTURE*) new STRUCTURE;
					// WITH LSP
					LSP->SIZE=INTSIZE;
					LSP->FORM=SCALAR;
					LSP->SCALKIND=DECLARED;
					LCP1=NULL;
					LCNT=0;
				}
				do {
					INSYMBOL();
					if (SY==IDENT)
					{
						//				NEW(LCP,KONST);
						LCP = (IDENTIFIER*) new IDENTIFIER; 
						// WITH LCP^
						strcpy_s(LCP->NAME,15,ID);
						LCP->IDTYPE=LSP;
						LCP->NEXT=LCP1;
						LCP->VALUES.IVAL=LCNT;
						LCP->KLASS=KONST;
						ENTERID(LCP);
						LCNT=LCNT + 1;
						LCP1=LCP;
						INSYMBOL();
					}
					else
						CERROR(2);
					if (!(FSYS+COMMA+RPARENT).in(SY))
					{
						CERROR(6);
						SKIP(FSYS+COMMA+RPARENT);
					}
				}
				while (SY==COMMA);
				LSP->FCONST=LCP1;
				TOP=TTOP;
				if (SY==RPARENT)
					INSYMBOL();
				else 
					CERROR(4);
			}
			else
			{
				if (SY==IDENT)
				{
					SEARCHID(SETOFIDS(2,TYPES,KONST),LCP);
					INSYMBOL();
					if (LCP->KLASS==KONST)
					{
						//				NEW(LSP,SUBRANGE);
						//				WITH LSP^, LCP
						LSP = (STRUCTURE*) new STRUCTURE;
						LSP->RANGETYPE=LCP->IDTYPE;
						LSP->FORM=SUBRANGE;
						if (STRGTYPE(LSP->RANGETYPE))
						{
							CERROR(148);
							LSP->RANGETYPE=NULL;
							LSP->MIN=LCP->VALUES;
							LSP->SIZE=INTSIZE;
						}
						if (SY==COLON)
							INSYMBOL();
						else
							CERROR(5);
						CONSTANT(FSYS,LSP1,LVALU);
						LSP->MAX=LVALU;
						if (LSP->RANGETYPE!=LSP1)
							CERROR(107);
					}
					else
					{
						LSP=LCP->IDTYPE;
						if ((LSP==STRGPTR)&&(SY==LBRACK))
						{
							INSYMBOL();
							CONSTANT(FSYS+RBRACK,LSP1,LVALU);
							if (LSP1==INTPTR)
							{
								if ((LVALU.IVAL<=0)||(LVALU.IVAL>STRGLGTH))
								{
									CERROR(203);
									LVALU.IVAL=DEFSTRGLGTH;
								}
								if (LVALU.IVAL!=DEFSTRGLGTH)
								{
									//							 NEW(LSP,ARRAYS,true,true);
									LSP = (STRUCTURE*) new STRUCTURE;
									LSP->AELTYPE = STRGPTR;
									// WITH LSP^,LVALU
									LSP->MAXLENG=LVALU.IVAL;
									LSP->SIZE=(LVALU.IVAL+CHRSPERWD)/CHRSPERWD;
								}
							}
							else
								CERROR(15);
							if (SY==RBRACK)
								INSYMBOL();
							else
								CERROR(12);
						}
						else
							if (LSP==INTPTR)
								if (SY==LBRACK)
								{ INSYMBOL();
						//   NEW(LSP,LONGINT);
						LSP = (STRUCTURE*) new STRUCTURE;
						LSP->ELTYPE=LONGINTPTR;
						CONSTANT(FSYS+RBRACK,LSP1,LVALU);
						if (LSP1==INTPTR)
							if ((LVALU.IVAL<=0)||
								(LVALU.IVAL>MAXDEC))
								CERROR(203);
							else
								LSP->SIZE=DECSIZE(LVALU.IVAL);
						else
							CERROR(15);
						if (SY==RBRACK)
							INSYMBOL();
						else
							CERROR(12);
						}
								else
									if (LSP->FORM==FILES)
										if (INMODULE)
											if (!ININTERFACE)
												CERROR(191); /*NO PRIVATE FILES*/
						if (LSP!=NULL)
							FSIZE=LSP->SIZE;
					}
				} /*SY==IDENT*/
				else
				{ 
					//		 NEW(LSP,SUBRANGE);
					LSP = (STRUCTURE*) new STRUCTURE;
					LSP->FORM=SUBRANGE;
					CONSTANT(FSYS+COLON,LSP1,LVALU);
					if (STRGTYPE(LSP1))
					{
						CERROR(148);
						LSP1=NULL;
					};
					// WITH LSP^
					LSP->RANGETYPE = LSP1;
					LSP->MIN = LVALU;
					LSP->SIZE = INTSIZE;
					if (SY==COLON)
						INSYMBOL();
					else
						CERROR(5);
					CONSTANT(FSYS,LSP1,LVALU);
					LSP->MAX=LVALU;
					if (LSP->RANGETYPE!=LSP1)
						CERROR(107);
				};
				if (LSP!=NULL)
					// WITH LSP^
					if (LSP->FORM==SUBRANGE)
						if (LSP->RANGETYPE!=NULL)
							if (LSP->RANGETYPE==REALPTR)
								CERROR(399);
							else
								if (LSP->MIN.IVAL>LSP->MAX.IVAL)
								{
									CERROR(102);
									LSP->MAX.IVAL=LSP->MIN.IVAL;
								}
			};
			FSP=LSP;
			if (!FSYS.in(SY))
			{
				CERROR(6);
				SKIP(FSYS); 
			}
		}
		else
			FSP=NULL;
	}
} /*SIMPLETYPE*/

bool DECLARATIONPART::PACKABLE(STP FSP, BITRANGE &NUMBITS)
{
	bool result;
	int LMIN,LMAX;
	result=false;
	if ((FSP!=NULL)&&(PACKING))
		// WITH FSP^
		switch (FSP->FORM)
	{
		case SUBRANGE:
		case SCALAR: 
			if ((FSP!=INTPTR)&&(FSP!=REALPTR))
			{
				GETBOUNDS(FSP,LMIN,LMAX);
				if (LMIN>=0)
				{
					result=true;
					NUMBITS=1;
					LMIN=1;
					while (LMIN<LMAX)
					{
						LMIN=LMIN+1;
						LMIN=LMIN+LMIN-1;
						NUMBITS=NUMBITS+1;
					}
				}
			}
			break;
		case POWER:
			if (PACKABLE(FSP->ELSET,NUMBITS))
			{ GETBOUNDS(FSP->ELSET,LMIN,LMAX);
			LMAX=LMAX + 1;
			if (LMAX<BITSPERWD)
			{
				result=true;
				NUMBITS=LMAX;
			}
			}
			break;
		default:
			break;
	}
	return result;
}

void DECLARATIONPART::FIELDLIST(SETOFSYS FSYS, STP &FRECVAR, STP &LSP, BITRANGE &NUMBITS)
{
	/*FIELDLIST*/
//	CTP LCP,LCP1,NXT,NXT1,LAST;
//	STP LSP,LSP1,LSP2,LSP3,LSP4;
	CTP NXT, NXT1, LAST;
//	ADDRRANGE MINSIZE,MAXSIZE,LSIZE;
//	VALU LVALU;
//	BITRANGE MAXBIT,MINBIT;
	ADDRRANGE	LSIZE;

	NXT1=NULL;
	LSP=NULL;
	LAST=NULL;
	if (!SET(2,IDENT,CASESY).in(SY))
	{
		CERROR(19);
		SKIP(FSYS+IDENT+CASESY);
	}
	while (SY==IDENT)
	{
		NXT=NXT1;
		do
		{
			if (SY==IDENT)
			{
	#if 0
				if (PACKING)
					NEW(LCP,FIELD,true)
				else NEW(LCP,FIELD,false);
	#endif
				LCP = (IDENTIFIER*) new IDENTIFIER;
				// WITH LCP^
				strcpy_s(LCP->NAME,15,ID);
				LCP->IDTYPE=NULL;
				LCP->NEXT=NXT;
				LCP->KLASS=FIELD;
				LCP->FISPACKED=false;
				NXT=LCP;
				ENTERID(LCP);
				INSYMBOL();
			}
			else
				CERROR(2);

			SET TEST0(2,COMMA,COLON);
			if (!TEST0.in(SY))
			{
				CERROR(6);
				SKIP(FSYS+COMMA+COLON+SEMICOLON+CASESY);
			}
			TEST=SY!=COMMA;
			if (!TEST)
				INSYMBOL();
			while (!TEST);
			if (SY==COLON)
				INSYMBOL();
			else
				CERROR(5);
 			TYP(FSYS+CASESY+SEMICOLON,LSP,LSIZE);
			if (LSP!=NULL)
				if (LSP->FORM==FILES)
					CERROR(108);
		}
		while (NXT!=NXT1);

		// WITH NXT^
		NXT->IDTYPE=LSP;
		ALLOCATE(NXT,NUMBITS);
		if (NXT->NEXT==NXT1)
			LAST=NXT;
		NXT=NXT->NEXT;
		NXT1=LCP;
		if (SY==SEMICOLON)
		{
			INSYMBOL();
			if (!(SET(3,IDENT,ENDSY,CASESY).in(SY)))/* <<<< SMF 2-28-78 */
			{
				CERROR(19);
				SKIP(FSYS+IDENT+CASESY);
			}
		}
	} /*while /*/
	NXT=NULL;
	while (NXT1!=NULL) {
		// WITH NXT1^

		LCP=NXT->NEXT;
		NXT->NEXT=NXT;
		NXT=NXT1;
		NXT1=LCP;

		if (SY==CASESY)
			VARIANTLIST();
		else
			FRECVAR=NULL;
	}
} /*FIELDLIST*/

//void DECLARATIONPART::TYP::FIELDLIST::ALLOCATE(CTP FCP)
//void DECLARATIONPART::TYP::ALLOCATE(CTP FCP, BITRANGE &NUMBITS)
void DECLARATIONPART::ALLOCATE(CTP FCP, BITRANGE &NUMBITS)
{
	bool ONBOUND;
	ONBOUND=false;
	// WITH FCP^) {
	if (PACKABLE(FCP->IDTYPE,NUMBITS))
    {
       if ((NUMBITS+NEXTBIT)>BITSPERWD)
	   {
			DISPL=DISPL + 1;
			NEXTBIT=0;
			ONBOUND=true;
	   };
       FCP->FLDADDR=DISPL;
	   FCP->FISPACKED=true;
       FCP->FLDWIDTH=NUMBITS;
	   FCP->FLDRBIT=NEXTBIT;
       NEXTBIT=NEXTBIT + NUMBITS;
	}
	else
	{
		 DISPL=DISPL + ORD(NEXTBIT>0);
		 NEXTBIT=0;
		 ONBOUND=true;
		 FCP->FISPACKED=false;
		 FCP->FLDADDR=DISPL;
		 if (FCP->IDTYPE!=NULL)
			DISPL=DISPL + FCP->IDTYPE->SIZE;
	};
	if (ONBOUND&&(LAST!=NULL))
		// WITH LAST^
	if (FCP->FISPACKED)
       if (FCP->FLDRBIT==0)
		   FCP->FISPACKED=false;
       else if ((FCP->FLDWIDTH<=8)&&(FCP->FLDRBIT<=8))
		 {
			 FCP->FLDWIDTH=8;
			 FCP->FLDRBIT=8;
		 }
} /*ALLOCATE*/

void DECLARATIONPART::TYP::FIELDLIST::VARIANTLIST()
//void DECLARATIONPART::TYP::VARIANTLIST()
//void DECLARATIONPART::VARIANTLIST()
{
	SET FSYS;

	bool GOTTAGNAME;
	//	NEW(LSP,TAGFLD);
	LSP = (STRUCTURE*) new STRUCTURE;
	// WITH LSP^
	LSP->TAGFIELDP=NULL;
	LSP->FSTVAR=NULL;
	LSP->FORM=TAGFLD;
	FRECVAR=LSP;
	INSYMBOL();
	if (SY==IDENT)
	{
		//     if (PACKING)
		//		 NEW(LCP,FIELD,true)
		//		LCP = new (IDENTIFIER*) new IDENTIFIER;
		//    else
		//		NEW(LCP,FIELD,false);
		LCP = (IDENTIFIER*) new IDENTIFIER;
		// WITH LCP^
		LCP->IDTYPE=NULL;
		LCP->KLASS = FIELD;
		LCP->NEXT=NULL;
		LCP->FISPACKED=false;
		GOTTAGNAME=false;
		PRTERR=false;
		SEARCHID(TYPES,LCP1);
		PRTERR=true;
		if (LCP1==NULL)
		{ 
			GOTTAGNAME=true;
			strcpy_s(LCP->NAME,15,ID);
			ENTERID(LCP);
			INSYMBOL();
			if (SY==COLON)
				INSYMBOL();
			else
				CERROR(5);
		}
		if (SY==IDENT)
		{
			SEARCHID(TYPES,LCP1);
			LSP1=LCP1->IDTYPE;
			if (LSP1!=NULL)
			{
				if (LSP1->FORM<=SUBRANGE)
				{
				 if (COMPTYPES(REALPTR,LSP1)) CERROR(109);
				 LCP->IDTYPE=LSP1;
				 LSP->TAGFIELDP=LCP;
				 if (GOTTAGNAME)
					 ALLOCATE(LCP,NUMBITS);
				}
				else
					CERROR(110);
			}
			INSYMBOL();
		}
		else {
			CERROR(2);
			SKIP(FSYS+OFSY+LPARENT);
		}
	}
	else
	{
		CERROR(2);
		SKIP(FSYS+OFSY+LPARENT);
	};
	LSP->SIZE=DISPL + ORD(NEXTBIT>0);
	if (SY==OFSY)
		INSYMBOL();
	else
		CERROR(8);
	LSP1=NULL;
	MINSIZE=DISPL;
	MAXSIZE=DISPL;
	MINBIT=NEXTBIT;
	MAXBIT=NEXTBIT;
	do
	{
		LSP2=NULL;
		do
		{
			CONSTANT(FSYS+COMMA+COLON+LPARENT,LSP3,LVALU);
			if (LSP->TAGFIELDP!=NULL)
				if (!COMPTYPES(LSP->TAGFIELDP->IDTYPE,LSP3))
					CERROR(111);
				//     NEW(LSP3,VARIANT);
				LSP3 = (STRUCTURE*) new STRUCTURE;
				// WITH LSP3^
					LSP3->NXTVAR=LSP1;
					LSP3->SUBVAR=LSP2;
					LSP3->VARVAL=LVALU;
					LSP3->FORM=VARIANT2;
				LSP1=LSP3;
				LSP2=LSP3;
				TEST=SY!=COMMA;
				if (!TEST)
					INSYMBOL();
		}
		while (!TEST);

		if (SY==COLON)
			INSYMBOL();
		else
			CERROR(5);
		if (SY==LPARENT)
			INSYMBOL();
		else
			CERROR(9);
		if (SY==RPARENT)
			LSP2=NULL;
		else
			DECLARATIONPART::TYP::FIELDLIST(FSYS+RPARENT+SEMICOLON,LSP2);
		if (DISPL>MAXSIZE)
		{
			MAXSIZE=DISPL;
			MAXBIT=NEXTBIT;
		}
		else if ((DISPL==MAXSIZE)&&(NEXTBIT>MAXBIT))
					MAXBIT=NEXTBIT;
		while (LSP3!=NULL)
		{
			LSP4=LSP3->SUBVAR;
			LSP3->SUBVAR=LSP2;
			LSP3->SIZE=DISPL + ORD(NEXTBIT>0);
			LSP3=LSP4;
		}
		if (SY==RPARENT)
		{
			INSYMBOL();
			if (!((FSYS+SEMICOLON).in(SY)))
			{
				CERROR(6);
				SKIP(FSYS+SEMICOLON);	}
			}
			else
				CERROR(4);
			TEST=SY!=SEMICOLON;
			if (!TEST)
			{
				INSYMBOL();
				DISPL=MINSIZE;
				NEXTBIT=MINBIT;
			}
	}
	while (!((TEST)||(SY==ENDSY))); /* <<<< SMF 2-28-78 */
	DISPL=MAXSIZE;
	NEXTBIT=MAXBIT;
	LSP->FSTVAR=LSP1;
} /*VARIANTLIST*/

void DECLARATIONPART::POINTERTYPE(STP &FSP, STP &LSP)
//DECLARATIONPART::FIELDLIST
{
//	NEW(LSP,POINTER);
	FSP=LSP;
	// WITH LSP^
	LSP = (STRUCTURE*) new STRUCTURE;
	LSP->ELTYPE=NULL;
	LSP->SIZE=PTRSIZE;
	LSP->FORM=POINTER;
	INSYMBOL();
	if (SY==IDENT)
	{
		PRTERR=false;
		SEARCHID(TYPES,LCP);
		PRTERR=true;
		if (LCP==NULL) /*FORWARD REFERENCED TYPE ID*/
		{
//			NEW(LCP,TYPES);
			LCP = (IDENTIFIER*) new IDENTIFIER;
			// WITH LCP^
			strcpy_s(LCP->NAME,15,ID);
			LCP->IDTYPE=LSP;
			LCP->NEXT=FWPTR;
			LCP->KLASS=TYPES;
			FWPTR=LCP;
		}
		else
		{
			if (LCP->IDTYPE!=NULL)
				if ((LCP->IDTYPE->FORM!=FILES)||SYSCOMP)
					LSP->ELTYPE=LCP->IDTYPE;
				else
					CERROR(108);
		}
		INSYMBOL();
	}
	else
		CERROR(2);
} /*POINTERTYPE*/ 

void DECLARATIONPART::USESDECLARATION::GETTEXT(bool &FOUND)
{
	int SEGINDEX;
	CTP LCP;
	FOUND=false;
	LCP=MODPTR;
	while ((LCP!=NULL)&&(!FOUND))
	{
		if (LCP->NAME==ID)
			FOUND=true;
		else
			LCP=LCP->NEXT;
		if (FOUND)
		{
			LSEPPROC=SEGTABLE[LCP->SEGID].SEGKIND==4;
			if (!LSEPPROC)
			{
				SEG=LCP->SEGID;
				NEXTPROC=1;
			}
			BEGADDR=SEGTABLE[LCP->SEGID].TEXTADDR;
			USEFILE=WORKCODE;
		}
		else
		{
			FOUND=true;
			if (LIBNOTOPEN)
			{
				SYSCOMM::RESET(&LIBRARY,SYSTEMLIB);
				if (SYSCOMM::IORESULT()!=0)
				{
					CERROR(187);
					FOUND=false;
				}
				else
				{
					char *segdict = (char*)(&SEGDICT);
					int rd;
					SYSCOMM::BLOCKREAD(&LIBRARY,segdict,1,rd);
					if (SYSCOMM::IORESULT()!=1)
					{
						CERROR(187);
						FOUND=false;
					}
				}
			}
		}
		if (FOUND)
		{
			LIBNOTOPEN=false;
			SEGINDEX=0;
			FOUND=false;
			while ((SEGINDEX<=MAXSEG)&&(!FOUND))
			{
				if (MAGIC)
					if (SEGDICT.SEGNAME[SEGINDEX]==LNAME)
						FOUND=true;
					else
						SEGINDEX=SEGINDEX+1;
				else if (SEGDICT.SEGNAME[SEGINDEX]==ID)
					FOUND=true;
				else
					SEGINDEX=SEGINDEX + 1;
				if (FOUND)
				{
					USEFILE=SYSLIBRARY;
					BEGADDR=SEGDICT.TEXTADDR[SEGINDEX];
					LSEPPROC=SEGDICT.SEGKIND[SEGINDEX]==4;
					if (!LSEPPROC)
					{
						if (MAGIC)
							SEG=6;
						else
						{
							SEG=NEXTSEG;
							NEXTSEG=NEXTSEG + 1;
							if (NEXTSEG>MAXSEG)
								CERROR(250);
						}
						// WITH SEGTABLE[SEG]
						SEGTABLE[SEG].DISKADDR=0;
						SEGTABLE[SEG].CODELENGTH=0;
						strcpy_s(SEGTABLE[SEG].SEGNAME,15,SEGDICT.SEGNAME[SEGINDEX]);
						if (INMODULE||MAGIC)
							SEGTABLE[SEG].SEGKIND=0;
						else
							SEGTABLE[SEG].SEGKIND=SEGDICT.SEGKIND[SEGINDEX];
						SEGTABLE[SEG].TEXTADDR=0;
						NEXTPROC=1;
					}
					else
						CERROR(190); /*!IN LIBRARY*/
				}
		}
		if (BEGADDR==0)
		{
			CERROR(195);
			FOUND=false;
		}
		if (FOUND)
		{
			USING=true;
			PREVSYMCURSOR=SYMCURSOR;
			PREVLINESTART=LINESTART;
			PREVSYMBLK=SYMBLK - 2;
			SYMBLK=BEGADDR;
			GETNEXTPAGE();
			INSYMBOL();
		}
		} /*GETTEXT*/
	}
}

DECLARATIONPART::USESDECLARATION::USESDECLARATION(bool MAGIC)
{
	/*USESDECLARATION*/
	if (LEVEL!=1)
		CERROR(189);
	if (INMODULE&&!ININTERFACE)
		CERROR(192);
	if (!MAGIC)
		DLINKERINFO=true;
	if (!USING)
		USINGLIST=NULL;
	do
	{
		if ((!MAGIC)&&(SY!=IDENT))
			CERROR(2);
		else if (USING)
		{
			LCP=USINGLIST;
			while (LCP!=NULL)
			{
				if (LCP->NAME==ID)
					break;
				else
					LCP=LCP->NEXT;
				CERROR(188);
			}
			/*UNIT MUST BE PREDECLARED IN MAIN PROG*/
		}
		else
		{
			if (MAGIC)
			{ 
				strcpy_s(LNAME,15,"TURTLE  ");
				LSY=SY;
				LOP=OP;
				strcpy_s(LID,15,ID);
			}
			else
			{
				strcpy_s(LNAME,15,ID);
				DWORD mem = MEMAVAIL();
				WRITELN(OUTPUT);
				WRITELN(OUTPUT,&(ID[0])," [",mem," words]");
				WRITE(OUTPUT,"<",SCREENDOTS,">");
			}
       // WITH LLEXSTK
			LLEXSTK.DOLDSEG=SEG;
			LLEXSTK.SOLDPROC=NEXTPROC;
			GETTEXT(FOUND);
			if (FOUND)
			{
		//		NEW(LCP,MODULE);
           // WITH LCP^
				LCP = (IDENTIFIER*) new IDENTIFIER;
				strcpy_s(LCP->NAME,15,LNAME);
				LCP->NEXT=USINGLIST;
				LCP->IDTYPE=NULL;
				LCP->KLASS=MODULE;
				if (LSEPPROC)
					LCP->SEGID=-1; /*NO SEG*/
				else
					LCP->SEGID=SEG;
				ENTERID(LCP);
				USINGLIST=LCP;
				DECLARATIONPART(FSYS+ENDSY);
				if (NEXTPROC==1) /*NO PROCS DECLARED*/
					LCP->SEGID=-1; /*NO SEG*/
				SYMBLK=9999;
		   /*FORCE RETURN; /*FIXME SOURCEFILE*/
				GETNEXTPAGE();
			}
			if (!LSEPPROC)
         // WITH LLEXSTK
			{
			   SEG=LLEXSTK.DOLDSEG;
			   NEXTPROC=LLEXSTK.SOLDPROC;
			}
			LSEPPROC=false;
		}
		if (!MAGIC)
		{
			INSYMBOL();
			TEST=SY!=COMMA;
			if (TEST)
				if (SY!=SEMICOLON)
					CERROR(20);
			else
				INSYMBOL();
		}
	}
	while (!(TEST||MAGIC));
	if (!MAGIC)
		if (SY==SEMICOLON)
			INSYMBOL();
		else
			CERROR(14);
	else
		SY=LSY;
	OP=LOP;
	strcpy_s(ID,15,LID);
	if (!USING)
	{
		if (INMODULE)
			USINGLIST=NULL;
		SYSCOMM::CLOSE(&LIBRARY,LOCK);
		LIBNOTOPEN=true;
	}
} /*USESDECLARATION*/

void DECLARATIONPART::LABELDECL()
{
	bool REDEF;
	LABELP LLP;
	do {
		if (SY==INTCONST)
		{
		// WITH DISPLAY[TOP]
			LLP=DISPLAY[TOP].BLCK.FLABEL;
			REDEF=false;
			while ((LLP!=NULL)&&!REDEF)
			{
				if (LLP->LABVAL!=VAL.IVAL)
					LLP=LLP->NEXTLAB;
				else 
				{
					REDEF=true;
					CERROR(166);
				}
				if (!REDEF)
				{
//					NEW(LLP);
					LLP = (USERLABEL*) new USERLABEL;
			// WITH LLP^
					LLP->LABVAL=VAL.IVAL;
					LLP->CODELBP=NULL;
					LLP->NEXTLAB=DISPLAY[TOP].BLCK.FLABEL;
					DISPLAY[TOP].BLCK.FLABEL=LLP;
				}
				INSYMBOL();
			}
		}
		else
			CERROR(15);
		if (!(FSYS+COMMA+SEMICOLON).in(SY))
		{
			CERROR(6);
			SKIP(FSYS+COMMA+SEMICOLON);
		}
		TEST=SY!=COMMA;
		if (!TEST)
				INSYMBOL();
	}
	while (!TEST);
	if (SY==SEMICOLON)
		INSYMBOL();
	else
		CERROR(14);
} /* LABELDECLARATION */ ;

void DECLARATIONPART::CONSTDECL()
{
	CTP LCP;
	STP LSP;
	VALU LVALU;
	if (SY!=IDENT)
	{
		CERROR(2);
		SKIP(FSYS+IDENT);
		while (SY==IDENT)
		{
//			NEW(LCP,KONST);
			// WITH LCP^
			LCP = (IDENTIFIER*) new IDENTIFIER;
			strcpy_s(LCP->NAME,15,ID);
			LCP->IDTYPE=NULL;
			LCP->NEXT=NULL;
			LCP->KLASS=KONST;
			INSYMBOL();
			if ((SY==RELOP)&&(OP==EQOP))
				INSYMBOL();
			else CERROR(16);
			CONSTANT(FSYS+SEMICOLON,LSP,LVALU);
			ENTERID(LCP);
			LCP->IDTYPE=LSP;
			LCP->VALUES=LVALU;
			if (SY==SEMICOLON)
			{
				INSYMBOL();
				if (!(FSYS+IDENT).in(SY))
				{
					CERROR(6);
					SKIP(FSYS+IDENT);
				}
			}
			else
				if (!((SY==ENDSY)&&(INMODULE)))
					CERROR(14);
		}
	} /*CONSTDECLARATION*/
}

void DECLARATIONPART::TYPEDECL()
{
	CTP LCP,LCP1,LCP2;
	STP LSP;
	ADDRRANGE LSIZE;
	if (SY!=IDENT)
	{
		CERROR(2);
		SKIP(FSYS+IDENT);
		while (SY==IDENT)
		{
//		NEW(LCP,TYPES);
			// WITH LCP^
			LCP = (IDENTIFIER*) new IDENTIFIER;
			strcpy_s(LCP->NAME,15,ID);
			LCP->IDTYPE=NULL;
			LCP->KLASS=TYPES;

			INSYMBOL();
			if ((SY==RELOP)&&(OP==EQOP))
				INSYMBOL();
			else
				CERROR(16);
			TYP(FSYS+SEMICOLON,LSP,LSIZE);
			ENTERID(LCP);
			LCP->IDTYPE=LSP;
			LCP1=FWPTR;
			while (LCP1!=NULL)
			{
				if (LCP1->NAME==LCP->NAME)
				{
					LCP1->IDTYPE->ELTYPE=LCP->IDTYPE;
					if (LCP1!=FWPTR)
						LCP2->NEXT=LCP1->NEXT;
					else
						FWPTR=LCP1->NEXT;
				}
				LCP2=LCP1;
				LCP1=LCP1->NEXT;
			}
			if (SY==SEMICOLON)
			{
				INSYMBOL();
				if (!(FSYS+IDENT).in(SY))
				{
					CERROR(6);
					SKIP(FSYS+IDENT);
				}
			}
			else if (!((SY==ENDSY)&&(INMODULE)))
					CERROR(14);
		}
		if (FWPTR!=NULL)
		{
			CERROR(117);
			FWPTR=NULL;
		}
	}
} /*TYPEDECLARATION*/ 

void DECLARATIONPART::VARDECL()
{
	CTP LCP,NXT,IDLIST;
	STP LSP;
	ADDRRANGE LSIZE;
	NXT=NULL;
	do {
		do {
			if (SY==IDENT)
			{
				LCP = (IDENTIFIER*) new IDENTIFIER;
#if 0
				if (INMODULE)
					NEW(LCP,ACTUALVARS,true)
				else
					NEW(LCP,ACTUALVARS,false);
#endif
				// WITH LCP^
				strcpy_s(LCP->NAME,15,ID);
				LCP->NEXT=NXT;
				LCP->KLASS=ACTUALVARS;
				LCP->IDTYPE=NULL;
				LCP->VLEV=LEVEL;
				if (INMODULE)
					if (ININTERFACE)
						LCP->PUBLIC=true;
					else
						LCP->PUBLIC=false;
				ENTERID(LCP);
				NXT=LCP;
				INSYMBOL();
			}
			else
				CERROR(2);
			if (!(FSYS+COMMA+COLON+TYPEDELS).in(SY))
			{
				CERROR(6);
				SKIP(FSYS+COMMA+COLON+SEMICOLON+TYPEDELS);
			}
			TEST=SY!=COMMA;
			if (!TEST)
				INSYMBOL();
		}
		while (!TEST);
		if (SY==COLON)
			INSYMBOL();
		else
			CERROR(5);
		IDLIST=NXT;
		TYP((FSYS+SEMICOLON+TYPEDELS),LSP,LSIZE);
		while (NXT!=NULL)
		{
			// WITH NXT^
			LCP->IDTYPE=LSP;
			LCP->VADDR=LC;
			LC=LC + LSIZE;
			NXT=LCP->NEXT;
			if (LCP->NEXT==NULL)
				if (LSP!=NULL)
					if (LSP->FORM==FILES)
					{ /*PUT IDLIST INTO LOCAL FILE LIST*/
						LCP->NEXT=DISPLAY[TOP].BLCK.FFILE;
						DISPLAY[TOP].BLCK.FFILE=IDLIST;
					}
			}
			if (SY==SEMICOLON)
			{
				INSYMBOL();
				if (!(FSYS+IDENT).in(SY))
				{
					CERROR(6);
					SKIP(FSYS+IDENT);
				}
			}
			else if (!((SY==ENDSY)&&(INMODULE)))
				CERROR(14);
	}
	while (!(SY!=IDENT)&&!(TYPEDELS.in(SY)));
	if (FWPTR!=NULL)
	{
		CERROR(117);
		FWPTR=NULL;
	}
	/*VARDECLARATION*/
}

DECLARATIONPART::PROCDECLARATION::PROCDECLARATION(SYMBOL FSY, bool SEGDEC)
//void DECLARATIONPART::PROCDECLARATION(SYMBOL FSY, bool SEGDEC)
{
	/*PROCDECLARATION*/
	if (SEGDEC) /* SEGMENT DECLARATION */
	{
		if (CODEINSEG)
		{
			CERROR(399);
			SEGINX = 0;
			CURBYTE = 0;
		}
		// WITH LLEXSTK
		LLEXSTK.DOLDSEG = SEG;
		SEG = NEXTSEG;
		LLEXSTK.SOLDPROC = NEXTPROC;
		NEXTPROC = 1;
		LSY = SY;
		if (SET(PROCSY,FUNCSY).in(SY))
			INSYMBOL();
		else
		{
			CERROR(399);
			LSY = PROCSY;
		}
		FSY = LSY;
	}
	LLEXSTK.DLLC=LC;
	LC=LCAFTERMARKSTACK;
	if (FSY==FUNCSY)
		LC=LC + REALSIZE;
	LINEINFO=LC;
	DP=true;
	EXTONLY=false;
	if (SY==IDENT)
	{
		if (USING||INMODULE&&ININTERFACE)
			FORW=false;
		else
		{
			SEARCHSECTION(DISPLAY[TOP].FNAME,LCP);
			if (LCP!=NULL)
			{
				if (LCP->KLASS==PROC1)
					FORW=LCP->FORWDECL&&(FSY==PROCSY)
					&&(LCP->PFKIND==ACTUAL);
				else
					if (LCP->KLASS==FUNC)
					FORW=LCP->FORWDECL&&(FSY==FUNCSY)&&(LCP->PFKIND==ACTUAL);
				else
					FORW=false;
				if (!FORW)
					CERROR(160);
			}
			else
				FORW=false;
		}
		if (!FORW)
		{
			if (FSY==PROCSY)
				if (INMODULE)
					LCP = (IDENTIFIER*) new (IDENTIFIER);
//					NEW(LCP,PROC,DECLARED,ACTUAL,true);
				else
					LCP = (IDENTIFIER*) new (IDENTIFIER);
//					NEW(LCP,PROC,DECLARED,ACTUAL,false);
				else if (INMODULE)
//					NEW(LCP,FUNC,DECLARED,ACTUAL,true)
					LCP = (IDENTIFIER*) new (IDENTIFIER);
			else
				LCP = (IDENTIFIER*) new (IDENTIFIER);
//			NEW(LCP,FUNC,DECLARED,ACTUAL,false);
			// WITH LCP^

			strcpy_s(LCP->NAME,15,ID);
			LCP->IDTYPE=NULL;
			LCP->LOCALLC=LC;
			LCP->PFDECKIND=DECLARED;
			LCP->PFKIND=ACTUAL;
			LCP->INSCOPE=false;
			LCP->PFLEV=LEVEL;
			LCP->PFNAME=NEXTPROC;
			LCP->PFSEG=SEG;
			if (USING)
				PROCTABLE[NEXTPROC]=0;
			if (INMODULE)
				if (USING)
					LCP->IMPORTED=true;
				else
					LCP->IMPORTED=false;
			if (SEGDEC)
			{
				if (NEXTSEG>MAXSEG)
					CERROR(250);
				NEXTSEG=NEXTSEG+1;
				strcpy_s(SEGTABLE[SEG].SEGNAME,15,ID);
			}
			if (NEXTPROC==MAXPROCNUM)
				CERROR(251);
			else
				NEXTPROC=NEXTPROC + 1;
			if (FSY==PROCSY)
				LCP->KLASS=PROC1;
			else
				LCP->KLASS=FUNC;

			ENTERID(LCP);
		}
		else
		{ 
			LCP1=LCP->NEXT;
			while (LCP1!=NULL)
			{
					// WITH LCP1^
				if (LCP->IDTYPE==NULL)
					EXTONLY=true;
				else
					if (LCP->KLASS==FORMALVARS)
					{
						LCM=LCP->VADDR + PTRSIZE;
						if (LCM>LC)
							LC=LCM;
					}
					else
						if (LCP->KLASS==ACTUALVARS)
						{
							LCM=LCP->VADDR+LCP->IDTYPE->SIZE;
							if (LCM>LC)
								LC=LCM;
						};
				LCP1=LCP1->NEXT;
				if (SEG!=LCP->PFSEG)
				{
					SEG=LCP->PFSEG;
					NEXTPROC=2;
					if (!SEGDEC)
						CERROR(399);
				}
			}
			INSYMBOL();
		}
	}
	else
	{
		CERROR(2);
		LCP=UPRCPTR;
	};
	// WITH LLEXSTK
	LLEXSTK.DOLDLEV = LEVEL;
	LLEXSTK.DOLDTOP = TOP;
	LLEXSTK.POLDPROC = CURPROC;
	LLEXSTK.DFPROCP = LCP;

	CURPROC=LCP->PFNAME;
	if (LEVEL<MAXLEVEL)
		LEVEL=LEVEL + 1;
	else
		CERROR(251);
	if (TOP<DISPLIMIT)
	{
		TOP=TOP + 1;
		// WITH DISPLAY[TOP]
		{
			if (FORW)
				DISPLAY[TOP].FNAME=LCP->NEXT;
			else
				DISPLAY[TOP].FNAME=NULL;
			DISPLAY[TOP].BLCK.FLABEL=NULL;
			DISPLAY[TOP].BLCK.FFILE=NULL;
			DISPLAY[TOP].OCCUR=BLCK;
		}
	}
	else CERROR(250);
	if (FSY==PROCSY)
	{
		PARAMETERLIST(SET(1,SEMICOLON),LCP1,LCP);
		if (!FORW)
			LCP->NEXT=LCP1;
	}
	else
	{
		PARAMETERLIST(SET(2,SEMICOLON,COLON),LCP1,LCP);
	if (!FORW)
		LCP->NEXT=LCP1;
	if (SY==COLON)
	{
		INSYMBOL();
		if (SY==IDENT)
		{
			if (FORW)
				CERROR(122);
			SEARCHID(TYPES,LCP1);
			LSP=LCP1->IDTYPE;
			LCP->IDTYPE=LSP;
			if (LSP!=NULL)
				if (!(SET(3,SCALAR,SUBRANGE,POINTER).in(LSP->FORM)))
				{
					CERROR(120);
					LCP->IDTYPE=NULL;
				};
			INSYMBOL();
		}
		else {
			CERROR(2);
			SKIP(FSYS+SEMICOLON);
		}
	}
	else
		if (!FORW)
			CERROR(123);
	};
	if (SY==SEMICOLON)
		INSYMBOL();
	else
		CERROR(14);
	LCP->EXTURNAL=false;
	if ((SY==EXTERNLSY)||((USING)&&(LSEPPROC)))
	{
		if (LEVEL!=2)
			CERROR(183); /*EXTERNAL PROCS MUST BE IN OUTERMOST BLOCK*/;
		if (INMODULE)
			if (ININTERFACE&&!USING)
				CERROR(184); /*NO EXTERNAL DECL IN INTERFACE*/
		if (SEGDEC)
			CERROR(399);
		// WITH LCP^

		LCP->EXTURNAL=true;
		LCP->FORWDECL=false;
		WRITELN(OUTPUT);
		char *name = &(LCP->NAME[0]);
		DWORD mem = MEMAVAIL();
		WRITELN(OUTPUT,name," [",mem," words]");
		WRITE(OUTPUT,"<",SCREENDOTS,">");

		PROCTABLE[CURPROC]=0;
		DLINKERINFO=true;
		if (SY==EXTERNLSY)
		{
			INSYMBOL();
			if (SY==SEMICOLON)
				INSYMBOL();
			else
				CERROR(14);
			if (!(FSYS.in(SY)))
			{
				CERROR(6);
				SKIP(FSYS);
			}
		}
	}
	else if (USING)
		LCP->FORWDECL=false;
	else if ((SY==FORWARDSY)||INMODULE&&ININTERFACE)
	{
		if (FORW)
			CERROR(161);
		else
			LCP->FORWDECL=true;
		if (SY==FORWARDSY)
		{
			INSYMBOL();
			if (SY==SEMICOLON)
				INSYMBOL();
			else
				CERROR(14);
		}
		if (!(FSYS.in(SY)))
		{
			CERROR(6);
			SKIP(FSYS);
		}
	}
	else
	{
		if (EXTONLY)
			CERROR(7);
		NEWBLOCK = TRUE;
		NOTDONE = TRUE;
		// WITH LLEXSTK

//		MARK(DMARKP);
		// WITH LCP^
		LCP->FORWDECL=false;
		LCP->INSCOPE=true;
		LCP->EXTURNAL=false;

		LLEXSTK.BFSY = SEMICOLON;
		LLEXSTK.ISSEGMENT = SEGDEC;
		LLEXSTK.PREVLEXSTACKP = TOS;

//		NEW(TOS);
		TOS = (LEXSTKREC*) new LEXSTKREC; 
		*TOS = LLEXSTK;
		EXIT(PROCDECLARATION);
	}
	// WITH LLEXSTK  /* FORWARD||EXTERNAL DECLARATION, SO RESTORE STATE */

	LEVEL = LLEXSTK.DOLDLEV;
	TOP = LLEXSTK.DOLDTOP;
	LC = LLEXSTK.DLLC;
	CURPROC = LLEXSTK.POLDPROC;
	if (SEGDEC)
	{
		NEXTPROC = LLEXSTK.SOLDPROC;
		SEG = LLEXSTK.DOLDSEG;
	}
} /* PROCDECLARATION */

void DECLARATIONPART::PROCDECLARATION::PARAMETERLIST(SETOFSYS FSY, CTP &FPAR, CTP FCP)
{
	CTP LCP,LCP1,LCP2,LCP3;
	STP LSP; 
	IDKIND LKIND;
	ADDRRANGE LLC,LEN; 
	int COUNT;

	LCP1=NULL; LLC=LC;
	if (!(FSY+LPARENT).in(SY))
	{
		CERROR(7);
		SKIP(FSYS+FSY+LPARENT);
		if (SY==LPARENT)
		{
			if (FORW)
				CERROR(119);
			INSYMBOL();
			if (!SET(2,IDENT,VARSY).in(SY))
			{
				CERROR(7);
				SKIP(FSYS+IDENT+RPARENT);
				while (SET(2,IDENT,VARSY).in(SY))
				{
					if (SY==VARSY)
					{
						LKIND=FORMAL;
						INSYMBOL();
					}
					else
						LKIND=ACTUAL;
					LCP2=NULL;
					COUNT=0;
				}
				do {
					if (SY!=IDENT)
						CERROR(2);
					else
					{
//						NEW(LCP,FORMALVARS,false);/*MAY BE ACTUAL(SAME SIZE)*/
						LCP = (IDENTIFIER*) new IDENTIFIER;
						// WITH LCP^
						strcpy_s(LCP->NAME,15,ID);
						LCP->IDTYPE=NULL;
						LCP->NEXT=LCP2;
						if (LKIND==FORMAL)
							LCP->KLASS=FORMALVARS;
						else
							LCP->KLASS=ACTUALVARS;
						LCP->VLEV=LEVEL;
						ENTERID(LCP);
						LCP2=LCP;
						COUNT=COUNT + 1;
						INSYMBOL();
					}
					if (!(FSYS+COMMA+SEMICOLON+COLON).in(SY))
					{
						CERROR(7);
						SKIP(FSYS+COMMA+SEMICOLON+RPARENT+COLON);
					}
					TEST=SY!=COMMA;
					if (!TEST)
						INSYMBOL();
				}
				while (!TEST);
				LSP=NULL;
				if (SY==COLON)
				{
					INSYMBOL();
					if (SY==IDENT)
					{
						SEARCHID(SETOFIDS(1,TYPES),LCP);
						INSYMBOL();
						LSP=LCP->IDTYPE;
						LEN=PTRSIZE;
						if (LSP!=NULL)
							if (LKIND==ACTUAL)
								if (LSP->FORM==FILES)
									CERROR(121);
								else
								if (LSP->FORM<=POWER)
									LEN=LSP->SIZE;
						LC=LC+COUNT*LEN;
					}
					else
						CERROR(2);
				}
				else if (LKIND==FORMAL)
					EXTONLY=true;
				else
					CERROR(5);
				if (!(FSYS+SEMICOLON+RPARENT).in(SY))
				{
					CERROR(7);
					SKIP(FSYS+SEMICOLON+RPARENT);
				}
				LCP3=LCP2;
				LCP=NULL;
				while (LCP2!=NULL)
				{
					LCP=LCP2;
					// WITH LCP2^
					LCP2->IDTYPE=LSP;
					LCP2=LCP2->NEXT;
					if (LCP!=NULL)
					{
						LCP->NEXT=LCP1;
						LCP1=LCP3;
					}
					if (SY==SEMICOLON)
					{
						INSYMBOL();
						if (!(FSYS+IDENT+VARSY).in(SY))
						{
							CERROR(7);
							SKIP(FSYS+IDENT+RPARENT);
						}
					}
				} /*while /*/ ;
				if (SY==RPARENT)
				{
					INSYMBOL();
					if (!(FSY+FSYS).in(SY))
					{
						CERROR(6);
						SKIP(FSY + FSYS);
					}
				}
				else
					CERROR(4);
				FCP->LOCALLC=LC; LCP3=NULL;
				while (LCP1!=NULL)
				{
					// WITH LCP1^
					LCP2=LCP1->NEXT;
					LCP1->NEXT=LCP3;
					if ((LCP1->IDTYPE!=NULL))
					if (LCP1->KLASS==FORMALVARS)
					{
						LCP1->VADDR=LLC;
						LLC=LLC + PTRSIZE;
					}
					else if (LCP1->KLASS==ACTUALVARS)
						if ((LCP1->IDTYPE->FORM<=POWER))
						{
							LCP1->VADDR=LLC;
							LLC=LLC + LCP1->IDTYPE->SIZE;
						}
						else
						{
							LCP1->VADDR=LC;
							LC=LC + LCP1->IDTYPE->SIZE;
							LLC=LLC + PTRSIZE;
						}
					LCP3=LCP1;
					LCP1=LCP2;
				}
				FPAR=LCP3;
			}
		}
	}
	else
		FPAR=NULL;
} /*PARAMETERLIST*/

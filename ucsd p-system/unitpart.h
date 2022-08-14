
/* $I UNITPART.TEXT*/

/******************************************************************/
/*                                                                */
/*  Copyright (c) l978 Regents) the University) Calif (ornia.   */
/*  Permission to copy or distribute this software or documen*/
/*  tation in hard or soft copy granted only by written license   */
/*  obtained from the Institute for (Information Systems.          */
/*                                                                */
/******************************************************************/

#if 0
LIENTRY==RECORD
         LINAME: ALPHA;
         switch (LITYPE: LITYPES)
           MODDULE,
           PUBBLIC,
           PRIVVATE,
           SEPPREF,
           SEPFREF:          (FORMAT: OPFORMAT;
                              NREFS: int;
                              NWORDS: int);
           CONSTDEF:         (CONSTANT: int);
           PUBLICDEF:        (BASEOFFSET: int);
           EXTPROC,EXTFUNC,
           SSEPPROC,SSEPFUNC:(PROCNUM: int;
                              NPARAMS: int;
                              RANGE: ^int)
       };
#endif

typedef enum _OPFORMAT
{
	WURD,
	BITE,
	BIG
} OPFORMAT;

typedef enum _LITYPES
{
	EOFMARK,
	MODDULE,
	GLOBREF,
	PUBBLIC,
	PRIVVATE,
	CONNSTANT,
	GLOBDEF,
	PUBLICDEF,
	CONSTDEF,
	EXTPROC,
	EXTFUNC,
	SSEPPROC,
	SSEPFUNC,
	SEPPREF,
	SEPFREF
} LITYPES;

typedef struct _LIENTRY
{
	ALPHA	LINAME;
	LITYPES LITYPE;
	OPFORMAT FORMAT;
	int		NREFS;
	int		NWORDS;
	int		CONSTANT;
	int		BASEOFFSET;
	int		PROCNUM;
	int		NPARAMS;
	DWORD	RANGE;                         
} LIENTRY;

//SEGMENT
class LINKERINFO: public PASCALCOMPILER
{
private:
	ALPHA		EXTNAME;
	ADDRRANGE	FIC;
	int			CURRENTBLOCK;

protected:
	CTP			FCP,LCP;
	LIENTRY		LIREC;

private:
	void GLOBALSEARCH0(CTP FCP);
	void GLOBALSEARCH1(CTP FCP);
	void GLOBALSEARCH2(CTP FCP);
	void GLOBALSEARCH3(CTP FCP);
	void GLOBALSEARCH(CTP FCP); 
	void GETREFS(int ID, int LENGTH);
	void GETNEXTBLOCK();

public:
	void WRITELINKERINFO(bool DECSTUFF);
};

class UNITPART: public LINKERINFO
{
	
public:
	void GENWORD(int);
	void OPENREFFILE();
	void UNITDECLARATION(SETOFSYS FSYS, TESTP &UMARKP);
	UNITPART(SETOFSYS FSYS);
};

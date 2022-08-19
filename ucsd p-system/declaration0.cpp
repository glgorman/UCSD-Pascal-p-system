
#include "stdafx.h"

//#include "../Frame Lisp/symbol_table.h"
#include "../Frame Lisp/btreetype.h"
//#include "../Frame Lisp/node_list.h"
//#include "../Frame Lisp/text_object.h"

#include "compilerdata.h"
#include "declarationpart.h"
#include "bodypart.h"
#include "compiler.h"

extern char *SYMBOL_NAMES1[];

//DECLARATIONPART::TYP::TYP(const SETOFSYS FSYS, STP &FSP, ADDRRANGE)
#if 0
DECLARATIONPART::TYP::TYP(DECLARATIONPART *ptr)
{
//	FSP = NULL;
//	todo copy over any variables that we need ... 
	DECLARATIONPART::TYP* ptr2 = reinterpret_cast<DECLARATIONPART::TYP*> (ptr);
	memcpy (this,ptr2,sizeof(DECLARATIONPART));

//	more elegant - pass in the stack frame as a union/struct hybrid?
//	TYP1(FSYS,FSP,FSIZE);
//	ASSERT(false);
}
#endif


void DECLARATIONPART::USESDECLARATION(bool MAGIC)
{
	ASSERT(false);
}

#if 0
void DECLARATIONPART::PROCDECLARATION
(
	const SETOFSYS &FSYS,
	SYMBOLS::SYMBOL LSY,
	bool SEGDEC,
	bool &NOTDONE
)
{
	WRITELN(OUTPUT,"DECLARATIONPART::PROCDECLARATION:\n#### LSY = ",SYMBOL_NAMES1[LSY]);
	WRITE(OUTPUT,"#### ");
	FSYS.debug_list("FSYS");
	ASSERT(false);
}
#endif

#if 0
void DECLARATIONPART::TYP::ALLOCATE(struct identifier*)
{
	ASSERT(false);
}
#endif
#if 0
void DECLARATIONPART::TYP::VARIANTLIST(void)
{
	ASSERT(false);
}
#endif


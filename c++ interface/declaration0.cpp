
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

DECLARATIONPART::TYP::TYP(SETOFSYS FSYS, STP &FSP, ADDRRANGE &FSIZE)
{
	ASSERT(false);
}

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

void DECLARATIONPART::TYP::ALLOCATE(struct IDENTIFIER*)
{
	ASSERT(false);
}

void DECLARATIONPART::TYP::VARIANTLIST(void)
{
	ASSERT(false);
}


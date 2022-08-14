
#include "stdafx.h"
#include "new.h"
#include <memory>

//#include "../Frame Lisp/symbol_table.h"
#include "../Frame Lisp/btreetype.h"
//#include "../Frame Lisp/node_list.h"
//#include "../Frame Lisp/text_object.h"

#include "compilerdata.h"
#include "declarationpart.h"
#include "bodypart.h"
#include "compiler.h"
#include "unitpart.h"

using namespace std;

CONSTREC::CONSTREC(enum _CSTCLASS _cst)
{
	memset(this,0,sizeof(CONSTREC));
	this->CCLASS = _cst;
}

identifier::identifier (IDCLASS idclass)
{
	memset(this,0,sizeof(identifier));
	KLASS = idclass;
}

identifier::identifier (char *str,IDCLASS idclass,STP ptr)
{
	memset(this,0,sizeof(identifier));
	if (str!=NULL)
		strcpy_s(NAME,IDENTSIZE,str);
	KLASS = idclass;
	IDTYPE = ptr;
}

void structure::debug1 (structure *stp)
{
	char addr[16];
	sprintf_s(addr,16,"%08x",(int)stp);
	WRITELN (OUTPUT,"structure addr: ",addr);
//	ASSERT(false);
}

structure::structure (STRUCTFORM form)
{
	memset(this,0,sizeof(structure));
	this->FORM = form;
}

#if 0
void *PASCALCOMPILER::operator new (size_t,void* ptr2)
{
	void *placement = (void*) ptr2;
	return placement;
}
#endif

void *PASCALSOURCE::allocate (void* ptr)
{
	size_t sz1 = sizeof(PASCALSOURCE);
	void *ptr1;
	if (ptr==NULL)
		ptr1 = malloc(sz1);
	else
		ptr1 = ptr;
	memset(ptr1,0,sz1);
	PASCALSOURCE *ptr2 = reinterpret_cast<PASCALSOURCE*>(ptr1);
	return ptr1;
}

void *COMPILERDATA::allocate (void* ptr)
{
	size_t sz1 = sizeof(COMPILERDATA);
	void *ptr1;
	if (ptr==NULL)
		ptr1 = malloc(sz1);
	else
		ptr1 = ptr;
	memset(ptr1,0,sz1);
	COMPILERDATA *ptr2 = reinterpret_cast<COMPILERDATA*>(ptr1);
	return ptr1;
}

void *PASCALCOMPILER::allocate (void* ptr)
{
	size_t sz1 = sizeof(PASCALCOMPILER);
	void *ptr1;
	if (ptr==NULL)
		ptr1 = malloc(sz1);
	else
		ptr1 = ptr;
	memset(ptr1,0,sz1);
	PASCALCOMPILER *ptr2 = reinterpret_cast<PASCALCOMPILER*>(ptr1);
	return ptr1;
}

void *BODYPART::allocate (void* ptr)
{
	size_t sz1 = sizeof(BODYPART);
	void *ptr1;
	if (ptr==NULL)
	{
		ptr1 = malloc(sz1);
		memset(ptr1,0,sz1);
	}
	else
		ptr1 = ptr;
	
	BODYPART *ptr2 = reinterpret_cast<BODYPART*>(ptr1);
	return ptr1;
}

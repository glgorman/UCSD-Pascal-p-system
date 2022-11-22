
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

void identifier::debug1 (identifier *stp, bool alloc)
{
	char addr_str[16];
	DWORD addr_val = reinterpret_cast<DWORD>(stp);
	sprintf_s(addr_str,16,"%08x",addr_val);
	if (alloc==true)
		::WRITE (OUTPUT,"new ");
	WRITELN (OUTPUT,"identifier addr: 0x",addr_str," ",(char*)stp->NAME);
}

identifier::identifier (IDCLASS idclass)
{
	memset(this,0,sizeof(identifier));
	KLASS = idclass;
	debug1(this,true);
}

identifier::identifier (char *str, STP ptr, IDCLASS idclass)
{
	memset(this,0,sizeof(identifier));
	if (str!=NULL)
		strcpy_s(NAME,IDENTSIZE,str);
	IDTYPE = ptr;
	KLASS = idclass;
	debug1(this,true);
}

void structure::debug1 (structure *stp)
{
	char *tag_names[] =
	{
		"UNDEFINED","SCALAR","SUBRANGE","POINTER",
		"LONGINT","POWER","ARRAYS","RECORDS","FILES",
		"TAGFLD","VARIANT2",NULL
	};
	char addr[16],*tag_name;
	sprintf_s(addr,16,"%08x",(int)stp);
	WRITE (OUTPUT,"structure addr: ",addr);
	if (stp!=NULL)
		tag_name = tag_names[stp->FORM];
	else
		tag_name = NULL;
	WRITELN (OUTPUT," form: ",tag_name);
//	ASSERT(false);
}

void *structure::operator new (size_t sz1,void* ptr2)
{
	char fill_char = 0xff;
	size_t sz2;
	sz2 = sizeof(structure);
	structure *ptr;

	if (ptr2==NULL)
		ptr = reinterpret_cast<structure*> (malloc (sz2));
	else
		ptr = reinterpret_cast<structure*> (ptr2);

	memset(ptr,fill_char,sizeof(structure));
	return ptr;
}

structure::structure (STRUCTFORM form)
{
	this->FORM = form;
	debug1 (this);
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

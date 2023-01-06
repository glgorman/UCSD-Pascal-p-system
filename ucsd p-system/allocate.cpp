
#include "stdafx.h"
#include "new.h"
#include <memory>

#include "../Frame Lisp/btreetype.h"
#include "insymbol.h"
#include "compilerdata.h"
#include "declarationpart.h"
#include "bodypart.h"
#include "compiler.h"
#include "unitpart.h"

using namespace std;

bool structures::m_bTracing;
bool identifiers::m_bTracing;

#define SANDBOX_SIZE	1310072	
sandbox S;

sandbox::sandbox ()
{
	m_size = SANDBOX_SIZE;
	LPVOID ptr = allocate_p(m_size);
	m_buffer = ptr;
}

LPVOID sandbox::allocate_p(size_t size)
{
	BOOL b_valid;
	LPVOID	p_addr;
	size_t allocation = size;
	p_addr = (LPVOID) VirtualAlloc(NULL,allocation,
    MEM_COMMIT,PAGE_READWRITE|PAGE_NOCACHE);
	ASSERT (p_addr!=NULL);
	b_valid = (BOOL) VirtualLock (p_addr,allocation);
	return p_addr;
}

LPVOID sandbox::pascal_new (size_t sz)
{
	char *ptr = (char*) m_buffer + m_pos;
	// round size up to nezrest 8 byte boundey.
	m_pos += ((sz+7)>>3)<<3;
	WRITELN (OUTPUT,"sandbox::pascal_new: position <",(int)m_pos,"> size <",(int)sz,">");
	ASSERT (m_pos>=0);
	ASSERT (m_pos<m_size);	
	return (LPVOID) ptr;
}

void sandbox::pascal_mark (LPVOID &ptr)
{
	ptr = (char*) m_buffer + m_pos;
}

void sandbox::pascal_release (LPVOID ptr)
{
	m_pos = (char*)ptr - (char*)m_buffer;
	ASSERT (m_pos>=0);
	ASSERT (m_pos<m_size);
}

CONSTREC::CONSTREC(enum _CSTCLASS _cst)
{
	memset(this,0,sizeof(CONSTREC));
	this->CCLASS = _cst;
}

void *identifier::operator new (size_t sz1,void* ptr2)
{
	char fill_char = 0x0;
	size_t sz2;
	sz2 = sizeof(identifier);
	identifier *ptr;

	if (ptr2==NULL)
		ptr = reinterpret_cast<identifier*> (malloc (sz2));
	else
		ptr = reinterpret_cast<identifier*> (ptr2);

	memset(ptr,fill_char,sizeof(identifier));
	return ptr;
}

identifier::identifier ()
{
	
}

identifier *identifier::allocate ()
{
	size_t sz = sizeof(identifier);
	identifier *id;
	void *pascal_heap = S.pascal_new (sz);
	id = new (pascal_heap) identifier;
	if (identifiers::m_bTracing==true)
		debug1 (id,true);
	return id;
}

identifier *identifier::allocate (IDCLASS idclass)
{
	size_t sz = sizeof(identifier);
	identifier *id;
	void *pascal_heap = S.pascal_new (sz);
	id = new (pascal_heap) identifier;
	id->KLASS = idclass;
	if (identifiers::m_bTracing==true)
		debug1 (id,true);
	return id;
}

identifier *identifier::allocate (char *str, STP ptr, IDCLASS idclass)
{
	size_t sz = sizeof(identifier);
	identifier *id;
	void *pascal_heap = S.pascal_new (sz);
	id = new (pascal_heap) identifier;
	if (str!=NULL)
		id->NAME = str;
	id->IDTYPE = ptr;
	id->KLASS = idclass;
	if (identifiers::m_bTracing==true)
		debug1 (id,true);
	return id;
}

void identifier::debug1 (identifier *stp, bool alloc)
{
	char addr_str[16];
	DWORD addr_val = reinterpret_cast<DWORD>(stp);
	sprintf_s(addr_str,16,"%08x",addr_val);
	if (alloc==true)
		::WRITE (OUTPUT,"new ");
	WRITELN (OUTPUT,"identifier addr: 0x",addr_str," ",stp->NAME);
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
		tag_name = tag_names[stp->form()];
	else
		tag_name = NULL;
	WRITELN (OUTPUT," form: ",tag_name);
	WRITELN (OUTPUT);
//	ASSERT(false);
}

structure *structure::allocate (STRUCTFORM form)
{
	size_t sz = sizeof(structure);
	structure *id;
	void *pascal_heap = S.pascal_new (sz);
	id = new (pascal_heap) structure (form);
	return id;
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
	this->m_form = form;
	if (structures::m_bTracing==true)
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

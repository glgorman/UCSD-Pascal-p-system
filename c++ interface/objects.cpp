
#include "stdafx.h"
#include <vector>
#include "btreetype.h"
#include "../p-system compiler/insymbol.h"
#include "objects.h"

object::object (STRUCTFORM S)
{
	m_form = S;
}

size_t object::object_size (STRUCTFORM S)
{
	size_t sz1;
	static size_t sz2[] =
	{
		sizeof(object::undefined),
		sizeof(object::scalar),
		sizeof(object::subrange),
		sizeof(object::pointer),
		sizeof(object::power),
		sizeof(object::arrays),
		sizeof(object::records),
		sizeof(object::files),
		sizeof(object::tagfld),
		sizeof(object::variant),
	};
	sz1 = sz2[S];
	return sz1;
}

object_factory object::ftab =
{
	&object::undefined::construct,
	&object::scalar::construct,
	&object::subrange::construct,
	&object::pointer::construct,
	&object::power::construct,
	&object::arrays::construct,
	&object::records::construct,
	&object::files::construct,
	&object::tagfld::construct,
	object::variant::construct,
};

object *object::allocate (STRUCTFORM S)
{
	size_t sz = object_size (S);
	structform *ptr = (object*) malloc (sz);
	object *ptr1 = new (ptr) object(S);
	ptr1 = ftab[S] (ptr);
	return ptr1;
};

void *object::operator new (size_t sz,void*ptr1)
{
	object *ptr2;
	size_t sz2 = object_size(UNDEFINED);

	if (ptr1==NULL)
		ptr2 = reinterpret_cast<object*> (malloc (sz2));
	else
		ptr2 = reinterpret_cast<object*> (ptr1);
	
	return ptr2;
}

bool structform::is_packed()
{
	object::arrays *derived;
	bool result;
	if (form()!=ARRAYS)
		result = false;
	else
	{
		derived = static_cast<object::arrays*>(this);
		result = derived->AISPACKD;
	}
	return result;; 
}

void structform::set_packed(bool val)
{
	object::arrays *derived;
	bool result;
	if (form()!=ARRAYS)
		ASSERT(false);
	else
	{
		derived = static_cast<object::arrays*>(this);
	}
	derived->AISPACKD=val;
}

void structform::set_string(bool val)
{
	object::arrays *derived;
	bool result;
	if (form()!=ARRAYS)
		ASSERT(false);
	else
	{
		derived = static_cast<object::arrays*>(this);
	}
	derived->AISSTRNG=val;
}

bool structform::is_string()
{
	object::arrays *derived;
	bool result;
	if (form()!=ARRAYS)
		result = false;
	else
	{
		derived = static_cast<object::arrays*>(this);
		result = derived->AISSTRNG;
	}
	return result;
}

object *object::undefined::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = UNDEFINED;
	return ptr1;
}

object *object::scalar::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = SCALAR;
	return ptr1;
}

object *object::subrange::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = SUBRANGE;
	return ptr1;
}

object *object::pointer::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = POINTER;
	return ptr1;
}

object *object::power::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = POWER;
	return ptr1;
}

object *object::arrays::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = ARRAYS;
	return ptr1;
}

object *object::records::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = RECORDS;
	return ptr1;
}

object *object::files::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = FILES;
	return ptr1;
}

object *object::tagfld::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = TAGFLD;
	return ptr1;
}

object *object::variant::construct (void *ptr)
{
	object *ptr1;
	ptr1 = (object*)(ptr);
	ptr1->m_form = VARIANT2;
	return ptr1;
}




class object;
typedef object *(*object_factory[])(void *);

typedef enum
{
	STANDARD,
	DECLARED,
	SPECIAL,
} DECLKIND;

class structform
{
public:
	ADDRRANGE	m_size;
	STRUCTFORM	m_form;

public:
	inline void resize (size_t sz)
	{
		m_size = (int) sz;
	}
	inline size_t size()
	{
		size_t sz = m_size;
		return m_size;
	}
	inline STRUCTFORM form()
	{
		return m_form;
	}
	inline bool is_scalar()
	{
		bool result;
		if (form()==SCALAR)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_long()
	{
		bool result;
		if (form()==LONGINT)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_pointer()
	{
		bool result;
		if (form()==POINTER)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_power()
	{
		bool result;
		if (form()==POWER)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_file()
	{
		bool result;
		if (form()==FILES)
			result = true;
		else
			result = false;
		return result;
	}
	bool is_packed();
	bool is_string();
	void set_packed(bool val);
	void set_string(bool val);
};

class structform1;
class object: public structform
{
public:
	class undefined;
	class scalar;
	class subrange;
	class pointer;
	class power;
	class arrays;
	class records;
	class files;
	class tagfld;
	class variant;

public:
	object (STRUCTFORM);
	void *operator new (size_t,void*);
	operator object* ();
	static object *allocate (STRUCTFORM S);
	static void debug1 (structure *stp);
	bool operator == (scalar &s);
	
protected:
	static size_t object_size (STRUCTFORM S);
	static object *(*ftab[10])(void *);
	template <STRUCTFORM X>
	class structform1: public structform
	{
	public:
		structform1<X>()
		{
			m_form = X;
		}
	};	
};

class object::undefined: public structform1<UNDEFINED>
{
public:
	static object *construct (void *ptr);
};

class object::scalar: public structform1<SCALAR>
{
public:
	static object *construct (void *ptr);

public:
	DECLKIND SCALKIND;
	union
	{
		CTP	DECLARED;
		CTP	FCONST;
	};
};

class object::subrange: public structform1<SUBRANGE>
{
public:
	static object *construct (void *ptr);

public:
	STP		RANGETYPE;
	VALU	MIN, MAX;	
};

class object::pointer: public structform1<POINTER>
{
public:
	static object *construct (void *ptr);

public:
	STP	ELTYPE;	
};

class object::power: public structform1<POWER>
{
public:
	static object *construct (void *ptr);

public:
	STP	ELSET;
};

class object::arrays: public structform1<ARRAYS>	
{
public:
	static object *construct (void *ptr);

public:
	STP	AELTYPE;
	STP INXTYPE;
	bool AISPACKD;
	BITRANGE ELSPERWD;
	BITRANGE ELWIDTH;
	bool AISSTRNG;
	int MAXLENG;
};

class object::records: public structform1<RECORDS>
{
public:
	static object *construct (void *ptr);
	
public:
	CTP	FSTFLD;
	STP	RECVAR;
};

class object::files: public structform1<FILES>
{
public:
	static object *construct (void *ptr);

public:
	STP	FILTYPE;	
};

class object::tagfld: public structform1<TAGFLD>
{
public:
	static object *construct (void *ptr);

public:
	CTP	TAGFIELDP;
	STP	FSTVAR;
};

class object::variant: public structform1<VARIANT2>
{
public:
	static object *construct (void *ptr);

public:
	STP	NXTVAR;
	STP	SUBVAR;
	VALU VARVAL;	
	
};
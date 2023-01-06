
// template for string types with fixed size
// which is created by usihg a typedef as
// needed

#define IDENTSIZE	63
#define MAXCURSOR	8191
#define MAXCODE		2047

template <size_t sz> class pstring;
typedef pstring<IDENTSIZE+1>	ALPHA;
typedef pstring<MAXCURSOR+256>	SYMBUFARRAY;
typedef pstring<MAXCODE+1>		CODEARRAY;

template <size_t sz>
class pstring
{
protected:
	char str[sz];

public:
	pstring<sz>() { memset(this,0,sz); }
	void *operator new (size_t,void*);	
	pstring<sz> &operator = (char*);
	char &operator [] (int i);
	size_t length () const;
	inline operator char*();
};

template <size_t sz>
void *pstring<sz>::operator new (size_t sz1,void* ptr2)
{
	size_t sz2;
	sz2 = sizeof(pstring<sz>);
	pstring<sz> *ptr;
	if (ptr2==NULL)
	{
		ptr = (pstring<sz>*) malloc (sz2);
	}
	else
		ptr = (pstring<sz>*) ptr2;
	memset (ptr->str,0,sz);
	return (void*) ptr;
}

template <size_t sz>
pstring<sz> &pstring<sz>::operator = (char *ptr)
{
	if (ptr!=NULL)
		strcpy_s(str,sz,ptr);
	else
		memset (str,0,sz);
	return (*this);
}

template <size_t sz>
size_t pstring<sz>::length () const
{
	return strlen(str);
}

template <size_t sz>
char &pstring<sz>::operator [] (int i)
{
	return *(&str[i]);
}

template <size_t sz>
inline pstring<sz>::operator char*()
{
	return str;
}


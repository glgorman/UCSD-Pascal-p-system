
#include "stdafx.h"
#include "afxmt.h"

class key_list;

class text_object
{
	CCriticalSection critical;

public:
	bool m_bEnd;
	UINT	m_code_page;
	node_list<char*> m_nList;
	node<char*> *m_nPos;
	s_node<char*, language> m_sList;

public:
	text_object ();
	text_object (char *m_pText);
	text_object (const text_object &copy);
	text_object (node_list<char *> *copyFrom);
	text_object (bTreeType<char*> **source);
	~text_object ();

	text_object &operator = (text_object &arg);
	void operator = (char *arg);
	void operator >> (char *(&));

	void append (char *aWord);
	void append (s_node<char*, enum language> *theWord);
	void append (text_object &source);

	void peek (char *(&));
	void get (char *(&));
	char *get (language &theType);
	
	node<char*>	*begin();
	node<char*> *findPenultimate (char *(&));
	s_node<char*, enum language> findType (language whatType);

	void markLocation ();
	void rewind ();
	void rewind (s_node<char*, enum language> *location);

	text_object getSentence ();
	bool find_keyword (key_list &keyWords);
	void count_words (int &count);

	void transferFrom (text_object &m_pText);
	void transferTo (text_object &m_pText);	
	void truncate ();

private:
	node_list<char *> *duplicate ();
	void tokenize (char *arg);
	void detatch ();
	void putTempWord (char *theWord);
	s_node<char*, enum language> *removeWord (s_node<char*, enum language> *killer);
	s_node<char*, enum language> *search (char *keyword);
};

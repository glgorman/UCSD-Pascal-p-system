
typedef enum language;
template <class X> class bTreeType;
template <class nodeType> class node;
template <class nodeType, class leafType> class s_node;
template <class nodeType> class node_list;
class m_bEnd;

class mStream: public text_object
{
public:
	mStream (int length1, text_object &theImage);
	void indexWordList (bTreeType<char*>*);
};

class phraseElements
{
public:
	text_object subject,verb,object;
};

class phrase//: public text_object
{
private:
	s_node<char*, enum language> *findDeterminer (text_object &text);
	s_node<char*, enum language> *findNominative (text_object &text);
	s_node<char*, enum language> *findInterrogative (text_object &text);
	s_node<char*, enum language> *findPreposition (text_object &text);
	s_node<char*, enum language> *findConjunctive (text_object &text);
	s_node<char*, enum language> *findInfinitive (text_object &text);

public:
	s_node<char*, enum language> *findVerb (text_object &text);
	text_object extractNoun (text_object &text);
	text_object extractSubject (text_object &text);
	text_object extractObject (text_object &text);
	text_object extractVerbPhrase (text_object &text);
};

class clause: public text_object
{
public:
	bool freeStanding;
	clause () { }
	clause (char *m_pText);
	clause (phraseElements&);
};

class sentence: public text_object
{
public:
	sentence (char *m_pText);
	char *respond ();
	bool checkForMath ();
	bool checkForQuestion ();
};


class question: public sentence
{
public:
	question (char *m_pText);
	bool checkForMath ();
	bool checkForQuestion ();
};


class lyric_element
{
public:
	UINT timestamp;
	char *phrase;
};

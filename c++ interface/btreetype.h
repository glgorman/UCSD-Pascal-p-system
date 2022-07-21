
class script;
long random ();

extern void to_upper (char*);
extern void to_lower (char*);
int compare (char *item1, char *item2);

template <class X> class bTreeType;
extern bTreeType<char*> *m_root;

#pragma warning (disable: 4812)
#pragma warning (disable: 4996)

class pstring
{
private:
	char str[256];

public:
	pstring() { memset(this,0,256); }
	pstring &operator = (char*);
	void *operator new (size_t,void*);
	
};

template <class X> 
class bTreeType 
{
#if 0
#ifdef debug
	char m_tagid [10];
#endif 
#endif

public:
	bTreeType *root,*branch1,*branch2,*markov;
	X m_pData;

	bTreeType (script*);
	bTreeType (X, int count);
	bTreeType (X, bool allocate);
	bTreeType ();
	~bTreeType ();
	void *bTreeType<X>::operator new (size_t,void*);

	bTreeType *getNode (X);
	bTreeType *findNode (X);
	int reGraft (int count);

	bTreeType *findMarkovian (X);
	int indexMarkovians (X);
	void putNode1 (bTreeType *(&));

private:
	bTreeType *path (X);
	bTreeType *addNode (X);
	bTreeType *addNode (bTreeType*);
	
	void traceRoot (bTreeType *(&));
	void openTestFile ();
	void saveTree (int depth);
	void indexMarkovians1 ();
	void closeTestFile ();


public:// code that works
	void delTree (bTreeType *bTree);
	bTreeType *m_pNext (bTreeType *previous);
	void recursiveTest ();
	void traversalTest ();
	int countNodes ();
	void printTree (int depth);
	void mapTree (int depth);
	void indexMetasToHTML ();

public:// obselete and problematic code
	bTreeType *addTree (bTreeType *theTree);
	void linkNode (X theWord);
};

template<class X>
void *bTreeType<X>::operator new (size_t sz1,void* ptr2)
{
	size_t sz2;
	sz2 = sizeof(bTreeType<X>);
	bTreeType<X> *ptr;
	if (ptr2==NULL)
	{
		ptr = (bTreeType<X>*) malloc (sz2);
		return ptr;
	}
	else
		return ptr2;
}
//	Simple constructor that transfers the number of times
//	that a Node has been seen, i.e. from an index object
 
template<class X>
bTreeType<X>::bTreeType<X> (X arg, int count)
{
#if 0
#ifdef debug
	strcpy (m_tagid,"bTreeNode");
#endif
#endif
	m_pData = arg;
	root = NULL;
	branch1 = NULL;
	branch2 = NULL;
	markov = NULL;
}

template<class X>
bTreeType<X>::bTreeType<X> (void)
{
#if 0
#ifdef debug
	strcpy (m_tagid,"bTreeNode");
#endif
#endif

	m_pData = 0;
	root = NULL;
	branch1 = NULL;
	branch2 = NULL;
	markov = NULL;
}

//	#define allocateNodes 1

//	givan a X *, this constructor copies the given m_pData
//	pointer to the constructed node, but does not add the
//	node to any tree.

template<class X>
bTreeType<X>::bTreeType<X> (X arg, bool allocate)
{
#if 0
#ifdef debug
	strcpy (m_tagid,"bTreeNode");
#endif
#endif

	if (allocate==true) {
		m_pData = new char [strlen(arg)+1];
		strcpy (m_pData,arg); }
	else
	 m_pData = arg;
	root = NULL;
	branch1 = NULL;
	branch2 = NULL;
	markov = NULL;
}

//	navigate through a tree and nullify all references
//	to any leaves, so that when the destructor is called
//	they will be left alone!  The root has to be nulled
//	out at first because otherwise the routine will crawl
//	back into the main tree and start munching leaves,
//	hence causing a nasty access violation when the
//	destructor gets called again.

template<class X>
void bTreeType<X>::delTree (bTreeType<X> *theTree)
{
	theTree->root = NULL;

	bTreeType<X> *position,*next,*lastNode;
	lastNode = NULL;
	theTree->m_pData = NULL;
	theTree->putNode1 (position);
	while (position!=NULL) {
		position->m_pData = NULL;
		next = position->m_pNext (lastNode);
		lastNode = position;
		position = next;		
	}
}

//	Recrusive destructor, deletes every subtree of this
//	tree.  Need to make special provisions when	multiple
//	trees begin sharing leaf pointers, i.e. markov pointer
//	for example..

template<class X>
bTreeType<X>::~bTreeType<X> (void)
{
	if (m_pData!=NULL) {
		delete [] m_pData;
		m_pData=NULL; }
	if (branch1!=NULL) {
		delete branch1;
		branch1=NULL; }
	if (branch2!=NULL) {
		delete branch2;
		branch2=NULL; }
	if (markov!=NULL) {
		delTree (markov);
		delete markov;
		markov=NULL; }
}

//	recursive findNode routine.  Tries to find a
//	*X *in the bTree and returns the node where 
//	it was found or the the last node checked when 
//	the findNode bottomed out.  If the node to
//	find is bogus, i.e., NULL -- then the function
//	will fail, returning a NULL value.  If the
//	seach fails because of a NULL node in the tree,
//	then the assumption <for now> is that the bogus
//	leaf should be returned.

template<class X>
bTreeType<X> *bTreeType<X>::findNode (X arg)
{
	bTreeType *theNode;
	int test = 0; 
	try {
	test = compare (arg,m_pData);
	if ((test<0)&&(branch1!=NULL))
		theNode = branch1->findNode (arg);
	else if ((test>0)&&(branch2!=NULL))
		theNode = branch2->findNode (arg);
	else
		theNode = this; }
	catch (...)	{
		if (m_pData==NULL)
			theNode=this;
		else
			m_pData=NULL; }
	return theNode;
}

//	Generic Routine looks for a location to add a Node and
//	then adds the node to the identified branch, creating a
//	new branch for the new Node as necessary.  When called
//	from addNode (bTreeType<X>*) find node gets called twice
//	but this should have no effect or loss of effiency, why?

template<class X>
bTreeType<X> *bTreeType<X>::addNode (X arg)
{
	int test;
	bTreeType *path;
	test = compare (arg,m_pData);
	try {
	if ((test<0)&&(branch1==NULL)) {
		branch1 = new (NULL) bTreeType (arg,false);
		branch1->root = this;
		path = branch1; }
	else if ((test>0)&&(branch2==NULL)) {
		branch2 = new (NULL) bTreeType (arg,false);
		branch2->root = this;
		path = branch2; }
	else if (test!=0) {
		path = findNode (arg);
		path->addNode (arg); }
	else 
		path = this; }
	catch (X) {
		if (m_pData==NULL) {
			m_pData = arg; }
		}
	return path;
}

//	Straightforward routine to add a bTreeType to a tree
//	in whatever natural loaction is	returned by findNode
//	This addNode is different from the other version, in
//	that that version must create the node from an object
//	and then add it.

template<class X>
bTreeType<X> *bTreeType<X>::addNode (bTreeType *arg)
{
	bTreeType *location = NULL;
	location = findNode (arg->m_pData);
	location->addNode (arg->m_pData);
	return location;
}

//	searches a binary tree for a Node, adds the Node if
//	it is not fould, otherwise increment the instance
//	count. <instance count no longer supported>

template<class X>
bTreeType<X> *bTreeType<X>::getNode (X arg)
{
	X aLeaf = NULL;
	bTreeType *theBranch = NULL;
	theBranch = findNode (arg);
	if (theBranch!=NULL) {
		aLeaf = theBranch->m_pData;
		if (aLeaf==NULL)
			theBranch->m_pData = arg;
		else if ((compare (arg,aLeaf)!=0)) {
			theBranch = theBranch->addNode (arg);}
	}
	return theBranch;
}

//	obtain a pointer indicating the path to take to get
//	to a particular object.

template<class X>
bTreeType<X> *bTreeType<X>::path (X arg)
{
	bTreeType *thePath = this;
	int test;
	if (arg!=NULL)
	do {
		m_pData = thePath->m_pData;
		if (m_pData==NULL)
			return NULL;
		test = compare (arg,m_pData);
		if (test<0)
			thePath = thePath->branch1;
		else if (test>0)
			thePath = thePath->branch2;
		if (thePath==NULL)
			break;
	}
	while (test!=0);
	else thePath=NULL;
	return thePath;
}

template <class X>
bTreeType<X> *bTreeType<X>::m_pNext (bTreeType *previous)
{
	bTreeType *result = NULL;
	if (branch2==NULL)
		traceRoot (result);
	else if (previous!=branch2)
		branch2->putNode1 (result);
	else
		traceRoot (result);
	if (result==this)		// OOPS!
		result = root;
	return result;
}

//	The rule is "always turn right" at every corner.
//	First obtain the root of this node and then follow
//	upward until a node is found where we passed on
//	through the first time, i.e. a node that has a
//	branch one that we already took!

template <class X>
void bTreeType<X>::traceRoot (bTreeType *(&found))
{
	bTreeType *location = this;
	found = root;
	if (found!=NULL)
	do {
		if (found->branch2!=location)
			break;
		location = found;
		found = location->root; }
	while (found!=NULL);
}

//	Non-recursive method to seek out the bottom left node
//	of a binary tree.

template <class X>
void bTreeType<X>::putNode1 (bTreeType *(&location))
{
	location = this;
	while (location->branch1!=0)
		location = location->branch1;
}

//	this is really screwed up right now, the idea is
//	to splice in another tree in the correct spot under
//	this one.  Useful for optimizing trees.

template<class X>
bTreeType<X> *bTreeType<X>::addTree (bTreeType *aTree)
{
	int depth = 0;
	int test;
	X arg = aTree->m_pData;
	bTreeType *location;
	test = ::compare (arg,m_pData);
	if (test==0)
		location = root;
	else if (test>0)
		location = branch1;
	else 
		location = branch2;	
	if (location!=NULL)
		location = location->findNode (arg);
	else {
		location = aTree;
		location->root = this;
	}
	return location;
}

//	This routine traces out a tree in lexical order, and
//	it puts in parenthesis to indicate recursion depth
//	This might be useful for reconstructing trees from
//	disk.

template<class X>
void bTreeType<X>::printTree (int depth)
{
	if (m_pData!=NULL)
		DEBUG_STR << "\"" << m_pData << "\", ";
	else
		DEBUG_STR << "<NULL>, ";
//	DEBUG_STR << timesSeen;

	DEBUG_STR << "(";
	if (branch1==NULL)
	DEBUG_STR << "NULL";
	else
		branch1->printTree (depth+1);
	DEBUG_STR << ", ";		
	if (branch2==NULL)
	DEBUG_STR << "NULL";
	else
		branch2->printTree (depth+1);
	DEBUG_STR << ")";
}

template<class X>
void bTreeType<X>::openTestFile ()
{		
	theFile.open ("bTreeTest.txt",ios::in|ios::out);
}

template<class X>
void bTreeType<X>::saveTree (int depth)
{
	if (branch1!=NULL)
		branch1->saveTree (depth+1);
//	for (int i=0;i<depth;i++)
//		theFile << "*";
	theFile << " " << m_pData << " [" << times;
	if (branch1!=NULL)
		theFile << "," << branch1->m_pData;
	if (branch2!=NULL)
		theFile << "," << branch2->m_pData;
	theFile << "]\n";
	if (branch2!=NULL)
		branch2->saveTree (depth+1);	
}

template<class X>
void bTreeType<X>::closeTestFile ()
{
	theFile.close ();
}

template<class X>
void bTreeType<X>::recursiveTest ()
{
	openTestFile ();
	saveTree (0);
	closeTestFile ();
}

template<class X>
int bTreeType<X>::reGraft (int count)
{
	bTreeType *child1, *child2, *parent;
	child1 = branch1;
	child2 = branch2;
	parent = root;

//	Following child1 until we bottom out.

	if (child1!=NULL)
		count = child1->reGraft (count);
	
//	Arriving at the current node.  Either child1
//	is null or we have already been there and done
//	that, i.e. we have printed out the list.

	count++;
//	DEBUG_STR << count << " " << m_pData << "\n";

//	The nice thing is that we never have to go
//	down that path again!  So we are free to
//	reassign it is necessary!

	if (child2!=NULL)
		count = child2->reGraft (count);
	return count;
}

//	Routine of general utility for finding the bottom
//	left branch of a tree.

template<class X>
int bTreeType<X>::countNodes ()
{
	int loopCount = 0;
	DEBUG_STR << "\nTraversal Test -- ";
	bTreeType *lastNode = this;
	bTreeType *next,*position;
	putNode1 (position);
	while (position!=NULL) {
		m_pData = position->m_pData;
		if (m_pData!=0)
		next = position->m_pNext (lastNode);
		lastNode = position;
		position = next;
		loopCount++;
	}
	return loopCount;
}

//	used to find the root node of a markov whose
//	penultimate/pred token is the same as theWord

template<class X>
bTreeType<X> *bTreeType<X>::findMarkovian (X theWord)
{
	bTreeWord *rootNode, *m_pNext = NULL;
	if (theWord!=NULL) {
		rootNode = m_root->findNode (theWord);
		m_pNext = rootNode->markov;
	}
	return m_pNext;
}



#include "stdafx.h"
#include "afxmt.h"
#include "new.h"
#include <memory>
#include "defines.h"
#include <fstream>

using namespace std;

#include "symbol_table.h"
#include "bTreeType.h"



//	simple string comparison called from
//	bTreeNode routines -- throws an exception
//	if either argument is NULL;

int compare (char *item1, char *item2)
{
	char buffer1 [4096], buffer2 [4096];
	int result;
	try {
		if ((item1==NULL)||(item2==NULL))
			throw ((char*)NULL);
		strcpy (buffer1,item1);
		strcpy (buffer2,item2);
		to_lower (buffer1);
		to_lower (buffer2);
		result = strcmp (buffer1,buffer2);
		return result;
		}
	catch (char* item) {
//		cout << "Null error in bTreeTypes comparison function.\n";
		throw item;
	}
}

void test_btree_allocate ()
{
	size_t sz;
	void *ptr0;
	sz = sizeof(bTreeType<char*>);
	ptr0 =  malloc(sz);
	bTreeType<char*> *str1;
	str1 = new (ptr0)bTreeType<char*>;

}








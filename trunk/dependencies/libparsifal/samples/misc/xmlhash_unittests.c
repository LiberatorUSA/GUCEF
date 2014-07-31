#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "xmlhash.h"

#include <assert.h>

#ifdef _MSC_VER
	#ifdef _DEBUG
		#include <crtdbg.h>
		#define _CRTDBG_MAP_ALLOC
	#endif
#define vsnprintf _vsnprintf
#endif

#define XMLHTable_InsertInt(t,key,i) \
    ((!XMLHTable_Insert((t),(key),NULL)) ? 0 : (*((int*)XMLHTable_GetData((t), (t)->bucket)) = (i)), 1)

extern FILE **GetLine_Fopen(const char *filename, const char *mode);
extern int GetLine_Fclose(FILE **f);
extern char *GetLine_Read(FILE **f);

char *fonly(char *f) { char *s = f + strlen(f); while(s-1!=f && *(s-1)!='/' && *(s-1)!='\\') s--; return s; }
#define UTEST(t,msg) (printf("%s:%d %s\n  %s ** %s\n", \
fonly(__FILE__), __LINE__, (t)?"OK":"FAILED", #t, (msg)?"MSG: "#msg:""))
#define UTEST_ASSERT(t) UTEST(t,NULL)
#define UTEST_ASSERT_MSG(t,msg) UTEST(t,msg)
#define UTEST_ASSERT_MSG_FMT(t,msg) UTEST(t,msg)

void die(const char *error)
{
	puts(error);
	exit(EXIT_FAILURE);
}

int EnumTable(char *key, void *data, void *table)
{	
	if (table==NULL) {
		UTEST_ASSERT_MSG(
			(!strcmp(key, "zell") ||
			!strcmp(key, "zulu") ||
			!strcmp(key, "zur") ||
			!strcmp(key, "halkoo") ||
			!strcmp(key, "halki") ||
			!strcmp(key, "nauta") ||
			!strcmp(key, "aalio")), "7 items left:");
			puts(key);
	}
	else if ((strcmp(key, "zell") &&
			strcmp(key, "zulu") &&
			strcmp(key, "zur") &&
			strcmp(key, "halkoo") &&
			strcmp(key, "halki") &&
			strcmp(key, "nauta") &&
			strcmp(key, "aalio"))) 
				XMLHTable_Remove((LPXMLHTABLE)table, key);
	return 0;
}

int unittests(void)
{
	char *s;
	FILE **f;
	LPXMLHTABLE ht;
	int i=0;
	int *ip;
	void *dup = ht;

	UTEST_ASSERT( f = GetLine_Fopen("finnish.txt", "r") );
	UTEST_ASSERT( ht = XMLHTable_CreateEx(ht, 255, int, sizeof(int)) );

	while((s = GetLine_Read(f)))
		if (!XMLHTable_InsertInt(ht, s, ++i)) die("out of memory");
	
	UTEST_ASSERT_MSG( i == 7776, "num of inserted rows" );

	GetLine_Fclose(f);
	
	UTEST_ASSERT( ip = XMLHTable_Lookup(ht, "keinua") );

	UTEST_ASSERT_MSG( *ip == 3400, "row should be 3400" );

	UTEST_ASSERT( XMLHTable_Remove(ht, "keinua") );
	
	UTEST_ASSERT( (ip = XMLHTable_Lookup(ht, "keinua")) == NULL );
	
	UTEST_ASSERT( ip = XMLHTable_Lookup(ht, "keinuu") );

	UTEST_ASSERT_MSG( *ip == 3401, "row should be 3401" );
	
	ht->userdata = ht;
	UTEST_ASSERT_MSG( XMLHTable_Enumerate(ht, EnumTable) == 0, "remove all but 7 items");
	ht->userdata = NULL;
	UTEST_ASSERT_MSG( XMLHTable_Enumerate(ht, EnumTable) == 0, "enumerated 7 items");
		
	UTEST_ASSERT_MSG( (ip = XMLHTable_Insert(ht, "zulu", dup)) == dup, "trying to insert duplicate key" );
	
	UTEST_ASSERT_MSG( (ip = XMLHTable_Insert(ht, "zulus", dup)) != dup, "trying to insert unique key" );

	XMLHTable_Destroy(ht, NULL, 1);
	
	UTEST_ASSERT( f = GetLine_Fopen("finnish.txt", "r") );
	UTEST_ASSERT( ht = XMLHTable_CreateEx(ht, 255, int, sizeof(int)) );
	
	i=0;
	while((s = GetLine_Read(f))) {
		i++;
		if (!(i==600 || i==601 || i==7000 || i ==50 || i == 70 || i==23))
			if (!XMLHTable_InsertInt(ht, s, i)) die("out of memory");
	}
	GetLine_Fclose(f);
	
	UTEST_ASSERT( !(ip = XMLHTable_Lookup(ht, "asuja"))  );
	UTEST_ASSERT( !(ip = XMLHTable_Lookup(ht, "asujan")) );
	UTEST_ASSERT( !(ip = XMLHTable_Lookup(ht, "teko")) );
	UTEST_ASSERT( !(ip = XMLHTable_Lookup(ht, "aattoa")) );
	UTEST_ASSERT( !(ip = XMLHTable_Lookup(ht, "afasia")) );
	UTEST_ASSERT( !(ip = XMLHTable_Lookup(ht, "aapoja")) );
	
	UTEST_ASSERT( XMLHTable_InsertInt(ht, "afasia", 600) );
	UTEST_ASSERT( XMLHTable_Remove(ht, "afasia") );
	UTEST_ASSERT( XMLHTable_InsertInt(ht, "teko", 700) );

	UTEST_ASSERT( ip = XMLHTable_Lookup(ht, "teko") );
	UTEST_ASSERT( *ip == 700 );
	UTEST_ASSERT( !(ip = XMLHTable_Lookup(ht, "afasia")) );
	
	XMLHTable_Destroy(ht, NULL, 1);

	return 0;
}


int main(int argc, char* argv[])
{
	#ifdef _MSC_VER
		#ifdef _DEBUG
			int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
			/* Turn on leak-checking bit for dumping leaks to output: */
			tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
			_CrtSetDbgFlag( tmpFlag );
		#endif
	#endif
	unittests();
	return 0;
}


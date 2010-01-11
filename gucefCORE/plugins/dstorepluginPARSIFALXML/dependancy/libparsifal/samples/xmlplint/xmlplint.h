#define VER_XMLPLINT "1.2.0"

#define PFOUT fout
#define ASSERT_MEM_ABORT(p) \
  if (!(p)) { fprintf(stderr, "Out of memory! Line: %d\n", __LINE__); return XML_ABORT; }

enum outputformat {
	XMLOUTPUTFORMAT_ROUNDRIP,
	XMLOUTPUTFORMAT_EVENTS,
	XMLOUTPUTFORMAT_CANONICAL,
	XMLOUTPUTFORMAT_SILENT
};

struct XMLNOTATION {
	char *name;
	char *publicID;
	char *systemID;
};

#define RET_SUCCESS 0
#define RET_NOTWF 1
#define RET_NOTVALID 2
#define RET_IOERR 3
#define RET_FATAL 4

/* stack macros (from xmldef.h) */
#define STACK_PUSH(stack,item) (XMLVector_Append((stack), (item)))
#define STACK_PEEK(stack) (XMLVector_Get((stack),(stack)->length-1))
#define STACK_REMOVE(stack) (XMLVector_Remove((stack), (stack)->length-1))
#define STACK_POP(stack,item) \
( ((stack)->length) ? (memcpy((item), STACK_PEEK((stack)), (stack)->itemSize), \
STACK_REMOVE((stack)), (item)) : NULL)


#ifndef _RM_DEFINES_
#define _RM_DEFINES_

#include <iostream>
#include <new>
#include <malloc.h>
#include <assert.h>

#ifdef _DEBUG
#define RM_DEBUGGING true
#else
#define RM_DEBUGGING false
#endif

#ifndef RM_ASSERT
#define RM_ASSERT(s) assert(s)
#endif //RM_ASSERT

#ifndef RM_MALLOC
/*
	s: Size in bytes
*/
#define RM_MALLOC(s) malloc(s)
#endif //RM_MALLOC

#ifndef RM_FREE
#define RM_FREE(pointer) free(pointer)
#endif //RM_FREE

#ifndef RM_DELETE
#define RM_DELETE(pointer) delete pointer
#endif //RM_DELETE

#ifndef RM_PLACEMENT_NEW
/*
	s: size
	t: type
*/
#define RM_PLACEMENT_NEW(s, t) new (RM_MALLOC(s)) t
#endif //RM_PLACEMENT_NEW

#ifndef RM_PLACEMENT_FREE
/*
	First runs the pointers destructor and then frees the memory
	p: pointer
	t: type
*/
#define RM_PLACEMENT_FREE(p, t) p->~t; free(p)
#endif //RM_PLACEMENT_FREE

/*
	s: Debug message - to be printed
	l: Severity level
		0 -> print the message
		1 -> print the message and abort
*/
#define RM_DEBUG_MESSAGE(s, l) { \
	switch(l) { \
	case 0: \
		std::clog << "WARNING: " << s << std::endl; \
		break; \
	case 1:\
		std::clog << "ERROR: " << s << std::endl; \
		throw(s);\
		break;\
	default: break;\
}}

#endif //_RM_DEFINES_
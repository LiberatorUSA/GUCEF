/*	Public domain	*/

#ifndef	_AGAR_CORE_VASPRINTF_H_
#define	_AGAR_CORE_VASPRINTF_H_
#include <stdarg.h>
#include <agar/core/begin.h>
/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC int AG_TryVasprintf(char **, const char *, va_list);

static __inline__ void
AG_Vasprintf(char **msg, const char *fmt, va_list args)
{
	if (AG_TryVasprintf(msg, fmt, args) == -1) 
		AG_FatalError(NULL);
}
#if defined(_AGAR_INTERNAL) || defined(_USE_AGAR_STD)
# define Vasprintf AG_Vasprintf
# define TryVasprintf AG_TryVasprintf
#endif 
__END_DECLS
/* Close generated block */
#include <agar/core/close.h>
#endif /* _AGAR_CORE_VASPRINTF_H_ */

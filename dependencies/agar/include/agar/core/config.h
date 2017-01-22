/*	Public domain	*/

#ifndef _AGAR_CORE_CONFIG_H_
#define _AGAR_CORE_CONFIG_H_
#include <agar/core/begin.h>

typedef struct ag_object AG_Config;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_Config *agConfig;
extern DECLSPEC AG_ObjectClass agConfigClass;
extern DECLSPEC AG_Config *AG_ConfigObject(void);
extern DECLSPEC int AG_ConfigInit(AG_Config *, Uint);
extern DECLSPEC int AG_ConfigFile(const char *, const char *, const char *, char *, size_t) BOUNDED_ATTRIBUTE(__string__, 4, 5);
extern DECLSPEC int AG_CreateDataDir(void);
extern DECLSPEC int AG_ConfigSave(void);
extern DECLSPEC int AG_ConfigLoad(void);
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif /* _AGAR_CORE_CONFIG_H_ */

/*	Public domain	*/

#ifndef _AGAR_CORE_DB_H_
#define _AGAR_CORE_DB_H_
#include <agar/core/begin.h>

struct ag_db;

/* Database data item (e.g., key or value) */
typedef struct ag_dbt {
	void *data;
	size_t size;
} AG_Dbt;

typedef int (*AG_DbIterateFn)(const AG_Dbt *key, const AG_Dbt *val, void *arg);

typedef struct ag_db_class {
	struct ag_object_class _inherit;
	const char *name;			/* Database method name */
	const char *descr;			/* Short description */
	enum ag_db_key_mode {
		AG_DB_KEY_DATA,		/* Variable-length data */
		AG_DB_KEY_NUMBER,	/* Logical record number */
		AG_DB_KEY_STRING	/* Legal C string */
	} keyMode;
	enum ag_db_record_mode {
		AG_DB_REC_VARIABLE,	/* Variable-length records */
		AG_DB_REC_FIXED		/* Fixed-length records */
	} recMode;
	int  (*open)(void *, const char *, Uint);
	void (*close)(void *);
	int  (*sync)(void *);
	int  (*exists)(void *, const AG_Dbt *);
	int  (*get)(void *, const AG_Dbt *, AG_Dbt *);
	int  (*put)(void *, const AG_Dbt *, const AG_Dbt *);
	int  (*del)(void *, const AG_Dbt *);
	int  (*iterate)(void *, AG_DbIterateFn, void *);
} AG_DbClass;

#define AGDB_CLASS(db) ((AG_DbClass *)AGOBJECT(db)->cls)

typedef struct ag_db {
	struct ag_object _inherit;
	Uint flags;
#define AG_DB_OPEN	0x01		/* Database is open */
#define AG_DB_READONLY	0x02		/* Open in read-only mode */
} AG_Db;

#define AGDB(p) ((AG_Db *)(p))

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_DbClass agDbClass;
extern DECLSPEC AG_DbClass agDbHashClass;
extern DECLSPEC AG_DbClass agDbBtreeClass;
extern DECLSPEC AG_DbClass agDbMySQLClass;
extern DECLSPEC AG_Db *AG_DbNew(const char *);
extern DECLSPEC int AG_DbOpen(AG_Db *, const char *, Uint);
extern DECLSPEC void AG_DbClose(AG_Db *);
extern DECLSPEC int AG_DbSync(AG_Db *);

static __inline__ int
AG_DbExists(AG_Db *db, AG_Dbt *key)
{
	AG_DbClass *dbc = AGDB_CLASS(db);
	int rv;

	AG_ObjectLock(db);
	rv = dbc->exists(db, key);
	AG_ObjectUnlock(db);
	return (rv);
}

static __inline__ int
AG_DbGet(AG_Db *db, const AG_Dbt *key, AG_Dbt *val)
{
	AG_DbClass *dbc = AGDB_CLASS(db);
	int rv;

	AG_ObjectLock(db);
	rv = dbc->get(db, key, val);
	AG_ObjectUnlock(db);
	return (rv);
}

static __inline__ int
AG_DbPut(AG_Db *db, const AG_Dbt *key, const AG_Dbt *val)
{
	AG_DbClass *dbc = AGDB_CLASS(db);
	int rv;

	AG_ObjectLock(db);
	rv = dbc->put(db, key, val);
	AG_ObjectUnlock(db);
	return (rv);
}

static __inline__ int
AG_DbDel(AG_Db *db, const AG_Dbt *key)
{
	AG_DbClass *dbc = AGDB_CLASS(db);
	int rv;

	AG_ObjectLock(db);
	rv = dbc->del(db, key);
	AG_ObjectUnlock(db);
	return (rv);
}

static __inline__ int
AG_DbIterate(AG_Db *db, AG_DbIterateFn fn, void *arg)
{
	AG_DbClass *dbc = AGDB_CLASS(db);
	int rv;

	AG_ObjectLock(db);
	rv = dbc->iterate(db, fn, arg);
	AG_ObjectUnlock(db);
	return (rv);
}
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif /* _AGAR_CORE_DB_H_ */

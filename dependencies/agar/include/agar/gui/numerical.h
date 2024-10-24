/*	Public domain	*/

#ifndef _AGAR_WIDGET_NUMERICAL_H_
#define _AGAR_WIDGET_NUMERICAL_H_

#include <agar/gui/widget.h>
#include <agar/gui/textbox.h>
#include <agar/gui/button.h>
#include <agar/gui/ucombo.h>
#include <agar/gui/units.h>

#include <agar/gui/begin.h>
	
typedef struct ag_numerical {
	struct ag_widget wid;
	Uint flags;
#define AG_NUMERICAL_HFILL	0x01
#define AG_NUMERICAL_VFILL	0x02
#define AG_NUMERICAL_INT	0x04	/* Default binding should be int */
#define AG_NUMERICAL_EXCL	0x08	/* Exclusive access to bindings */

	char format[32];		/* Print format (for reals) */
	const AG_Unit *unit;		/* Conversion unit in use */
	int writeable;			/* 0 = read-only */
	char inTxt[64];			/* Input text buffer */
	AG_Textbox *input;		/* Input textbox */
	AG_UCombo *units;		/* Unit selector */
	AG_Button *incbu;		/* Increment button */
	AG_Button *decbu;		/* Decrement button */
	int wUnitSel, hUnitSel;		/* Size hints for entry box */
	int wPreUnit;			/* Size hint for unit selector */
	AG_Timer updateTo;		/* Timer for non-EXCL mode */
} AG_Numerical;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agNumericalClass;
extern DECLSPEC AG_Numerical *AG_NumericalNew(void *, Uint, const char *, const char *, ...) FORMAT_ATTRIBUTE(printf,4,5);
extern DECLSPEC AG_Numerical *AG_NumericalNewS(void *, Uint, const char *, const char *);
extern DECLSPEC AG_Numerical *AG_NumericalNewFlt(void *, Uint, const char *, const char *, float *);
extern DECLSPEC AG_Numerical *AG_NumericalNewFltR(void *, Uint, const char *, const char *, float *, float, float);
extern DECLSPEC AG_Numerical *AG_NumericalNewDbl(void *, Uint, const char *, const char *, double *);
extern DECLSPEC AG_Numerical *AG_NumericalNewDblR(void *, Uint, const char *, const char *, double *, double, double);
#ifdef AG_HAVE_LONG_DOUBLE
extern DECLSPEC AG_Numerical *AG_NumericalNewLdbl(void *, Uint, const char *, const char *, long double *);
extern DECLSPEC AG_Numerical *AG_NumericalNewLdblR(void *, Uint, const char *, const char *, long double *, long double, long double);
#endif
extern DECLSPEC AG_Numerical *AG_NumericalNewInt(void *, Uint, const char *, const char *, int *);
extern DECLSPEC AG_Numerical *AG_NumericalNewIntR(void *, Uint, const char *, const char *, int *, int, int);
extern DECLSPEC AG_Numerical *AG_NumericalNewUint(void *, Uint, const char *, const char *, Uint *);
extern DECLSPEC AG_Numerical *AG_NumericalNewUintR(void *, Uint, const char *, const char *, Uint *, Uint, Uint);
extern DECLSPEC void AG_NumericalSizeHint(AG_Numerical *, const char *);
extern DECLSPEC void AG_NumericalIncrement(AG_Numerical *);
extern DECLSPEC void AG_NumericalDecrement(AG_Numerical *);
extern DECLSPEC void AG_NumericalUpdate(AG_Numerical *);
extern DECLSPEC void AG_NumericalSelectUnit(AG_Numerical *, const char *);
extern DECLSPEC int AG_NumericalSetUnitSystem(AG_Numerical *, const char *);
extern DECLSPEC void AG_NumericalSetPrecision(AG_Numerical *, const char *, int);
extern DECLSPEC void AG_NumericalSetWriteable(AG_Numerical *, int);
extern DECLSPEC float AG_NumericalGetFlt(AG_Numerical *);
extern DECLSPEC double AG_NumericalGetDbl(AG_Numerical *);
#ifdef AG_HAVE_LONG_DOUBLE
extern DECLSPEC long double AG_NumericalGetLdbl(AG_Numerical *);
#endif
extern DECLSPEC int AG_NumericalGetInt(AG_Numerical *);
#define AG_NumericalGetUint(n) ((Uint)AG_NumericalGetInt(n))
#define AG_NumericalGetUint8(n) ((Uint8)AG_NumericalGetUint32(n))
#define AG_NumericalGetUint16(n) ((Uint16)AG_NumericalGetUint32(n))
#define AG_NumericalGetSint8(n) ((Sint8)AG_NumericalGetUint32(n))
#define AG_NumericalGetSint16(n) ((Sint16)AG_NumericalGetUint32(n))
extern DECLSPEC Uint32 AG_NumericalGetUint32(AG_Numerical *);
#define AG_NumericalGetSint32(n) ((Sint32)AG_NumericalGetUint32(n))
#ifdef AG_HAVE_64BIT
extern DECLSPEC Uint64 AG_NumericalGetUint64(AG_Numerical *);
#define AG_NumericalGetSint64(n) ((Sint64)AG_NumericalGetUint64(n))
#endif
#ifdef AG_LEGACY
extern DECLSPEC AG_Numerical *AG_NumericalNewUint8(void *, Uint, const char *, const char *, Uint8 *) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewUint8R(void *, Uint, const char *, const char *, Uint8 *, Uint8, Uint8) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewSint8(void *, Uint, const char *, const char *, Sint8 *) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewSint8R(void *, Uint, const char *, const char *, Sint8 *, Sint8, Sint8) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewUint16(void *, Uint, const char *, const char *, Uint16 *) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewUint16R(void *, Uint, const char *, const char *, Uint16 *, Uint16, Uint16) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewSint16(void *, Uint, const char *, const char *, Sint16 *) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewSint16R(void *, Uint, const char *, const char *, Sint16 *, Sint16, Sint16) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewUint32(void *, Uint, const char *, const char *, Uint32 *) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewUint32R(void *, Uint, const char *, const char *, Uint32 *, Uint32, Uint32) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewSint32(void *, Uint, const char *, const char *, Sint32 *) DEPRECATED_ATTRIBUTE;
extern DECLSPEC AG_Numerical *AG_NumericalNewSint32R(void *, Uint, const char *, const char *, Sint32 *, Sint32, Sint32) DEPRECATED_ATTRIBUTE;
extern DECLSPEC void AG_NumericalSetIncrement(AG_Numerical *, double) DEPRECATED_ATTRIBUTE;
extern DECLSPEC void AG_NumericalSetRangeDbl(AG_Numerical *, double, double) DEPRECATED_ATTRIBUTE;
extern DECLSPEC void AG_NumericalSetRangeInt(AG_Numerical *, int, int) DEPRECATED_ATTRIBUTE;
extern DECLSPEC void AG_NumericalSetRangeFlt(AG_Numerical *, float, float) DEPRECATED_ATTRIBUTE;
#define AG_NumericalSetRange AG_NumericalSetRangeDbl
extern DECLSPEC void AG_NumericalSetMin(AG_Numerical *, double) DEPRECATED_ATTRIBUTE;
#define AG_NumericalSetMinDbl(num,v) AG_NumericalSetMin((num),(v))
#define AG_NumericalSetMinFlt(num,v) AG_NumericalSetMin((num),(float)(v))
#define AG_NumericalSetMinInt(num,v) AG_NumericalSetMin((num),(int)(v))
extern DECLSPEC void AG_NumericalSetMax(AG_Numerical *, double) DEPRECATED_ATTRIBUTE;
#define AG_NumericalSetMaxDbl(num,v) AG_NumericalSetMax((num),(v))
#define AG_NumericalSetMaxFlt(num,v) AG_NumericalSetMax((num),(float)(v))
#define AG_NumericalSetMaxInt(num,v) AG_NumericalSetMax((num),(int)(v))
#endif
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_NUMERICAL_H_ */

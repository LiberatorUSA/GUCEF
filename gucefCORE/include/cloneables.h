/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */
 
#ifndef GUCEF_CORE_CLONEABLES_H
#define GUCEF_CORE_CLONEABLES_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"      /* gucefCORE basic types */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"        /* templated implementation of a cloneable object */
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#include "CTLinkedCloneableObj.h"        /* templated implementation of a linked cloneable object */
#define GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CTCloneableObj< CDynamicBuffer > TCloneableBuffer;
typedef CTCloneableObj< CString >        TCloneableString;
typedef CTCloneableObj< bool >           TCloneableBool;
typedef CTCloneableObj< double >         TCloneableDouble;
typedef CTCloneableObj< float >          TCloneableFloat;
typedef CTCloneableObj< UInt32 >         TCloneableUInt32;
typedef CTCloneableObj< Int32 >          TCloneableInt32;
typedef CTCloneableObj< UInt16 >         TCloneableUInt16;
typedef CTCloneableObj< Int16 >          TCloneableInt16;
typedef CTCloneableObj< UInt8 >          TCloneableUInt8;
typedef CTCloneableObj< Int8 >           TCloneableInt8;
typedef TCloneableInt8                   TCloneableChar;
typedef TCloneableInt8                   TCloneableByte;
typedef TCloneableUInt8                  TCloneableUByte;

typedef CTLinkedCloneableObj< CDynamicBuffer > TLinkedCloneableBuffer;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CLONEABLES_H ? */

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

#ifndef GUCEF_CORE_CSTRINGSTORAGE_H
#define GUCEF_CORE_CSTRINGSTORAGE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_CORE_CMUTEX_H
#endif /* GUCEF_CORE_CMUTEX_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Internally used class for string memory management 
 */
class CStringStorage
{
        public:
        
        static CStringStorage* Instance( void );                
        
        private:
        friend class CString;

        UInt32 RegisterString( const char* str   , 
                               UInt32 length     ,
                               const char** strp );

        UInt32 RegisterStringCombo( const char* string1 ,
                                    UInt32 length1      ,
                                    const char* string2 ,
                                    UInt32 length2      ,
                                    const char** strp   );
                               
        void RegisterStringCopy( UInt32 stringid );

        void UnregisterString( UInt32 stringid );
        
        private:
        friend class CGUCEFCOREModule;
        
        static void Deinstance( void );

        private:        
        static MT::CMutex _mutex;
        static CStringStorage* _instance;

        CStringStorage( void );
        CStringStorage( const CStringStorage& src );
        CStringStorage& operator=( const CStringStorage& src );
        
        CDynamicArray _strings;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CEVENTPUMP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-05-2005 :
        - Fixed a bugf in RegisterStringCombo(): The allocated string storage
          was 1 byte to small causing memory corruption.
- 26-03-2005 :
        - Added string caching as a means to temp keep a string 'alive' even 
          though no CString object is curently refrencing the string. This 
          mechanism can be used when copying an C++ object is not an option.
- 13-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/

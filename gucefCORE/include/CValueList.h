/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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
 
#ifndef GUCEF_CORE_CVALUELIST_H
#define GUCEF_CORE_CVALUELIST_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTRINGMAP_H
#include "CStringMap.h"          /* string container */
#define GUCEF_CORE_CSTRINGMAP_H
#endif /* GUCEF_CORE_CSTRINGMAP_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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

// @deprecated replace with std::map< CString, CString >
class GUCEFCORE_EXPORT_CPP CValueList
{
        public:
        
        CValueList( void );
        
        CValueList( const CValueList& src );
        
        CValueList& operator=( const CValueList& src );        
        
        virtual ~CValueList();
               
        void Set( const CString& keyandvalue );
        
        void Set( const CString& key   ,
                  const CString& value );
        
        CString GetValue( const CString& key ) const;                
        
        CString GetValue( const UInt32 index ) const;
        
        CString GetPair( const CString& key ) const;
        
        CString GetPair( const UInt32 index ) const;
        
        CString GetKey( const UInt32 index ) const;                
        
        bool Exists( const CString& key ) const;
        
        void Delete( const CString& key );
        
        void DeleteAll( void );
        
        UInt32 GetCount( void ) const;
        
        private:                
                
        CStringMap m_list;     
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CVALUELIST_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-09-2005 :
        - initial version

--------------------------------------------------------------------------*/
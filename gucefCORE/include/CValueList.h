/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
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

class GUCEFCORE_EXPORT_CPP CValueList
{
        public:
        
        CValueList( void );
        
        CValueList( const CValueList& src );
        
        CValueList& operator=( const CValueList& src );        
        
        virtual ~CValueList();
               
        CString operator[]( const CString& key ) const;
        
        void SetMultiple( const CString& keyandvalue ,
                          const char seperator       );
        
        void Set( const CString& keyandvalue );
        
        void Set( const CString& key   ,
                  const CString& value );
        
        CString GetValue( const CString& key ) const;                
        
        CString GetValue( const UInt32 index ) const;
        
        CString GetPair( const CString& key ) const;
        
        CString GetPair( const UInt32 index ) const;
        
        CString GetKey( const UInt32 index ) const;                
        
        bool HasKey( const CString& key ) const;
        
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
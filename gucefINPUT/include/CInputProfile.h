/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef CINPUTPROFILE_H
#define CINPUTPROFILE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CSTRINGMAP_H
#include "CStringMap.h"
#define CSTRINGMAP_H
#endif /* CSTRINGMAP_H ? */

#ifndef GUCEFINPUT_MACROS_H
#include "gucefINPUT_macros.h"
#define GUCEFINPUT_MACROS_H
#endif /* GUCEFINPUT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Class that defines an input mapping profile.
 *      It maps a number of named action maps to a manipulator.
 *      A manipulator is that which takes action based on input.
 */
class EXPORT_CPP CInputProfile
{
        public:

        CInputProfile( const CORE::CString& profilename );
        
        CInputProfile( const CInputProfile& src );        
        
        ~CInputProfile();
        
        CInputProfile& operator=( const CInputProfile& src );
        
        bool SetActionMapLink( const CORE::CString& actionmapname   ,
                               const CORE::CString& manipulatorname );
                               
        CORE::CStringList GetActionMapList( void ) const;
        
        const CORE::CString& GetName( void ) const;
        
        CORE::CString GetManipulatorActionMap( const CORE::CString& manipulatorname ) const;
        
        const CORE::CStringList* GetActionMapManipulators( const CORE::CString& actionmapname ) const;
        
        const CORE::CStringList& GetManipulatorList( void ) const; 
        
        void Clear( void );
        
        UInt32 GetActionMapCount( void ) const;
        
        UInt32 GetManipulatorCount( void ) const;
        
        bool UsesActionMap( const CORE::CString& actionmapname ) const;
        
        bool LoadConfig( const CORE::CDataNode& node );
        
        bool SaveConfig( CORE::CDataNode& node );
        
        private:
        
        CInputProfile( void );
        
        private:
        
        CORE::CString m_name;
        CORE::CStringMap m_map;
        CORE::CStringList m_manipulators;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CINPUTPROFILE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
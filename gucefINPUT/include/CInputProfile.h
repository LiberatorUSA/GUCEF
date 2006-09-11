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
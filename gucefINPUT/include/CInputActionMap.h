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

#ifndef CINPUTACTIONMAP_H
#define CINPUTACTIONMAP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDVSTRING_H
#include "CDVString.h"
#define CDVSTRING_H
#endif /* CDVSTRING_H ? */

#ifndef CSTRINGLIST_H
#include "CStringList.h"
#define CSTRINGLIST_H
#endif /* CSTRINGLIST_H ? */

#ifndef GUCEFINPUT_MACROS_H
#include "gucefINPUT_macros.h"
#define GUCEFINPUT_MACROS_H
#endif /* GUCEFINPUT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF{ namespace CORE { class CDataNode; } }

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
 *
 */
class EXPORT_CPP CInputActionMap
{
        public:
        
        CInputActionMap( const CORE::CString& name       = ""  , 
                         const UInt32 mousemapcount      = 16  ,
                         const UInt32 keyboardmapcount   = 128 ,
                         const UInt32 deviceboolmapcount = 16  ,
                         const UInt32 devicevarmapcount  = 16  ,
                         const UInt32 devicecount        = 4   );

        CInputActionMap( const CInputActionMap& src );
        
        ~CInputActionMap();
        
        CInputActionMap& operator=( const CInputActionMap& src );
        
        void SetMouseButtonDownMap( const UInt32 buttonindex        ,
                                    const UInt32 actionindex        ,
                                    const CORE::CString& actionName );
                                    
        UInt32 GetMouseButtonDownMap( const UInt32 buttonindex );
                                    
        void SetMouseButtonUpMap( const UInt32 buttonindex        ,
                                  const UInt32 actionindex        ,
                                  const CORE::CString& actionName );
                                    
        UInt32 GetMouseButtonUpMap( const UInt32 buttonindex );

        void SetKeyboardKeyDownMap( const UInt32 buttonindex        ,
                                    const UInt32 actionindex        ,
                                    const CORE::CString& actionName );
                                    
        UInt32 GetKeyboardKeyDownMap( const UInt32 buttonindex );
        
        void SetKeyboardKeyUpMap( const UInt32 keyindex           ,
                                  const UInt32 actionindex        ,
                                  const CORE::CString& actionName );
                                    
        UInt32 GetKeyboardKeyUpMap( const UInt32 keyindex );
                
        void SetDeviceBooleanOnMap( const UInt32 deviceindex ,
                                    const UInt32 stateindex  ,
                                    const UInt32 actionindex );
                                    
        UInt32 GetDeviceBooleanOnMap( const UInt32 deviceindex ,
                                      const UInt32 stateindex  );
                                      
        void SetDeviceBooleanOffMap( const UInt32 deviceindex ,
                                     const UInt32 stateindex  ,
                                     const UInt32 actionindex );
                                    
        UInt32 GetDeviceBooleanOffMap( const UInt32 deviceindex ,
                                       const UInt32 stateindex  );
                                       
        void SetDeviceVariableChangedMap( const UInt32 deviceid    ,
                                          const UInt32 stateindex  ,
                                          const UInt32 actionindex );
                                          
        UInt32 GetDeviceVariableChangedMap( const UInt32 deviceid    ,
                                            const UInt32 stateindex  );

        UInt32 GetMouseButtonMaxMapCount( void ) const;
        
        UInt32 GetKeyboardKeyMaxMapCount( void ) const;

        UInt32 GetDeviceMaxMapCount( void ) const;
        
        UInt32 GetDeviceMaxBoolMapCount( void ) const;
        
        UInt32 GetDeviceMaxVarMapCount( void ) const;
        
        CORE::CStringList GetMouseButtonDownActionNames( void ) const;

        CORE::CStringList GetMouseButtonUpActionNames( void ) const;
        
        CORE::CStringList GetKeyboardKeyUpActionNames( void ) const;
        
        CORE::CStringList GetKeyboardKeyDownActionNames( void ) const;
                                            
        bool SaveConfig( CORE::CDataNode& tree );
                                                               
        bool LoadConfig( const CORE::CDataNode& treeroot );
        
        void Clear( void );
        
        void Reset( void );
        
        const CORE::CString& GetName( void ) const;
        
        private:
        
        bool EnsureDeviceMaxCapacity( const UInt32 devicemax );
        void EnsureDeviceBoolStateMaxCapacity( const UInt32 deviceindex, const UInt32 statemax );
        void EnsureDeviceVarStateMaxCapacity( const UInt32 deviceindex, const UInt32 statemax );
        
        private:
        
        UInt32* m_mousebupmap;
        UInt32* m_mousebdownmap;
        UInt32* m_keyboardkupmap;
        UInt32* m_keyboardkdownmap;
        UInt32** m_deviceboolonmap; 
        UInt32** m_devicebooloffmap;
        UInt32** m_devicevarchangemap;

        CORE::CStringList m_mouseBUpActionNames;
        CORE::CStringList m_mouseBDownActionNames;
        CORE::CStringList m_keyboardKUpActionNames;
        CORE::CStringList m_keyboardKDownActionNames;
        
        UInt32 m_mousemapcount;
        UInt32 m_keyboardmapcount;
        UInt32 m_deviceboolmapcount;
        UInt32 m_devicevarmapcount;
        UInt32 m_devicecount;
        
        CORE::CString m_name;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CINPUTACTIONMAP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-10-2005 :
        - Added utils for string aliases for the actions
- 16-09-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
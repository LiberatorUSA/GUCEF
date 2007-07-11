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

#ifndef GUCEF_INPUT_CINPUTPROFILEMANAGER_H
#define GUCEF_INPUT_CINPUTPROFILEMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CSTRINGMAP_H
#include "CStringMap.h"
#define CSTRINGMAP_H
#endif /* CSTRINGMAP_H ? */

#ifndef CICONFIGURABLE_H
#include "CIConfigurable.h"
#define CICONFIGURABLE_H
#endif /* CICONFIGURABLE_H ? */

#ifndef GUCEFINPUT_MACROS_H
#include "gucefINPUT_macros.h"
#define GUCEFINPUT_MACROS_H
#endif /* GUCEFINPUT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { namespace CORE { class CDataNode; } }

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

class CInputProfile;
class CIInputHandler;
class CGUCEFINPUTModule;
class CInputActionMap;

/*-------------------------------------------------------------------------*/

/**
 *      Provides a means to create an input mapping routing for your application.
 *      Input handling is done per profile if you use this manager's handlers.
 *      Each profile has a routing table for action maps to manipulators.
 *      Action maps can be reused across profiles giving you a verry flexible
 *      input mapping mechanism.
 */
class EXPORT_CPP CInputProfileManager : public CORE::CIConfigurable
{
        public:                

        static CInputProfileManager* Instance( void );
        
        /**
         *      Attempts to add the profile if no profile with such 
         *      a name already exists
         */
        bool AddProfile( const CInputProfile& profile );
        
        /**
         *      Sets the given profile.
         *      If the profle exists it will be overwritten, 
         *      if not it will be added
         */
        void SetProfile( const CInputProfile& profile );
        
        /**
         *      Attempts to retieve a profile for the name given.
         *      If no profile with the given name can be found NULL is returned.
         *
         *      Note that for profile alterations you should create a local copy, alter it
         *      and then set it using SetProfile()
         */
        const CInputProfile* GetProfile( const CORE::CString& profilename );
        
        /**
         *      Returns a list of the names of all profiles available
         */
        CORE::CStringList GetProfileList( void ) const;        
        
        /**
         *      Attempts to retrieve the handler for the entire profile.
         *      If no handler can be found for the given profile name then 
         *      NULL will be returned.
         *
         *      Note that this the member function you would typicly use
         *      to hook an input profile to an input context by setting
         *      the pointer returned here as the handler for the your
         *      input context.
         */
        CIInputHandler* GetProfileHandler( const CORE::CString& profilename );               
        
        /**
         *      Attempts to add the action map if no action map with such 
         *      a name already exists
         */
        bool AddActionMap( const CInputActionMap& actionmap );
        
        /**
         *      Sets the given action map.
         *      If an action map with the same name exists it will be overwritten, 
         *      if not it will be added
         */
        void SetActionMap( const CInputActionMap& actionmap );
        
        /**
         *      Attempts to retieve an action map for the name given.
         *      If no action map with the given name can be found NULL is returned.
         */
        CInputActionMap* GetActionMap( const CORE::CString& actionmapname );
        
        /**
         *      returns a list with the names of all currently available
         *      action maps.
         */
        CORE::CStringList GetActionMapList( void ) const;
        
        /**
         *      Returns a list of the names of all manipulators available
         *
         *      Note that manipulators who have not registered here will not be 
         *      available and as such will not appear in the list. So for creating 
         *      your mapping config you might want to use GetRefrencedManipulatorsList()
         *      instead because it does not require the manipulators to already be 
         *      registered.
         */
        CORE::CStringList GetManipulatorList( void ) const;
        
        /**
         *      Returns the names of all manipulators that are mentioned in profiles
         *      Usefull for configuration if not all the manipulators are available yet.
         */
        CORE::CStringList GetRefrencedManipulatorsList( void ) const;
        
        bool LoadConfig( const CORE::CDataNode& node );
        
        bool SaveConfig( CORE::CDataNode& node );

        private:
        friend class CGUCEFINPUTModule;
        
        static void Deinstance( void );
        
        private:
        friend class CInputBasedManipulator;
        
        void RegisterManipulator( CInputBasedManipulator* manipulator );
        
        void UnregisterManipulator( CInputBasedManipulator* manipulator );
        
        private:
        
        CInputProfileManager( void );
        CInputProfileManager( const CInputProfileManager& src );
        virtual ~CInputProfileManager();
        CInputProfileManager& operator=( const CInputProfileManager& src );
        
        void UpdateProfileMapping( const UInt32 index );
        void UpdateAllMappings( void );
        
        private:
        
        CORE::CStringMap m_profiles;
        CORE::CStringMap m_actionmaps;
        CORE::CStringMap m_manipulators;
               
        private:
        
        static CInputProfileManager* m_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CINPUTPROFILEMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/

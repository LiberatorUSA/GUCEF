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

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string>
#include <map>
#include "gucefMT_CMutex.h"

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#include "gucefCORE_macros.h"
#include "ExceptionMacros.h"

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
 *  The CNotificationIDRegistry is used to obtain unique event id's for 
 *  events name strings. 
 *
 *  Note that it is strongly recommended to include the various namespaces 
 *  in the event strings. For example:
 *      "GUCEF::CORE::CNotificationIDRegistry::FooBarEvent"
 *  If you follow this rule you should never have to worry about naming
 *  conflicts, while keeping the ease of use that comes from integer id's
 */
class GUCEFCORE_EXPORT_CPP CNotificationIDRegistry
{
    public:
           
    /**
     *  Standard singleton Instance()
     *  creates the singleton object if it does not exist yet
     *
     *  @return pointer to the singleton object
     */
    static CNotificationIDRegistry* Instance( void );
    
    /**
     *  Registers string identifiers and returns a unique event ID which can be used
     *  in it's place.
     *
     *  @param keyvalue the key string you wish to register and obtain a unique notification event ID for.
     *  @param okIfAlreadyRegistered indicates whether a register action using an already registered key should be handled silently
     *  @throw EKeyAlreadyRegistered thrown when the given key is already registered and okIfAlreadyRegistered is false or an empty key string is given
     *  @return unique notification id for the given key identifier
     */
    CEvent Register( const std::string& keyvalue              ,
                     const bool okIfAlreadyRegistered = false );
    
    /**
     *  Unregisters the string identifier. The ascociated event id should not longer be used.
     *  
     *  @param keyvalue the key string identifier you wish to unregister
     *  @param okIfUnknownKeyGiven indicates whether an unregister action using an unknown key string identifier should be handled silently
     *  @throw EUnknownKey thrown when the given key string is unknown and okIfUnknownKeyGiven is false or an empty key string is given
     */
    void Unregister( const std::string& keyvalue            ,
                     const bool okIfUnknownKeyGiven = false );
    
    /**
     *  Performs a lookup operation for the given key and if possible returns the
     *  associated unique notification event ID.
     *
     *  @param keyvalue the key you wish to use for the lookup operation.
     *  @param registerUnknown Whether or not to register the key if it does not yet exist.
     *  @throw EUnknownKey thrown when the requested item is not found and registerUnknown == false.
     */
    CEvent Lookup( const std::string& keyvalue        ,
                   const bool registerUnknown = false );    
    
    /**
     *  Performs a lookup operation for the given notification id and attempts to
     *  locate a matching identifier string. Verry usefull for translation into human readable
     *  output (when debugging).
     *
     *  Note that this operation is secondary and is not optimized for speed
     *  in any way.
     *
     *  @param eventID the notification ID you wish to resolve into an identifier
     *  @throw EUnknownEventID thrown when the given event id is unknown
     *  @return the notification event identifier for the given id.
     */
    std::string Lookup( const CEvent& eventID ) const;
    
    bool IsRegistered( const std::string& keyvalue ) const;    

    GUCEF_DEFINE_MSGEXCEPTION( GUCEFCORE_EXPORT_CPP, EKeyAlreadyRegistered );
    GUCEF_DEFINE_MSGEXCEPTION( GUCEFCORE_EXPORT_CPP, EEmptyKeyString );
    GUCEF_DEFINE_MSGEXCEPTION( GUCEFCORE_EXPORT_CPP, EUnknownKey );
    GUCEF_DEFINE_MSGEXCEPTION( GUCEFCORE_EXPORT_CPP, EInvalidKey );
    GUCEF_DEFINE_MSGEXCEPTION( GUCEFCORE_EXPORT_CPP, EInvalidEventID );
    
    private:
    friend class CGUCEFCOREModule;
        
    static void Deinstance( void );
    
    private:   
    
    CNotificationIDRegistry( void );
    
    CNotificationIDRegistry( const CNotificationIDRegistry& src );
    
    virtual ~CNotificationIDRegistry();
    
    CNotificationIDRegistry& operator=( const CNotificationIDRegistry& src );
    
    private:
    
    static CNotificationIDRegistry* m_instance;
    static MT::CMutex m_dataLock;
    
    typedef std::map<const std::string,UInt32> TRegistryList;
    TRegistryList m_list;
    UInt32 m_lastid;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

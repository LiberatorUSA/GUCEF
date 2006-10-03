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
 
#ifndef GUCEF_CORE_CEXCLUSIVEACTIVEOBJ_H
#define GUCEF_CORE_CEXCLUSIVEACTIVEOBJ_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string>
#include "gucefCORE_macros.h"
#include "gucefCORE_ETypes.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEXCLUSIVEACTIVEOBJ_CPP
    #pragma warning( push )
#endif

#pragma warning( disable: 4251 ) // 'classname' needs to have dll-interface to be used by clients of class 'classname'
#pragma warning( disable: 4284 ) // return type for operator -> is 'const *' (ie; not a UDT or reference to a UDT).
#pragma warning( disable: 4786 ) // identifier was truncated to 'number' characters

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

class CExclusiveActivationManager;

/*-------------------------------------------------------------------------*/

/**
 *  Abstract base class for any object that should be active exclusively 
 *  for it's type ( ie familiy ). Calling Activate() on an object of type 'x'
 *  will cause all other registerd objects of type 'x' to be deactivated.
 */
class GUCEFCORE_EXPORT_CPP CExclusiveActiveObj
{
    public:
    
    /**
     *  Constructs and registers the object at the manager as an mutual 
     *  exclusive activation family member for family 'objTypeName'
     *
     *  @param objTypeName the type name of the mutual exclusive activation family
     */
    explicit CExclusiveActiveObj( const std::string& objTypeName );        
    
    /**
     *  Destructs the object and unregisters the object at the manager
     */
    virtual ~CExclusiveActiveObj();
    
    /**
     *  Attempts to set the object of it's type as the exclusively activated
     *  instance.
     *
     *  Note that if the object's activation sequence failed (ie OnActivate() returned false )
     *  then the previous active object of the given type will remain the activated instance.
     *
     *  @return whether the object was successfuly activated.
     */
    bool Activate( void );
    
    /**
     *  Attempts to deactivate the object, this will cause a switch to the default
     *  object for the exclusive activation family. If no default has been set then
     *  no instance will be active anymore.
     *
     *  Note that this action will fail if the instance is not the active instance within
     *  it's family
     *
     *  @return whether the object was successfuly deactivated.
     */
    bool Deactivate( void );

    /**
     *  Attempts to set the instance as the default activation instance.
     *
     *  Note: only fails if the instance is not registered with the exclusive activation manager.
     *
     *  @return whether setting the instance as the default instance for the family was successful.
     */
    bool SetAsDefault( void );

    /**
     *  Returns the active state of the object
     *
     *  @return the current active state of the object  
     */
    bool IsActive( void ) const;

    /**
     *  Returns whether the object is the default instance for it's family
     *
     *  @return whether the object is the default instance for it's family
     */
    bool IsTheDefault( void ) const;
    
    /**
     *  Returns whether the instance is currently registered as a 
     *  mutual exclusive activation object.
     *
     *  Note that if an instance is not registered calls made to
     *  Activate() will always fail !!!
     */
    bool IsRegistered( void ) const;
    
    /**
     *  Returns the objects name.
     *
     *  Note that object names should be unique within at minimum the scope of it's
     *  type family. It will be used for lookup operations when using the manager.
     *
     *  @return the name of the derived class instance
     */
    virtual std::string GetObjName( void ) const = 0;
    
    /**
     *  Returns the name of the mutual exclusive activation family of which
     *  the object instance is a member.
     *
     *  @return the type name of the mutual exclusive activation family  
     */
    const std::string& GetObjTypeName( void ) const;
    
    protected:
    
    void RegisterAsExclusiveActiveObj( void );
    
    void UnregisterAsExclusiveActiveObj( void );

    protected:
    friend class CExclusiveActivationManager;

    /**
     *  Handler for the activation sequence
     *  If successful (ie true is returned) this object will become
     *  the exclusive activated object within it's family.
     *
     *  @return whether the activation sequence was successful.
     */
    virtual bool OnActivate( void ) = 0;

    /**
     *  Handler for post-activation-sequence operations
     *  Allows you to perform operations based on whether a switch
     *  from one exclusive family member to another was successful.
     *
     *  Note that this event callback is not manditory, it's default
     *  implementation is a no-operation.
     *
     *  @param whether the activation sequence was successful.
     */
    virtual void OnPostActivate( const bool activationSuccessful );
    
    /**
     *  Handler for the deactivation sequence
     *  If successful (ie true is returned) this object will yield
     *  it's activated state to another object of the same type family
     *
     *  @return whether the deactivation sequence was successful.
     */
    virtual bool OnDeactivate( void ) = 0;

    /**
     *  Handler for post-deactivation-sequence operations
     *  Allows you to perform operations based on whether 
     *  a deactivation state change has occured.
     *
     *  Note that this event callback is not manditory, it's default
     *  implementation is a no-operation.
     *
     *  @param whether the deactivation sequence was successful.
     */
    virtual void OnPostDeactivate( const bool deactivationSuccessful );

    private:
    friend class CExclusiveActivationManager;

    void SetActiveState( const bool active );
    
    private:
    
    CExclusiveActiveObj( void );                                      /**< canot be used: a typename is manditory */
    CExclusiveActiveObj( const CExclusiveActiveObj& src );            /**< an exclusive object suggests it is unique, so why would you want to copy it ? */
    CExclusiveActiveObj& operator=( const CExclusiveActiveObj& src ); /**< an exclusive object suggests it is unique, so why would you want to copy it ? */
    
    private:
    
    std::string m_objTypeName;
    bool m_active;    
    bool m_registered;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEXCLUSIVEACTIVEOBJ_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CEXCLUSIVEACTIVEOBJ_H  ? */

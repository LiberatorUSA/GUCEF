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

#ifndef GUCEF_CORE_CEXCLUSIVEACTIVEOBJ_H
#define GUCEF_CORE_CEXCLUSIVEACTIVEOBJ_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string>

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

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
class GUCEF_CORE_PUBLIC_CPP CExclusiveActiveObj
{
    public:

    /**
     *  Constructs and registers the object at the manager as an mutual
     *  exclusive activation family member for family 'objTypeName'
     *
     *  @param objTypeName the type name of the mutual exclusive activation family
     */
    explicit CExclusiveActiveObj( const CString& objTypeName );

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
    virtual CString GetObjName( void ) const = 0;

    /**
     *  Returns the name of the mutual exclusive activation family of which
     *  the object instance is a member.
     *
     *  @return the type name of the mutual exclusive activation family
     */
    const CString& GetObjTypeName( void ) const;

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

    CString m_objTypeName;
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

#endif /* GUCEF_CORE_CEXCLUSIVEACTIVEOBJ_H  ? */

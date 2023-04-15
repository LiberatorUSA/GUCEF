/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CIOBSERVER_H
#define GUCEF_CORE_CIOBSERVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CILOCKABLE_H
#include "gucefMT_CILockable.h"
#define GUCEF_MT_CILOCKABLE_H
#endif /* GUCEF_MT_CILOCKABLE_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CITYPENAMED_H
#include "CITypeNamed.h"
#define GUCEF_CORE_CITYPENAMED_H
#endif /* GUCEF_CORE_CITYPENAMED_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
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

class CNotifierImplementor;
class CNotifier;
class CPumpedObserver;
class CEvent;

/*-------------------------------------------------------------------------*/

/**
 *  Interface for observers.
 *  Observer based classes should implement OnNotify()
 */
class GUCEF_CORE_PUBLIC_CPP CIObserver : public virtual MT::CILockable ,
                                         public virtual CITypeNamed
{
    public:

    CIObserver( void );

    CIObserver( const CIObserver& src );

    virtual ~CIObserver();

    CIObserver& operator=( const CIObserver& src );

    protected:
    friend class CNotifierImplementor;
    friend class CPumpedObserver;
    friend class CObserverScopeLock;
    friend class CObserverScopeReadOnlyLock;

    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventId the unique event id for an event
     *  @param evenData optional notifier defined user data
     */
    virtual void OnNotify( CNotifier* notifier                ,
                           const CEvent& eventId              ,
                           CICloneable* evenData = GUCEF_NULL ) = 0;

    /**
     *  Can be implemented by the derived classes such that all notification system interactions
     *  are protected against multiple threads accessing them
     *  Typical implementation would be to have this call Lock() on a mutex member in a derived class
     *
     *  Default inline implementation simply calls the object global Lock() but you may wish to use a 
     *  distinct lock to guard the notification mechanism since it can operate independently
     */
    virtual MT::TLockStatus NotificationLock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const 
        { return Lock( lockWaitTimeoutInMs ); }

    /**
     *  Counterpart to the NotificationLock() member function. This releases the lock obtained using NotificationLock() 
     *  Typical implementation would be to have this call Unlock() on a mutex member in a derived class
     */
    virtual MT::TLockStatus NotificationUnlock( void ) const 
        { return Unlock(); }

    /**
     *  Can be implemented by the derived classes such that all notification system interactions 
     *  are protected against multiple threads accessing them
     *  Typical implementation would be to have this call Lock() as a reader on a reader-writer lock in a derived class
     *
     *  Default inline implementation simply calls the object global ReadOnlyLock() but you may wish to use a 
     *  distinct lock to guard the notification mechanism since it can operate independently
     */
    virtual MT::TLockStatus NotificationReadOnlyLock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const 
        { return ReadOnlyLock( lockWaitTimeoutInMs ); }

    /**
     *  Counterpart to the NotificationReadOnlyLock() member function. This releases the lock obtained using NotificationReadOnlyLock() 
     *  Typical implementation would be to have this call Unlock() as a reader on a reader-writer lock in a derived class
     */
    virtual MT::TLockStatus NotificationReadOnlyUnlock( void ) const 
        { return ReadOnlyUnlock(); };

};

/*-------------------------------------------------------------------------*/

/**
 *  Leverages the CIObserver interface to NotificationLock() and NotificationUnlock() the scope of an
 *  notification operation while still having the same stack unroll etc advantages of
 *  the CScopeMutex class
 *  Typical usage would be to pass in the 'this' pointer to lock the member function
 *  scope for a object who's class is derived from CIObserver
 */
class GUCEF_CORE_PUBLIC_CPP CObserverScopeLock
{
    public:

    /**
     *  Locks if a lockable object is provided
     *  Otherwise ( lockableNotifier == GUCEF_NULL ) this object will act as a no-op
     *  This allows you to use the same code in a thread-safe or non-threadsafe manner depending on context
     *  Only taking the locking penalty when needed based on said context
     */
    CObserverScopeLock( const CIObserver* lockableObserver );

    /**
     *  Locks the lockable object and retaints the lock for the lifespan of this object by default
     *  You can unlock early via the EarlyUnlock() member function
     */
    CObserverScopeLock( const CIObserver& lockableObserver );

    ~CObserverScopeLock();

    /**
     *  Returns whether the current lock state based on the Lock() and Unlock()
     *  operations on the lockable object
     */
    bool IsLocked( void ) const;

    /**
     *  Allows you to unlock before the scope lock triggers destruction of the CObjectScopeLock object
     *  Useful for more complex code flows where in most code paths you want to retain the lock except for
     *  a small subset of code path(s)
     */
    bool EarlyUnlock( void );

    private:
    const CIObserver* m_lockableObserver;
    bool m_isLocked;

    CObserverScopeLock( const CObserverScopeLock& src );              /* Copying doesnt make sense */
    CObserverScopeLock& operator=( const CObserverScopeLock& src );   /* Copying doesnt make sense */
};

/*-------------------------------------------------------------------------*/

/**
 *  Leverages the CIObserver interface to NotificationReadOnlyLock() and NotificationReadOnlyUnlock() the scope of an
 *  notification operation while still having the same stack unroll etc advantages of
 *  the CScopeMutex class
 *  Typical usage would be to pass in the 'this' pointer to lock the member function
 *  scope for a object who's class is derived from CIObserver
 */
class GUCEF_CORE_PUBLIC_CPP CObserverScopeReadOnlyLock
{
    public:

    /**
     *  Locks if a lockable object is provided
     *  Otherwise ( lockableNotifier == GUCEF_NULL ) this object will act as a no-op
     *  This allows you to use the same code in a thread-safe or non-threadsafe manner depending on context
     *  Only taking the locking penalty when needed based on said context
     */
    CObserverScopeReadOnlyLock( const CIObserver* lockableNotifier );

    /**
     *  Locks the lockable object and retaints the lock for the lifespan of this object by default
     *  You can unlock early via the EarlyUnlock() member function
     */
    CObserverScopeReadOnlyLock( const CIObserver& lockableNotifier );

    ~CObserverScopeReadOnlyLock();

    /**
     *  Returns whether the current lock state based on the Lock() and Unlock()
     *  operations on the lockable object
     */
    bool IsLocked( void ) const;

    /**
     *  Allows you to unlock before the scope lock triggers destruction of the CObjectScopeLock object
     *  Useful for more complex code flows where in most code paths you want to retain the lock except for
     *  a small subset of code path(s)
     */
    bool EarlyReaderUnlock( void );

    private:
    const CIObserver* m_lockableObserver;
    bool m_isLocked;

    CObserverScopeReadOnlyLock( const CObserverScopeReadOnlyLock& src );              /* Copying doesnt make sense */
    CObserverScopeReadOnlyLock& operator=( const CObserverScopeReadOnlyLock& src );   /* Copying doesnt make sense */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIOBSERVER_H ? */

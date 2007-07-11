/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef CVFSURLHANDLER_H
#define CVFSURLHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CURLHANDLER_H
#include "CURLHandler.h"             /* gucefCORE url handler base class */
#define GUCEF_CORE_CURLHANDLER_H
#endif /* GUCEF_CORE_CURLHANDLER_H ? */

#ifndef GUCEF_VFS_MACROS_H
#include "gucefVFS_macros.h"         /* often used gucefVFS macros */
#define GUCEF_VFS_MACROS_H
#endif /* GUCEF_VFS_MACROS_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CVFS;

/*-------------------------------------------------------------------------*/

/**
 *      VFS URL handling implementation.
 *      for example: vfs://foo/bar.dat
 *  
 *      For internal use, no need to export this class
 */
class CVFSURLHandler : public CORE::CURLHandler
{
        public:
        
        /**
         *      Doesnt do anything special atm.
         */        
        CVFSURLHandler( void );
        
        /**
         *      Doesnt do anything special atm.
         */        
        CVFSURLHandler( const CVFSURLHandler& src );
        
        /**
         *      Doesnt do anything special atm.
         */
        virtual ~CVFSURLHandler();
        
        /**
         *      Doesnt do anything special atm.
         */        
        CVFSURLHandler& operator=( const CVFSURLHandler& src );

        virtual bool Activate( CORE::CURL& url );
        
        virtual void Deactivate( CORE::CURL& url );
                         
        virtual bool IsActive( const CORE::CURL& url ) const;        

        virtual CORE::CICloneable* Clone( void ) const;
        
        static void Register( void );
        
        static void Unregister( void );        
        
        private:
        
        CVFS* m_vfs;        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CVFSURLHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-04-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDSTORECODECPLUGIN_H
#include "CDStoreCodecPlugin.h"     /* plugin for CDStoreCodec */
#define GUCEF_CORE_CDSTORECODECPLUGIN_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGIN_H ? */

#include "CDStoreCodecPluginRef.h"  /* definition of the class implemented here */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDStoreCodecPluginRef::CDStoreCodecPluginRef( const CDStoreCodecPluginRef& src )
        : _plugin( src._plugin )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDStoreCodecPluginRef::CDStoreCodecPluginRef( void )
        : _plugin( NULL )
{
        GUCEF_BEGIN;
        GUCEF_END;       
}

/*-------------------------------------------------------------------------*/

CDStoreCodecPluginRef::~CDStoreCodecPluginRef()
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

bool 
CDStoreCodecPluginRef::IsValid( void ) const
{
        GUCEF_BEGIN;        
        GUCEF_END_RET( bool, _plugin->IsValid() );
}

/*-------------------------------------------------------------------------*/        

bool 
CDStoreCodecPluginRef::StoreDataTree( const CDataNode* tree   ,
                                      const CString& filename )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( bool, _plugin->StoreDataTree( tree     ,
                                                     filename ) );
}

/*-------------------------------------------------------------------------*/        

bool 
CDStoreCodecPluginRef::StoreDataTree( const CDataNode* tree   ,
                                      CIOAccess* file         )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( bool, _plugin->StoreDataTree( tree ,
                                                     file ) );
}

/*-------------------------------------------------------------------------*/                                      
                                                                   
bool 
CDStoreCodecPluginRef::BuildDataTree( CDataNode* treeroot     ,
                                      const CString& filename )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( bool, _plugin->BuildDataTree( treeroot ,
                                                     filename ) );
}

/*-------------------------------------------------------------------------*/

bool 
CDStoreCodecPluginRef::BuildDataTree( CDataNode* treeroot ,
                                      CIOAccess* file     )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( bool, _plugin->BuildDataTree( treeroot ,
                                                     file     ) );
}                                      

/*-------------------------------------------------------------------------*/                                      
        
CString 
CDStoreCodecPluginRef::GetName( void ) const
{
        GUCEF_BEGIN;        
        GUCEF_END_RET( CString, _plugin->GetName() );
}

/*-------------------------------------------------------------------------*/
        
CString 
CDStoreCodecPluginRef::GetTypeName( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END_RET( CString, _plugin->GetTypeName() );
}

/*-------------------------------------------------------------------------*/
        
CString 
CDStoreCodecPluginRef::GetCopyright( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END_RET( CString, _plugin->GetCopyright() );
}

/*-------------------------------------------------------------------------*/
        
const TVersion* 
CDStoreCodecPluginRef::GetVersion( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END_RET( const TVersion*, _plugin->GetVersion() );
}

/*-------------------------------------------------------------------------*/  
        
CICloneable* 
CDStoreCodecPluginRef::Clone( void ) const
{GUCEF_TRACE;
        return new CDStoreCodecPluginRef( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

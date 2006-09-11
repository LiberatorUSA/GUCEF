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
{TRACE;
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

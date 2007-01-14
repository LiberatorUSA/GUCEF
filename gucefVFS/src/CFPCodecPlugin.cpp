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

#include "dvoswrap.h"

#ifndef GUCEF_CORE_CCPPWRAPFILEACCESS_H
#include "CCPPWrapFileAccess.h"
#define GUCEF_CORE_CCPPWRAPFILEACCESS_H
#endif /* GUCEF_CORE_CCPPWRAPFILEACCESS_H ? */

#include "CFPCodecPlugin.h"

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum
{
        VFSPLUG_INIT            = 0    ,
        VFSPLUG_SHUTDOWN               ,
        VFSPLUG_PACK_OPEN              ,
        VFSPLUG_PACK_CLOSE             ,
        VFSPLUG_LOAD                   ,
        VFSPLUG_LOAD_WRITEABLE         ,
        VFSPLUG_UNLOAD                 ,
        VFSPLUG_FILELIST               ,
        VFSPLUG_FREE_FILELIST          ,
        VFSPLUG_NAME                   ,
        VFSPLUG_COPYRIGHT              ,
        VFSPLUG_VERSION                ,
        
        VFSPLUG_LASTFPTR
};

/*-------------------------------------------------------------------------*/

typedef void ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Init )                 ( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef void ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Shutdown )             ( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef void ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Pack_Open )            ( void** plugdata, CORE::TIOAccess* packfile, void** packdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef void ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Pack_Close )           ( void** plugdata, CORE::TIOAccess* packfile, void** packdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef CORE::TIOAccess* ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Load )           ( void** plugdata, CORE::TIOAccess* packfile, void** packdata, const char* filename, UInt32* errorcode ) PLUGIN_CALLSPEC_SUFFIX;
typedef CORE::TIOAccess* ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Load_Writeable ) ( void** plugdata, CORE::TIOAccess* packfile, void** packdata, const char* filename, UInt32* errorcode ) PLUGIN_CALLSPEC_SUFFIX;
typedef void   ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Unload )             ( void** plugdata, CORE::TIOAccess* packfile, void** packdata, CORE::TIOAccess* file, const char* filename, UInt32* errorcode ) PLUGIN_CALLSPEC_SUFFIX;
typedef char** ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Filelist )           ( void** plugdata, CORE::TIOAccess* packfile, void** packdata, UInt32* errorcode ) PLUGIN_CALLSPEC_SUFFIX;
typedef void ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Free_Filelist )        ( void** plugdata, CORE::TIOAccess* packfile, void** packdata, char** filelist, UInt32* errorcode ) PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Name )          ( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Copyright )     ( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef const CORE::TVersion* ( PLUGIN_CALLSPEC_PREFIX *TVFSPLUGFPTR_Version )   ( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CFPCodecPlugin::CFPCodecPlugin( void )
{TRACE;
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CFPCodecPlugin::CFPCodecPlugin( const CFPCodecPlugin& src )
{TRACE;
        /* dummy, do not use */        
} 

/*-------------------------------------------------------------------------*/

CFPCodecPlugin::CFPCodecPlugin( const CORE::CString& codecfile )
        : _id( 0 )
{TRACE;
        _sohandle = CORE::LoadModuleDynamicly( codecfile.C_String() );
        if ( !_sohandle ) return;
        
        _fptable[ VFSPLUG_INIT ] = CORE::GetFunctionAddress( _sohandle      ,
                                                             "VFSPLUG_Init" ,
                                                             1*sizeof(void*)      );                                                       
        _fptable[ VFSPLUG_SHUTDOWN ] = CORE::GetFunctionAddress( _sohandle          ,
                                                                 "VFSPLUG_Shutdown" ,
                                                                 1*sizeof(void*)          );
        _fptable[ VFSPLUG_PACK_OPEN ] = CORE::GetFunctionAddress( _sohandle           ,
                                                                  "VFSPLUG_Pack_Open" ,
                                                                  3*sizeof(void*)           );                                                       
        _fptable[ VFSPLUG_PACK_CLOSE ] = CORE::GetFunctionAddress( _sohandle           ,
                                                                   "VFSPLUG_Pack_Close" ,
                                                                   3*sizeof(void*)            );                                                                                                                            
        _fptable[ VFSPLUG_LOAD ] = CORE::GetFunctionAddress( _sohandle      ,
                                                             "VFSPLUG_Load" ,
                                                             5*sizeof(void*)      );
        _fptable[ VFSPLUG_LOAD_WRITEABLE ] = CORE::GetFunctionAddress( _sohandle                ,
                                                                       "VFSPLUG_Load_Writeable" ,
                                                                       5*sizeof(void*)                );
        _fptable[ VFSPLUG_UNLOAD ] = CORE::GetFunctionAddress( _sohandle        ,
                                                               "VFSPLUG_Unload" ,
                                                               6*sizeof(void*)        );                                                         
        _fptable[ VFSPLUG_FILELIST ] = CORE::GetFunctionAddress( _sohandle          ,
                                                                 "VFSPLUG_Filelist" ,
                                                                 4*sizeof(void*)          );                                                                 
        _fptable[ VFSPLUG_FREE_FILELIST ] = CORE::GetFunctionAddress( _sohandle               ,
                                                                      "VFSPLUG_Free_Filelist" ,
                                                                      5*sizeof(void*)               );                                                            
        _fptable[ VFSPLUG_NAME ] = CORE::GetFunctionAddress( _sohandle      ,
                                                             "VFSPLUG_Name" ,
                                                             sizeof(void*)        );
        _fptable[ VFSPLUG_COPYRIGHT ] = CORE::GetFunctionAddress( _sohandle                ,
                                                                  "VFSPLUG_Copyright_EULA" ,
                                                                  sizeof(void*)                  );
        _fptable[ VFSPLUG_VERSION ] = CORE::GetFunctionAddress( _sohandle         ,
                                                                "VFSPLUG_Version" ,
                                                                sizeof(void*)           );
                                                          
        if ( ( !_fptable[ VFSPLUG_INIT ] ) ||
             ( !_fptable[ VFSPLUG_SHUTDOWN ] ) ||
             ( !_fptable[ VFSPLUG_PACK_OPEN ] ) ||
             ( !_fptable[ VFSPLUG_PACK_CLOSE ] ) ||
             ( !_fptable[ VFSPLUG_LOAD ] ) ||
             ( !_fptable[ VFSPLUG_LOAD_WRITEABLE ] ) ||
             ( !_fptable[ VFSPLUG_UNLOAD ] ) ||
             ( !_fptable[ VFSPLUG_FILELIST ] ) ||
             ( !_fptable[ VFSPLUG_FREE_FILELIST ] ) ||
             ( !_fptable[ VFSPLUG_NAME ] ) ||
             ( !_fptable[ VFSPLUG_COPYRIGHT ] ) ||
             ( !_fptable[ VFSPLUG_VERSION ] ) )
        {
                CORE::UnloadModuleDynamicly( _sohandle );
                memset( _fptable, NULL, sizeof(void*) * VFSPLUG_LASTFPTR );
                _sohandle = NULL;
                
                DEBUGOUTPUT( "Invalid codec module" );
                return;        
        }
        _path = codecfile;
        
        /*
         *      Intialize the plugin module
         */
        ( (TVFSPLUGFPTR_Init) _fptable[ VFSPLUG_INIT ] )( &_plugdata );
        
        CORE::tsprintf( "FPCodec plugin initialized\n" );
        CORE::tsprintf( "  - Name: %s\n", GetName().C_String() );
        CORE::tsprintf( "  - Copyright/EULA: %s\n", GetCopyrightEULA().C_String() );
}

/*-------------------------------------------------------------------------*/

CFPCodecPlugin::~CFPCodecPlugin()
{TRACE;
        if ( _sohandle )
        {
                /*
                 *      Let the plugin know we are shutting it down
                 */
                ( (TVFSPLUGFPTR_Shutdown) _fptable[ VFSPLUG_SHUTDOWN ] )( &_plugdata ); 
                 
                /*
                 *      Unload the module
                 */ 
                CORE::UnloadModuleDynamicly( _sohandle );
        }                
}

/*-------------------------------------------------------------------------*/

void 
CFPCodecPlugin::OpenPack( CORE::CIOAccess* packfile ,
                          void** packdata           )
{TRACE;
        return ( (TVFSPLUGFPTR_Pack_Open) _fptable[ VFSPLUG_PACK_OPEN ] )( &_plugdata               ,
                                                                           packfile->CStyleAccess() ,
                                                                           packdata                 );
}

/*-------------------------------------------------------------------------*/
        
void 
CFPCodecPlugin::ClosePack( CORE::CIOAccess* packfile ,
                           void** packdata           )
{TRACE;
        return ( (TVFSPLUGFPTR_Pack_Close) _fptable[ VFSPLUG_PACK_CLOSE ] )( &_plugdata               ,
                                                                             packfile->CStyleAccess() ,
                                                                             packdata                 );
}

/*-------------------------------------------------------------------------*/

bool 
CFPCodecPlugin::IsValid( void ) const
{TRACE;
        return _sohandle != NULL;
}

/*-------------------------------------------------------------------------*/

CORE::CString 
CFPCodecPlugin::GetName( void )
{TRACE;
        return ( (TVFSPLUGFPTR_Name) _fptable[ VFSPLUG_NAME ] )( &_plugdata );
}

/*-------------------------------------------------------------------------*/

CORE::CString 
CFPCodecPlugin::GetCopyrightEULA( void )
{TRACE;
        return ( (TVFSPLUGFPTR_Copyright) _fptable[ VFSPLUG_COPYRIGHT ] )( &_plugdata );
}

/*-------------------------------------------------------------------------*/

const CORE::TVersion* 
CFPCodecPlugin::GetVersion( void )
{TRACE;
        return ( (TVFSPLUGFPTR_Version) _fptable[ VFSPLUG_VERSION ] )( &_plugdata );
}

/*-------------------------------------------------------------------------*/

CORE::CStringList
CFPCodecPlugin::GetFilelist( CORE::CIOAccess* packfile ,
                             void** packdata           )
{TRACE;
        UInt32 errorcode;
        CORE::CStringList list;
        char** clist = ( (TVFSPLUGFPTR_Filelist) _fptable[ VFSPLUG_FILELIST ] )( &_plugdata               ,
                                                                                 packfile->CStyleAccess() ,
                                                                                 packdata                 ,
                                                                                 &errorcode               );
        while ( clist )
        {
                list.Append( *clist );
                ++clist;       
        }
                
        ( (TVFSPLUGFPTR_Free_Filelist) _fptable[ VFSPLUG_FREE_FILELIST ] )( &_plugdata               ,
                                                                            packfile->CStyleAccess() ,
                                                                            packdata                 ,
                                                                            clist                    ,
                                                                            &errorcode               );
        return list;        
}
                
/*-------------------------------------------------------------------------*/                
                
CORE::CIOAccess* 
CFPCodecPlugin::GetFile( CORE::CIOAccess* packfile ,
                         const CORE::CString& file ,
                         void** packdata           ,
                         UInt32& errorcode         )
{TRACE;        
        CORE::TIOAccess* access = ( (TVFSPLUGFPTR_Load) _fptable[ VFSPLUG_LOAD ] )( &_plugdata               ,
                                                                                    packfile->CStyleAccess() ,
                                                                                    packdata                 ,                                                                              
                                                                                    file.C_String()          ,
                                                                                    &errorcode               );
                                                                              
        if ( access )
        {
                return new CORE::CCPPWrapFileAccess( access );                
        }                                                                              
        return NULL;
}                   
        
/*-------------------------------------------------------------------------*/        
        
CORE::CIOAccess* 
CFPCodecPlugin::GetWriteableFile( CORE::CIOAccess* packfile ,
                                  const CORE::CString& file ,
                                  void** packdata           ,
                                  UInt32& errorcode         )
{TRACE;
        CORE::TIOAccess* access = ( (TVFSPLUGFPTR_Load_Writeable) _fptable[ VFSPLUG_LOAD_WRITEABLE ] )( &_plugdata               ,
                                                                                                        packfile->CStyleAccess() ,
                                                                                                        packdata                 ,
                                                                                                        file.C_String()          ,
                                                                                                        &errorcode               );
                                                                              
        if ( access )
        {
                return new CORE::CCPPWrapFileAccess( access );                
        }                                                                              
        return NULL;
}

/*-------------------------------------------------------------------------*/

void 
CFPCodecPlugin::Unload( CORE::CIOAccess* file         ,
                        const CORE::CString& filename ,
                        CORE::CIOAccess* packfile     ,
                        void** packdata               ,
                        UInt32& errorcode             )
{TRACE;
        ( (TVFSPLUGFPTR_Unload) _fptable[ VFSPLUG_UNLOAD ] )( &_plugdata               ,
                                                              packfile->CStyleAccess() ,
                                                              packdata                 ,   
                                                              file->CStyleAccess()     ,
                                                              filename.C_String()      ,
                                                              &errorcode               );
        delete file;                                                                     
}

/*-------------------------------------------------------------------------*/

void 
CFPCodecPlugin::SetCodecID( UInt32 id )
{TRACE;
        _id = id;
}

/*-------------------------------------------------------------------------*/

UInt32
CFPCodecPlugin::GetCodecID( void ) const
{TRACE;
        return _id;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CFPCodecPlugin::GetLocation( void ) const
{TRACE;
        return _path;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

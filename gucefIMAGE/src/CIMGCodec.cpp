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

#include "CIMGCodec.h"        /* definition of this class */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum
{
        IMGPLUG_CHECK_LOADABLE_EXT = 0 ,
        IMGPLUG_CHECK_LOADABLE_FILE    ,
        IMGPLUG_CHECK_LOADABLE_FORMAT  ,
        IMGPLUG_CHECK_SAVEABLE_EXT     ,
        IMGPLUG_CHECK_SAVEABLE_FILE    ,
        IMGPLUG_CHECK_SAVEABLE_FORMAT  ,
        IMGPLUG_GET_LOADABLE_EXT_LIST  ,
        IMGPLUG_GET_SAVEABLE_EXT_LIST  , 
        IMGPLUG_LOAD                   ,
        IMGPLUG_SAVE                   ,
        IMGPLUG_UNLOAD                 ,
        IMGPLUG_NAME                   ,
        IMGPLUG_COPYRIGHT              ,
        IMGPLUG_VERSION 
};

/*-------------------------------------------------------------------------*/

typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_Ext_Loadable ) ( const char *filename, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_Loadable )     ( CORE::TIOAccess *access, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_FLoadable )    ( Int32 format, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_Ext_Saveable ) ( const char *filename, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_Saveable )     ( const TImageData* imgdata, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_FSaveable )    ( Int32 format, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Load )               ( UInt32 hidx, CORE::TIOAccess *access, TImageData *imgdata, void **plugdata ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Save )               ( UInt32 hidx, const char *filename, UInt32 format, UInt32 compression, const TImageData *imgdata ) GUCEF_CALLSPEC_SUFFIX;
typedef void   ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Unload )             ( UInt32 hidx, TImageData *imgdata, void *plugdata ) GUCEF_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Name )          ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const char** ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Loadable_Ext_List ) ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const char** ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Saveable_Ext_List ) ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Copyright )  ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const TVersion* ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Version )  ( void ) GUCEF_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CIMGCodec::CIMGCodec( const CORE::CString& filename )
        : _sohandle( NULL ) ,
          _loadcount( 0 )
{
        DEBUGOUTPUT( "CIMGCodec::CIMGCodec( const CORE::CString& filename )" );
        
        try 
        {
                _sohandle = CORE::LoadModuleDynamicly( filename.C_String() );        
                if ( _sohandle == NULL ) return;        
        }
        catch ( ... )
        {
                _sohandle = NULL;
                return;        
        }                
        
        _fptable[ IMGPLUG_CHECK_LOADABLE_EXT ] = CORE::GetFunctionAddress( _sohandle                             ,
                                                                           "IMGPLUG_Check_If_Loadable_Using_Ext" ,
                                                                           2*PTRSIZE                             );
        _fptable[ IMGPLUG_CHECK_LOADABLE_FILE ] = CORE::GetFunctionAddress( _sohandle                   ,
                                                                            "IMGPLUG_Check_If_Loadable" ,
                                                                            2*PTRSIZE                   );
        _fptable[ IMGPLUG_CHECK_LOADABLE_FORMAT ] = CORE::GetFunctionAddress( _sohandle                          ,
                                                                             "IMGPLUG_Check_If_Format_Loadable" ,
                                                                             PTRSIZE+4                          );
        _fptable[ IMGPLUG_CHECK_SAVEABLE_FORMAT ] = GetFunctionAddress( _sohandle                          ,
                                                                        "IMGPLUG_Check_If_Format_Saveable" ,
                                                                        PTRSIZE+4                          );
        _fptable[ IMGPLUG_LOAD ] = CORE::GetFunctionAddress( _sohandle                 ,
                                                             "IMGPLUG_Load_Image_Data" ,
                                                             4+PTRSIZE*3               );
        _fptable[ IMGPLUG_SAVE ] = CORE::GetFunctionAddress( _sohandle                 ,
                                                             "IMGPLUG_Save_Image_Data" ,
                                                             2*PTRSIZE+12              );
        _fptable[ IMGPLUG_UNLOAD ] = CORE::GetFunctionAddress( _sohandle                   ,
                                                               "IMGPLUG_Unload_Image_Data" ,
                                                               2*PTRSIZE+4                 );
        _fptable[ IMGPLUG_NAME ] = CORE::GetFunctionAddress( _sohandle             ,
                                                             "IMGPLUG_Description" ,
                                                             0                     );
        _fptable[ IMGPLUG_COPYRIGHT ] = CORE::GetFunctionAddress( _sohandle                ,
                                                                  "IMGPLUG_Copyright_EULA" ,
                                                            0                        );
        _fptable[ IMGPLUG_GET_LOADABLE_EXT_LIST ] = CORE::GetFunctionAddress( _sohandle                   ,
                                                                              "IMGPLUG_Loadable_Ext_List" ,
                                                                              0                           );                                                            
        _fptable[ IMGPLUG_GET_SAVEABLE_EXT_LIST ] = CORE::GetFunctionAddress( _sohandle                   ,
                                                                              "IMGPLUG_Saveable_Ext_List" ,
                                                                              0                           );
                                                                        
        if ( ( !_fptable[ IMGPLUG_CHECK_LOADABLE_EXT ] ) ||
             ( !_fptable[ IMGPLUG_CHECK_LOADABLE_FILE ] ) ||
             ( !_fptable[ IMGPLUG_CHECK_LOADABLE_FORMAT ] ) ||
             ( !_fptable[ IMGPLUG_CHECK_SAVEABLE_FORMAT ] ) ||
             ( !_fptable[ IMGPLUG_LOAD ] ) ||
             ( !_fptable[ IMGPLUG_SAVE ] ) ||
             ( !_fptable[ IMGPLUG_UNLOAD ] ) ||
             ( !_fptable[ IMGPLUG_NAME ] ) ||
             ( !_fptable[ IMGPLUG_COPYRIGHT ] ) ||
             ( !_fptable[ IMGPLUG_GET_LOADABLE_EXT_LIST ] ) ||
             ( !_fptable[ IMGPLUG_GET_SAVEABLE_EXT_LIST ] ) )
        {        
                CORE::UnloadModuleDynamicly( _sohandle );
                _sohandle = NULL;
                
                DEBUGOUTPUT( "Invalid codec module" );
        }             
}

/*-------------------------------------------------------------------------*/

CIMGCodec::CIMGCodec( const CIMGCodec& src )
{
        DEBUGOUTPUT( "CIMGCodec::CIMGCodec( const CIMGCodec& src )" );
        
        /* should never be used */
}

/*-------------------------------------------------------------------------*/

CIMGCodec&
CIMGCodec::operator=( const CIMGCodec& src )
{
        DEBUGOUTPUT( "CIMGCodec::operator=( const CIMGCodec& src )" );
        
        /* should never be used */
        return *this;        
}

/*-------------------------------------------------------------------------*/

CIMGCodec::~CIMGCodec()
{
        DEBUGOUTPUT( "CIMGCodec::~CIMGCodec()" );
        
        UnloadModuleDynamicly( _sohandle );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::CheckIfLoadable( const CORE::CString& filename ,
                            UInt32 *hidx                  )
{
        DEBUGOUTPUT( "CIMGCodec::CheckIfLoadable( const CORE::CString& filename , UInt32 *hidx )" );
        
        return ((TIMGPLUGFPRT_Check_Ext_Loadable) _fptable[ IMGPLUG_CHECK_LOADABLE_EXT ])( filename.C_String() ,
                                                                                           hidx                ) > 0;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::CheckIfLoadable( CORE::CIOAccess& access ,
                            UInt32 *hidx            )
{
        DEBUGOUTPUT( "CIMGCodec::CheckIfLoadable( CORE::CIOAccess& access , UInt32 *hidx )" );
        
        return ((TIMGPLUGFPRT_Check_Loadable) _fptable[ IMGPLUG_CHECK_LOADABLE_FILE ])( access.CStyleAccess() ,
                                                                                        hidx                  ) > 0;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::CheckIfLoadable( Int32 format ,
                            UInt32 *hidx )
{
        DEBUGOUTPUT( "CIMGCodec::CheckIfLoadable( Int32 format , UInt32 *hidx )" );
        
        return ((TIMGPLUGFPRT_Check_FLoadable) _fptable[ IMGPLUG_CHECK_LOADABLE_FORMAT ])( format ,
                                                                                           hidx   ) > 0;
}

/*-------------------------------------------------------------------------*/

bool 
CIMGCodec::CheckIfSaveable( const CORE::CString& filename ,
                            UInt32 *hidx                  )
{
        DEBUGOUTPUT( "CIMGCodec::CheckIfSaveable( const CORE::CString& filename, UInt32 *hidx )" );
        
        return ((TIMGPLUGFPRT_Check_Ext_Saveable) _fptable[ IMGPLUG_CHECK_SAVEABLE_EXT ])( Extract_File_Ext( filename.C_String() ) ,
                                                                                           hidx                                    ) > 0;
}                            

/*-------------------------------------------------------------------------*/

bool 
CIMGCodec::CheckIfSaveable( const TImageData* imgdata ,
                            UInt32 *hidx              )
{
        DEBUGOUTPUT( "CIMGCodec::CheckIfSaveable( const TImageData* imgdata, UInt32 *hidx )" );
        
        return ((TIMGPLUGFPRT_Check_Saveable) _fptable[ IMGPLUG_CHECK_SAVEABLE_FILE ])( imgdata ,
                                                                                        hidx     ) > 0;
}                            

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::CheckIfSaveable( Int32 format ,
                            UInt32 *hidx )
{
        DEBUGOUTPUT( "CIMGCodec::CheckIfSaveable( Int32 format, UInt32 *hidx )" );
        
        return ((TIMGPLUGFPRT_Check_FSaveable) _fptable[ IMGPLUG_CHECK_SAVEABLE_FORMAT ])( format ,
                                                                                           hidx   ) > 0;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Load( UInt32 hidx             ,
                 CORE::CIOAccess& access ,
                 TImageData *imgdata     ,
                 void **plugdata         )
{
        DEBUGOUTPUT( "CIMGCodec::Load( UInt32 hidx, CORE::CIOAccess& access, TImageData *imgdata, void **plugdata )" );
        
        return ((TIMGPLUGFPRT_Load) _fptable[ IMGPLUG_LOAD ])( hidx                  ,
                                                               access.CStyleAccess() ,
                                                               imgdata               ,
                                                               plugdata              ) > 0;
}

/*-------------------------------------------------------------------------*/

void
CIMGCodec::Unload( UInt32 hidx         ,
                   TImageData* imgdata ,
                   void *plugdata      )
{
        DEBUGOUTPUT( "CIMGCodec::Unload( UInt32 hidx, TImageData* imgdata, void *plugdata )" );
        
        ((TIMGPLUGFPRT_Unload) _fptable[ IMGPLUG_UNLOAD ])( hidx     ,
                                                            imgdata  ,
                                                            plugdata );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Save( UInt32 hidx                   ,
                 const CORE::CString& filename ,
                 UInt32 format                 ,
                 UInt32 compression            ,
                 const TImageData *imgdata     )
{
        DEBUGOUTPUT( "CIMGCodec::Save( UInt32 hidx, const CORE::CString& filename, UInt32 format, UInt32 compression, const TImageData *imgdata )" );
        
        return ((TIMGPLUGFPRT_Save) _fptable[ IMGPLUG_SAVE ])( hidx                ,
                                                               filename.C_String() ,
                                                               format              ,
                                                               compression         ,
                                                               imgdata             ) > 0;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIMGCodec::GetName( void )
{
        return ((TIMGPLUGFPTR_Name) _fptable[ IMGPLUG_NAME ])();
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIMGCodec::GetCopyrightEULA( void )
{
        return ((TIMGPLUGFPTR_Copyright) _fptable[ IMGPLUG_COPYRIGHT ])();
}

/*-------------------------------------------------------------------------*/

const TVersion*
CIMGCodec::GetVersion( void )
{
        return ((TIMGPLUGFPTR_Version) _fptable[ IMGPLUG_VERSION ])();
}


/*-------------------------------------------------------------------------*/

UInt32
CIMGCodec::GetLoadCount( void ) const
{
        return _loadcount;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::GetIsValid( void ) const
{
        return _sohandle > 0;
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CIMGCodec::GetLoadableExtList( void ) const
{
        CORE::CStringList list;
        const char** clist = ((TIMGPLUGFPTR_Loadable_Ext_List) _fptable[ IMGPLUG_GET_LOADABLE_EXT_LIST ])();       
        while ( clist )
        {
                list.Append( *clist );
                ++clist;
        }
        return list;        
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CIMGCodec::GetSaveableExtList( void ) const
{
        CORE::CStringList list;
        const char** clist = ((TIMGPLUGFPTR_Saveable_Ext_List) _fptable[ IMGPLUG_GET_SAVEABLE_EXT_LIST ])();       
        while ( clist )
        {
                list.Append( *clist );
                ++clist;
        }
        return list;        
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


 
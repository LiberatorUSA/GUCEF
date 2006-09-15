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

#ifndef IMAGEDATA_H
#include "imagedata.h"             /* image data storage */
#define IMAGEDATA_H
#endif /* IMAGEDATA_H ? */

#ifndef IMGPLUGLINK_H
#include "imgpluglink.h"           /* linkage for the hardcoded codec */
#define IMGPLUGLINK_H
#endif /* IMGPLUGLINK_H ? */

#ifndef GUCEFIMGLOADERS_H
#include "GUCEFimgloaders.h"       /* access & link function for hardcoded codecs */
#define GUCEFIMGLOADERS_H
#endif /* GUCEFIMGLOADERS_H ? */ 

#ifndef CIMGCODEC_H
#include "CIMGCodec.h"             /* Image format handler codec */
#define CIMGCODEC_H
#endif /* CIMGCODEC_H ? */

#ifndef CEVENTPUMP_H
#include "CEventPump.h"            /* event pump, point for dispatching events */
#define CEVENTPUMP_H
#endif /* CEVENTPUMP_H ? */

#ifndef CEVENT_H
#include "CEvent.h"                /* event class */
#define CEVENT_H
#endif /* CEVENT_H ? */ 

#ifndef CEVENTTYPEREGISTRY_H    
#include "CEventTypeRegistry.h"    /* Registry for event types */
#define CEVENTTYPEREGISTRY_H
#endif /* CEVENTTYPEREGISTRY_H ? */

#ifndef CSTRINGLIST_H
#include "CStringList.h"           /* List of strings */
#define CSTRINGLIST_H
#endif /* CSTRINGLIST_H ? */

#include "CIMGCodecManager.h"      /* header for this class */

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

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CIMGCodecManager* CIMGCodecManager::_instance = NULL;
MT::CMutex CIMGCodecManager::_mutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CIMGCodecManager::CIMGCodecManager( void )
{
        DEBUGOUTPUT( "CIMGCodecManager::CIMGCodecManager( void )" );
        
        /*
         *      Add the Hardcoded codec at construction time making it codec 0         
         */
        TImgLoaderPlugin* hcc = new TImgLoaderPlugin;
        GUCEF_IMGPLUG_Link_Functions( hcc ); 
        _codecs.AddEntry( hcc ); 
        
        /*
         *      Register event types
         */
        CORE::CEventTypeRegistry* etr = CORE::CEventTypeRegistry::Instance();
        CHECKMEM( etr, sizeof(CORE::CEventTypeRegistry) );
        _codecaddevent = etr->RegisterType( "GUCEF::IMAGE::CIMGCODECMANAGER::IMGCODECADD" ,
                                            sizeof(TIMGCodedAdded)                        );
        _codecdelevent = etr->RegisterType( "GUCEF::IMAGE::CIMGCODECMANAGER::IMGCODECDEL" ,
                                            sizeof(TIMGCodedDeleted)                      ); 
                                            
        /*
         *      Send an event for the hardcoded codec
         */                   /*
        CORE::CEvent codecaddevent( _codecaddevent );
        CORE::CString filename( "GUCEF::IMAGE::HARDCODED" );
        CORE::CString name( "Default gucefIMAGE image codec" );  
        TIMGCodedAdded data;
        data.filename = filename.GetStructRef();
        data.index = 0;
        data.name = name.GetStructRef();
        codecaddevent.SetData( &data                  ,
                               sizeof(TIMGCodedAdded) ); 
        CORE::CEventPump::Instance()->SendEvent( codecaddevent );   */                                                                           
}

/*-------------------------------------------------------------------------*/

CIMGCodecManager::CIMGCodecManager( const CIMGCodecManager& src )
{
        DEBUGOUTPUT( "CIMGCodecManager::CIMGCodecManager( const CIMGCodecManager& src )" );
        
        /* should never be used */
}

/*-------------------------------------------------------------------------*/

CIMGCodecManager::~CIMGCodecManager()
{       
        DEBUGOUTPUT( "CIMGCodecManager::~CIMGCodecManager()" );
        
        _mutex.Lock();
        for ( Int32 i=0; i<=_codecs.GetLast(); ++i )
        {
                delete _codecs[ i ];
        }        
}

/*-------------------------------------------------------------------------*/

CIMGCodecManager&
CIMGCodecManager::operator=( const CIMGCodecManager& src )
{
        DEBUGOUTPUT( "CIMGCodecManager::operator=( const CIMGCodecManager& src )" );
        
        /* should never be used */
        return *this;
}

/*-------------------------------------------------------------------------*/

CIMGCodecManager*
CIMGCodecManager::Instance( void )
{
        DEBUGOUTPUT( "CIMGCodecManager::Instance( void )" );
        
        _mutex.Lock();
        if ( !_instance )
        {
                _instance = new CIMGCodecManager();
                CHECKMEM( _instance, sizeof(CIMGCodecManager) );
        }
        _mutex.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

Int32
CIMGCodecManager::AddCodec( const CORE::CString& filename )
{
        DEBUGOUTPUT( "CIMGCodecManager::AddCodec( const CORE::CString& filename )" );
        
        _mutex.Lock();
        CIMGCodec* codec = new CIMGCodec( filename );
        if ( codec->GetIsValid() )
        {
                Int32 idx = _codecs.AddEntry( codec );
                
                /*
                 *      Send an event notifying the rest of the system 
                 *      that a new image codec is available
                 */
                CORE::CEvent codecaddevent( _codecaddevent );                
                TIMGCodedAdded data;
                data.filenamestr = filename.Cache();
                data.index = idx;
                data.namestr = codec->GetName().Cache();
                codecaddevent.SetData( &data                  ,
                                       sizeof(TIMGCodedAdded) ); 
                CORE::CEventPump::Instance()->SendEvent( codecaddevent );
                
                _mutex.Unlock();
                return idx;
        }
        else
        {
                delete codec;
                _mutex.Unlock();
                return -1;
        }
}

/*-------------------------------------------------------------------------*/


/**
 *      Attempts to delete the given codec.
 *      Note that deleting a codec will cause the dynamicly linked
 *      codec module to be unloaded. A codec module cannot be unloaded
 *      unless it's loadcount is zero. So be sure to use
 *      UnloadImageData() as often as LoadImageData().
 */
bool
CIMGCodecManager::DeleteCodec( UInt32 codecidx )
{
        DEBUGOUTPUT( "CIMGCodecManager::DeleteCodec( UInt32 codecidx )" );
        
        _mutex.Lock();
        
        /*
         *      Index 0 is the hardcoded codec which cannot be deleted.
         */
        if ( !codecidx ) return false;
        
        /*
         *      If the index is valid and there are no outstanding image loads  
         *      we will now remove the codec
         */
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( !codec->GetLoadCount() )
                {
                        /*
                         *      Send an event notifying the rest of the system 
                         *      that an image codec is no longer available
                         */
                        CORE::CEvent codecaddevent( _codecdelevent );
                        TIMGCodedDeleted data;
                        data.index = codecidx;
                        data.namestr = codec->GetName().Cache();
                        codecaddevent.SetData( &data                    ,
                                               sizeof(TIMGCodedDeleted) ); 
                        CORE::CEventPump::Instance()->SendEvent( codecaddevent );
                                        
                        delete codec;
                        _codecs.SetEntry( codecidx, NULL );                        
                                        
                        _mutex.Unlock();
                        return true;
                }
        }
        _mutex.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/

/**
 *      Attempts to load image data using the plugin identified with
 *      the provided handle. The plugin will attempt to load the image
 *      data from the recource provided by 'access' and store the
 *      result in 'imgdata'.
 */
bool
CIMGCodecManager::LoadImageData( UInt32 codecidx         ,
                                 UInt32 hidx             ,
                                 CORE::CIOAccess& access ,
                                 TImageData* imgdata     ,
                                 void** plugdata         )
{
        DEBUGOUTPUT( "CIMGCodecManager::LoadImageData( UInt32 codecidx, UInt32 hidx, CORE::CIOAccess& access, TImageData* imgdata, void** plugdata )" );
        
        _mutex.Lock();
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                bool retval = ( hcc->IMGPLUG_Load_Image_Data( hidx                  ,
                                                              access.CStyleAccess() ,
                                                              imgdata               ,
                                                              plugdata              ) > 0 );
                if ( retval ) ++hcc->loadcount;                                                            
                _mutex.Unlock();
                return retval;                                                                                  
        }                                                            
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( codec )
                {
                        bool retval = codec->Load( hidx     ,
                                                   access   ,
                                                   imgdata  ,
                                                   plugdata );
                        _mutex.Unlock();
                        return retval;
                }
        }
        _mutex.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/

/**
 *      Use this member function to unload image data that was
 *      obtained trough a call to LoadImageData(). The same
 *      handle and imagedata storage structure should be passed that
 *      where used to load them. Note that a codec cannot be deleted
 *      unless it's loadcount is zero.
 */
void
CIMGCodecManager::UnloadImageData( UInt32 codecidx     ,
                                   UInt32 hidx         ,
                                   TImageData* imgdata ,
                                   void* plugdata      )
{
        DEBUGOUTPUT( "CIMGCodecManager::UnloadImageData( UInt32 codecidx, UInt32 hidx, TImageData* imgdata, void* plugdata )" );
        
        _mutex.Lock();
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                hcc->IMGPLUG_Unload_Image_Data( hidx     ,
                                                imgdata  ,
                                                plugdata );
                --hcc->loadcount;                                                              
                _mutex.Unlock();                                                                                
        }         
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( codec )
                {
                        codec->Unload( hidx     ,
                                       imgdata  ,
                                       plugdata );
                }
        }
        _mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

/**
 *      This function should save the image data provided in the format
 *      provided to a file with the given filename. If format is one of the
 *      explicit types then the save must be in that format or the save
 *      operation should fail. if format is IMGFORMAT_DONT_CARE then the
 *      format of the output file can be decided uppon at the user's discression.
 *      A return value of 0 indicates failure. A return value of > 0 indicates
 *      success (I recommend using 1). compression is a percentage between 0
 *      and 100 indicating the desired amount of compression. 0 indicates the
 *      lowest level of compression the plugin can provide and 100 the highest.
 */
bool
CIMGCodecManager::SaveImageData( UInt32 codecidx               ,
                                 UInt32 hidx                   ,
                                 const CORE::CString& filename ,
                                 UInt32 format                 ,
                                 UInt32 compression            ,
                                 TImageData* imgdata           )
{
        DEBUGOUTPUT( "CIMGCodecManager::SaveImageData( UInt32 codecidx, UInt32 hidx, const CORE::CString& filename, UInt32 format, UInt32 compression, TImageData* imgdata )" );
        
        _mutex.Lock();
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                bool retval = ( hcc->IMGPLUG_Save_Image_Data( hidx                ,
                                                              filename.C_String() ,
                                                              format              ,
                                                              compression         ,
                                                              imgdata             ) > 0 );
                _mutex.Unlock();
                return retval;                                                                                  
        }            
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( codec )
                {
                        bool retval = codec->Save( hidx        , 
                                                   filename    ,
                                                   format      ,
                                                   compression ,
                                                   imgdata     );
                        _mutex.Unlock();
                        return retval;
                }
        }
        _mutex.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/

/**
 *      Returns the given codec's copyright/EULA information.
 */
CORE::CString
CIMGCodecManager::GetCopyrightEULA( UInt32 codecidx )
{
        DEBUGOUTPUT( "CIMGCodecManager::GetCopyrightEULA( UInt32 codecidx )" );

        _mutex.Lock();
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                CORE::CString retval = hcc->IMGPLUG_Copyright();
                _mutex.Unlock();
                return retval;                                                                                  
        }          
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( codec )
                {
                        CORE::CString retval = codec->GetCopyrightEULA();
                        _mutex.Unlock();
                        return retval;
                }
        }
        _mutex.Unlock();
        CORE::CString nullstr;
        return nullstr;
}

/*-------------------------------------------------------------------------*/

/**
 *      Returns the given codec's description information.
 */
CORE::CString
CIMGCodecManager::GetDescription( UInt32 codecidx )
{
        DEBUGOUTPUT( "CIMGCodecManager::GetDescription( UInt32 codecidx )" );
        
        _mutex.Lock();
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                CORE::CString retval = hcc->IMGPLUG_Name();
                _mutex.Unlock();
                return retval;                                                                                  
        }          
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( codec )
                {
                        CORE::CString retval = codec->GetName();
                        _mutex.Unlock();
                        return retval;
                }
        }
        _mutex.Unlock();
        CORE::CString nullstr;
        return nullstr;
}

/*-------------------------------------------------------------------------*/

/**
 *      Returns the given codec's version information.
 */
const CORE::TVersion*
CIMGCodecManager::GetVersion( UInt32 codecidx )
{
        DEBUGOUTPUT( "CIMGCodecManager::GetVersion( UInt32 codecidx )" );
        
        _mutex.Lock();
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                const CORE::TVersion* retval = hcc->IMGPLUG_Version();
                _mutex.Unlock();
                return retval;                                                                                  
        }          
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( codec )
                {
                        const CORE::TVersion* retval = codec->GetVersion();
                        _mutex.Unlock();
                        return retval;
                }
        }
        _mutex.Unlock();
        return NULL;
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CIMGCodecManager::GetLoadableExtList( UInt32 codecidx )
{
        DEBUGOUTPUT( "CIMGCodecManager::GetLoadableExtList( UInt32 codecidx )" );
        
        _mutex.Lock();
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                const char** clist = hcc->IMGPLUG_Loadable_Ext_List();
                
                CORE::CStringList list;
                while ( *clist )
                {               
                        list.Append( *clist );                        
                        ++clist;
                }
                
                _mutex.Unlock();
                return list;                                                                                  
        }          
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( codec )
                {
                        CORE::CStringList list = codec->GetLoadableExtList();
                        _mutex.Unlock();
                        return list;
                }
        }
        _mutex.Unlock();
        
        CORE::CStringList emptylist;
        return emptylist;
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CIMGCodecManager::GetSaveableExtList( UInt32 codecidx )
{
        DEBUGOUTPUT( "CIMGCodecManager::GetSaveableExtList( UInt32 codecidx )" );
        
        _mutex.Lock();
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                const char** clist = hcc->IMGPLUG_Saveable_Ext_List();
                
                CORE::CStringList list;
                while ( *clist )
                {               
                        list.Append( *clist );                        
                        ++clist;
                }
                
                _mutex.Unlock();
                return list;                                                                                  
        }          
        if ( (Int32)codecidx <= _codecs.GetLast() )
        {
                CIMGCodec* codec = (CIMGCodec*) _codecs[ codecidx ];
                if ( codec )
                {
                        CORE::CStringList list = codec->GetSaveableExtList();
                        _mutex.Unlock();
                        return list;
                }
        }
        _mutex.Unlock();
        
        CORE::CStringList emptylist;
        return emptylist;
}

/*-------------------------------------------------------------------------*/

/**
 *      Checks wheter the codec with the index given 
 *      can load the given format.
 */        
bool 
CIMGCodecManager::CheckIfLoadable( UInt32 codecidx               ,
                                   const CORE::CString& filename ,
                                   UInt32 *hidx                  )
{
        DEBUGOUTPUT( "CIMGCodecManager::CheckIfLoadable( UInt32 codecidx, const CORE::CString& filename, UInt32 *hidx )" );
        
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                return hcc->IMGPLUG_Check_If_Loadable_Using_Ext( filename.C_String() ,
                                                                 hidx                ) > 0;                
        }
        
        return ( (CIMGCodec*) _codecs[ codecidx ] )->CheckIfLoadable( filename ,
                                                                      hidx     );
}                                   
                
/*-------------------------------------------------------------------------*/                

/**
*      Checks wheter the codec with the index given 
*      can load the given format.
*/
bool 
CIMGCodecManager::CheckIfLoadable( UInt32 codecidx         ,
                                   CORE::CIOAccess& access ,
                                   UInt32 *hidx            )

{
        DEBUGOUTPUT( "CIMGCodecManager::CheckIfLoadable( UInt32 codecidx, CORE::CIOAccess& access, UInt32 *hidx )" );
        
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                return hcc->IMGPLUG_Check_If_Loadable( access.CStyleAccess() ,
                                                       hidx                  ) > 0;          
        }
        
        return ( (CIMGCodec*) _codecs[ codecidx ] )->CheckIfLoadable( access ,
                                                                      hidx   );
}                                   
                
/*-------------------------------------------------------------------------*/                    

/**
*      Checks wheter the codec with the index given 
*      can load the given format.
*/
bool 
CIMGCodecManager::CheckIfLoadable( UInt32 codecidx ,
                                   Int32 format    ,
                                   UInt32 *hidx    )
{
        DEBUGOUTPUT( "CIMGCodecManager::CheckIfLoadable( UInt32 codecidx, Int32 format, UInt32 *hidx )" );
        
        if ( !codecidx )
        {
                TImgLoaderPlugin* hcc = (TImgLoaderPlugin*) _codecs[ 0 ];
                return hcc->IMGPLUG_Check_If_Format_Loadable( format ,
                                                              hidx   ) > 0;         
        }
        
        return ( (CIMGCodec*) _codecs[ codecidx ] )->CheckIfLoadable( format ,
                                                                      hidx   );
} 

/*-------------------------------------------------------------------------*/

/**
 *      Checks wheter the codec with the index given 
 *      can load the given format.
 */        
bool 
CIMGCodecManager::CheckIfSaveable( UInt32 codecidx               ,
                                   const CORE::CString& filename ,
                                   UInt32 *hidx                  )
{
        return ( (CIMGCodec*) _codecs[ codecidx ] )->CheckIfSaveable( filename ,
                                                                      hidx     );
}   

/*-------------------------------------------------------------------------*/                                

/**
 *      Checks wheter the codec with the index given 
 *      can load the given format.
 */
bool 
CIMGCodecManager::CheckIfSaveable( UInt32 codecidx           ,
                                   const TImageData* imgdata ,
                                   UInt32 *hidx              )
{
        return ( (CIMGCodec*) _codecs[ codecidx ] )->CheckIfSaveable( imgdata ,
                                                                      hidx    );
} 

/*-------------------------------------------------------------------------*/                                  

/**
 *      Checks wheter the codec with the index given 
 *      can load the given format.
 */
bool 
CIMGCodecManager::CheckIfSaveable( UInt32 codecidx ,
                                   Int32 format    ,
                                   UInt32 *hidx    )
{
        return ( (CIMGCodec*) _codecs[ codecidx ] )->CheckIfSaveable( format ,
                                                                      hidx   );
}

/*-------------------------------------------------------------------------*/ 

void 
CIMGCodecManager::LoadAll( void )
{
        DEBUGOUTPUT( "CIMGCodecManager::LoadAll( void )" );
        
        _mutex.Lock();        
        
        CORE::CString file;
        CORE::CString filepath; 
        CORE::CString path = GetPluginDir();
        
        struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( path.C_String() );
        if ( did )
        {
                do {                
                        if ( CORE::DI_Is_It_A_File( did ) )
                        {
                                DEBUGOUTPUTsss( "Attempt to load ", CORE::DI_Name( did ), " as an IMGCodec plugin" );
                                
                                filepath = path; 
                                file = CORE::DI_Name( did );
                                CORE::AppendToPath( filepath, file );
                                AddCodec( filepath );
                        }
                }
                while ( CORE::DI_Next_Dir_Entry( did ) != 0 );
                
                CORE::DI_Cleanup( did );
        }
        
        _mutex.Unlock();
}

/*-------------------------------------------------------------------------*/
        
void 
CIMGCodecManager::UnloadAll( void )
{
        for ( Int32 i=0; i<_codecs.GetLast(); ++i )
        {
                DeleteCodec( i );        
        }
}

/*-------------------------------------------------------------------------*/

void 
CIMGCodecManager::OnSetPluginDir( const CORE::CString& path )
{
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


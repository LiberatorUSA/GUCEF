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

#ifndef CIMGCODECMANAGER_H
#define CIMGCODECMANAGER_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCORE_H
#include "gucefCORE.h"             /* CORE Library of the GUCEFramework */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"     /* IMAGE Library macros */
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

#ifndef IMAGEDATA_H
#include "imagedata.h"             /* image data storage */
#define IMAGEDATA_H
#endif /* IMAGEDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class EXPORT_CPP CIMGCodecManager : public CORE::CIPluginManager
{
        public:      
        
        /**
         *      Structure that holds event data for the
         *      "GUCEF::IMAGE::CIMGCODECMANAGER::IMGCODECADD" event
         */
        struct SIMGCodedAdded
        {                
                UInt32 filenamestr;         /**< string cacheid: The filename of the codec */
                UInt32 namestr;             /**< string cacheid: Name of the codec */
                UInt32 index;               /**< Manager index for the codec */    
        };
        typedef struct SIMGCodedAdded TIMGCodedAdded;  
        
        /**
         *      Structure that holds event data for the
         *      "GUCEF::IMAGE::CIMGCODECMANAGER::IMGCODECDEL" event
         */
        struct SIMGCodedDeleted
        {
                UInt32 namestr;        /**< string cacheid: Name of the codec */
                UInt32 index;          /**< Manager index for the codec */          
        };
        typedef struct SIMGCodedDeleted TIMGCodedDeleted;          

        static CIMGCodecManager* Instance( void );

        /**
         *      Attempts to load a codec file which is a dynamicly linked
         *      library.
         *
         *      Dispatches the following events:
         *              - "GUCEF::IMAGE::CIMGCODECMANAGER::IMGCODECADD"
         *                      This event has the following data:
         *                              - TIMGCodedAdded        
         */
        Int32 AddCodec( const CORE::CString& filename );

        /**
         *      Attempts to delete the given codec.
         *      Note that deleting a codec will cause the dynamicly linked
         *      codec module to be unloaded. A codec module cannot be unloaded
         *      unless it's loadcount is zero. So be sure to use
         *      UnloadImageData() as often as LoadImageData().
         *
         *      Dispatches the following events:
         *              - "GUCEF::IMAGE::CIMGCODECMANAGER::IMGCODECDEL"   
         *                      This event has the following data:
         *                              - TIMGCodedDeleted     
         */
        bool DeleteCodec( UInt32 codecidx );
        
        /**
         *      Checks wheter the codec with the index given 
         *      can load the given format.
         */        
        bool CheckIfLoadable( UInt32 codecidx               ,
                              const CORE::CString& filename ,
                              UInt32 *hidx                  );

        /**
         *      Checks wheter the codec with the index given 
         *      can load the given format.
         */
        bool CheckIfLoadable( UInt32 codecidx         ,
                              CORE::CIOAccess& access ,
                              UInt32 *hidx            );        
        
        /**
         *      Checks wheter the codec with the index given 
         *      can load the given format.
         */
        bool CheckIfLoadable( UInt32 codecidx ,
                              Int32 format    ,
                              UInt32 *hidx    ); 
                              
        /**
         *      Checks wheter the codec with the index given 
         *      can load the given format.
         */        
        bool CheckIfSaveable( UInt32 codecidx               ,
                              const CORE::CString& filename ,
                              UInt32 *hidx                  );

        /**
         *      Checks wheter the codec with the index given 
         *      can load the given format.
         */
        bool CheckIfSaveable( UInt32 codecidx           ,
                              const TImageData* imgdata ,
                              UInt32 *hidx              );        
        
        /**
         *      Checks wheter the codec with the index given 
         *      can load the given format.
         */
        bool CheckIfSaveable( UInt32 codecidx ,
                              Int32 format    ,
                              UInt32 *hidx    );                               

        /**
         *      Attempts to load image data using the plugin identified with
         *      the provided handle. The plugin will attempt to load the image
         *      data from the recource provided by 'access' and store the
         *      result in 'imgdata'.
         */
        bool LoadImageData( UInt32 codecidx         ,
                            UInt32 hidx             ,
                            CORE::CIOAccess& access ,
                            TImageData* imgdata     ,
                            void** plugdata         );

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
        bool SaveImageData( UInt32 codecidx               ,
                            UInt32 hidx                   ,
                            const CORE::CString& filename ,
                            UInt32 format                 ,
                            UInt32 compression            ,
                            TImageData* imgdata           );

        /**
         *      Use this member function to unload image data that was
         *      obtained trough a call to LoadImageData(). The same
         *      handle and imagedata storage structure should be passed that
         *      where used to load them. Note that a codec cannot be deleted
         *      unless it's loadcount is zero.
         */
        void UnloadImageData( UInt32 codecidx     ,
                              UInt32 hidx         ,
                              TImageData* imgdata ,
                              void* plugdata      );

        /**
         *      Returns the given codec's copyright/EULA information.
         */
        CORE::CString GetCopyrightEULA( UInt32 codecidx );

        /**
         *      Returns the given codec's description information.
         */
        CORE::CString GetDescription( UInt32 codecidx );

        /**
         *      Returns the given codec's version information.
         */        
        const CORE::TVersion* GetVersion( UInt32 codecidx );
        
        CORE::CStringList GetLoadableExtList( UInt32 codecidx );
        
        CORE::CStringList GetSaveableExtList( UInt32 codecidx );
        
        virtual void LoadAll( void );
        
        virtual void UnloadAll( void );
        
        protected:
        
        virtual void OnSetPluginDir( const CORE::CString& path );        

        private:

        CIMGCodecManager( void );
        CIMGCodecManager( const CIMGCodecManager& src );
        virtual ~CIMGCodecManager();
        CIMGCodecManager& operator=( const CIMGCodecManager& src );

        CORE::CDynamicArray _codecs;
        UInt32 _codecaddevent;
        UInt32 _codecdelevent;

        static MT::CMutex _mutex;
        static CIMGCodecManager* _instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CIMGCODECMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Designed and implemented this class.
          This class is a replacement of the old CImagePluginManager class.

-----------------------------------------------------------------------------*/
 
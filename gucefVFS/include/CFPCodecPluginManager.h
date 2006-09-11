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

#ifndef CFPCODECPLUGINMANAGER_H
#define CFPCODECPLUGINMANAGER_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFMT_H
#include "gucefMT.h"             /* MT Library of the GUCEFramework */ 
#define GUCEFMT_H
#endif /* GUCEFMT_H ? */

#include "CIPluginManager.h"
#include "CIConfigurable.h"

#ifndef GUCEFVFS_MACROS_H
#include "gucefVFS_macros.h"     /* gucefVFS Library macros */
#define GUCEFVFS_MACROS_H
#endif /* GUCEFVFS_MACROS_H ? */

#ifndef CFPCODECPLUGIN_H
#include "CFPCodecPlugin.h"      /* file pack codec plugin class */
#define CFPCODECPLUGIN_H
#endif /* CFPCODECPLUGIN_H ? */

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

class EXPORT_CPP CFPCodecPluginManager : public CORE::CIPluginManager ,
                                         public CORE::CIConfigurable
{
        public:      
        
        /**
         *      Structure that holds event data for the
         *      "GUCEF::VFS::CFPCODECMANAGER::FPCODECADD" event
         */
        struct SFPCodedAdded
        {                
                UInt32 filenamestr;  /**< string cacheid: The filename of the codec */
                UInt32 namestr;      /**< string cacheid: Name of the codec */
                UInt32 index;        /**< Manager index for the codec */    
        };
        typedef struct SFPCodedAdded TFPCodedAdded;  
        
        /**
         *      Structure that holds event data for the
         *      "GUCEF::VFS::CFPCODECMANAGER::FPCODECDEL" event
         */
        struct SFPCodedDeleted
        {
                UInt32 namestr; /**< string cacheid: the name of the codec */
                UInt32 index;   /**< Manager index for the codec */          
        };
        typedef struct SFPCodedDeleted TFPCodedDeleted;          

        static CFPCodecPluginManager* Instance( void );

        /**
         *      Attempts to load a codec file which is a dynamicly linked
         *      library.
         *
         *      Dispatches the following events:
         *              - "GUCEF::VFS::CFPCODECMANAGER::FPCODECADD"
         *                      This event has the following data:
         *                              - TIMGCodedAdded        
         */
        CFPCodecPlugin* AddCodec( const CORE::CString& filename );

        /**
         *      Attempts to delete the given codec.
         *      Note that deleting a codec will cause the dynamicly linked
         *      codec module to be unloaded. A codec module cannot be unloaded
         *      unless it's loadcount is zero. So be sure to use
         *      UnloadImageData() as often as LoadImageData().
         *
         *      Dispatches the following events:
         *              - "GUCEF::VFS::CFPCODECMANAGER::FPCODECDEL"
         *                      This event has the following data:
         *                              - TFPCodedDeleted     
         */
        bool DeleteCodec( CFPCodecPlugin* fpcodec );
        
        CORE::CStringList GetCodecExtList( void ) const;
        
        CFPCodecPlugin* GetCodec( UInt32 codecidx ) const;
        
        CFPCodecPlugin* GetCodec( const CORE::CString fileext ) const;
        
        virtual void LoadAll( void );
        
        virtual void UnloadAll( void );
        
        /**
         *      Attempts to store the given tree in the file
         *      given according to the method of the codec metadata
         *
         *      @param tree the data tree you wish to store
         *      @return wheter storing the tree was successfull
         */
        virtual bool SaveConfig( CORE::CDataNode& tree );
                                    
        /**
         *      Attempts to load data from the given file to the 
         *      root node given. The root data will be replaced 
         *      and any children the node may already have will be deleted.
         *
         *      @param treeroot pointer to the node that is to act as root of the data tree
         *      @return whether building the tree from the given file was successfull.
         */                                    
        virtual bool LoadConfig( const CORE::CDataNode& treeroot );        
        
        protected:
        
        virtual void OnSetPluginDir( const CORE::CString& path );
        
        private:
        friend class CGUCEFVFSModule;
        
        static void Deinstance( void );        

        private:
        
        bool IsPluginLoaded( const CORE::CString& path );

        CFPCodecPluginManager( void );
        CFPCodecPluginManager( const CFPCodecPluginManager& src );
        ~CFPCodecPluginManager();
        CFPCodecPluginManager& operator=( const CFPCodecPluginManager& src );

        CORE::CDynamicArray _codecs;
        UInt32 _codecaddevent;
        UInt32 _codecdelevent;

        static MT::CMutex _mutex;
        static CFPCodecPluginManager* _instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CFPCODECPLUGINMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-09-2005 :
        - Added check that makes sure the same module is not loaded twice
- 27-11-2004 :
        - Designed and implemented this class.
          This class is a replacement of the old CImagePluginManager class.

-----------------------------------------------------------------------------*/

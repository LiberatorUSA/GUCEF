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

#ifndef GUCEF_CORE_CSTDCODECPLUGIN_H
#define GUCEF_CORE_CSTDCODECPLUGIN_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICODECPLUGIN_H
#include "CICodecPlugin.h"
#define GUCEF_CORE_CICODECPLUGIN_H
#endif /* GUCEF_CORE_CICODECPLUGIN_H ? */

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

/**
 *  GUCEF CORE module plugin implementation for codecs
 */
class GUCEFCORE_EXPORT_CPP CStdCodecPlugin : public CICodecPlugin
{            
    public:
    
    CStdCodecPlugin( void );
    
    virtual ~CStdCodecPlugin();
    
    virtual bool GetCodecs( CCodecSet& codecSet );
    
    virtual bool GetCodecList( CCodecList& codecList );

    virtual bool Encode( CIOAccess& source         ,
                         CIOAccess& dest           ,
                         const CString& familyName ,
                         const CString& typeName   );

    virtual bool Decode( CIOAccess& source         ,
                         CIOAccess& dest           ,
                         const CString& familyName ,
                         const CString& typeName   );

    virtual CString GetDescription( void ) const;

    virtual CString GetCopyright( void ) const;
    
    virtual TVersion GetVersion( void ) const;
    
    virtual CString GetModulePath( void ) const;
    
    virtual bool IsLoaded( void ) const;
    
    virtual bool Load( const CString& pluginPath );
    
    virtual bool Unload( void );
                 
    private:

    CStdCodecPlugin( const CStdCodecPlugin& src );            /**< not implemented */
    CStdCodecPlugin& operator=( const CStdCodecPlugin& src ); /**< not implemented */
    
    void LinkCodecSet( void );
    void UnlinkCodecSet( void );
    
    private:
    
    CCodecSet m_codecSet;
    CCodecList m_codecList;
    CString m_modulePath;
    void* m_fpTable[ 10 ];  /**< function pointer table */
    void* m_soHandle;       /**< access to the so module */
    void* m_pluginData;    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace CORE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTDCODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 15-12-2006 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
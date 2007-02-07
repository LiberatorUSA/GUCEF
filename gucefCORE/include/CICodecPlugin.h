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

#ifndef GUCEF_CORE_CICODECPLUGIN_H
#define GUCEF_CORE_CICODECPLUGIN_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_CORE_CICODEC_H
#include "CICodec.h"
#define GUCEF_CORE_CICODEC_H
#endif /* GUCEF_CORE_CICODEC_H ? */

#ifndef GUCEF_CORE_CIPLUGIN_H
#include "CIPlugin.h"
#define GUCEF_CORE_CIPLUGIN_H
#endif /* GUCEF_CORE_CIPLUGIN_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

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
 *  Interface class for plugin's that provide a number of codec's
 *
 *  Note that the CTCodecPluginItem template can be used by the class 
 *  decending from this class to specify it's codec's delegation to the 
 *  plugin module with ease. This allows you to use a more generic class
 *  as the utility interface and base class for the given codec family.
 */
class GUCEFCORE_EXPORT_CPP CICodecPlugin : public CIPlugin
{
    public:
    
    typedef CTSharedPtr< CICodec > CCodecPtr;
    typedef std::map< CString, CCodecPtr > CCodecFamilySet;
    typedef std::map< CString, CCodecFamilySet > CCodecSet;
    typedef std::vector< CString > CCodecFamilyList;    
    typedef std::map< CString, CCodecFamilyList > CCodecList;
            
    public:
    
    CICodecPlugin( void );
    
    virtual ~CICodecPlugin();
    
    virtual bool GetCodecs( CCodecSet& codecSet ) = 0;
    
    virtual bool GetCodecList( CCodecList& codecList ) = 0;

    virtual bool Encode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         CIOAccess& dest                ,
                         UInt32& destBytesWritten       ,
                         const CString& familyName      ,
                         const CString& typeName        ) = 0;

    virtual bool Decode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         CIOAccess& dest                ,
                         UInt32& destBytesWritten       ,
                         const CString& familyName      ,
                         const CString& typeName        ) = 0;
                 
    private:

    CICodecPlugin( const CICodecPlugin& src );
    CICodecPlugin& operator=( const CICodecPlugin& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace CORE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CICODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 15-12-2006 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
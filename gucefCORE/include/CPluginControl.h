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

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#define GUCEF_CORE_CPLUGINCONTROL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_GUCEFMT_H
#include "gucefMT.h"                   /* MT library API */
#define GUCEF_MT_GUCEFMT_H
#endif /* GUCEF_MT_GUCEFMT_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"                /* platform string implementation */  
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"            /* platform dynamic array implementation */  
#define GUCEF_CORE_CDYNAMICARRAY_H 
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"         /* often used gucef macros */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

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

/*
 *      Forward declarations of framework classes used here
 */
class CPluginManager;
class CString;

/*-------------------------------------------------------------------------*/

/**
 *      Global framework plugin control center
 *      All plugin managers derived from CPluginManager automaticly register here.
 *      
 */
class EXPORT_CPP CPluginControl
{
        public:
        
        static CPluginControl* Instance( void );             
        
        void LoadAll( void );
        
        void UnloadAll( void );
        
        void SetPluginDir( const CString& path );
        
        CString GetPluginDir( void ) const;   
        
        private:
        friend class CPluginManager;
        
        void Register( CPluginManager* pman );
        
        void Unregister( CPluginManager* pman );
        
        private:
        friend class CGUCEFCOREModule;
        
        static void Deinstance( void );
        
        private:
        
        CPluginControl( void );
        CPluginControl( const CPluginControl& src );
        ~CPluginControl();
        CPluginControl& operator=( const CPluginControl& src );
        
        static MT::CMutex _mutex;
        static CPluginControl* _instance;
        
        CString _plugindir;
        CDynamicArray _managers;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Initial implementation

-----------------------------------------------------------------------------*/

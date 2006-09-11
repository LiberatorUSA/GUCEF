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

#ifndef GUCEF_CORE_CSYSCONSOLECLIENT_H
#define GUCEF_CORE_CSYSCONSOLECLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"               /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CSTRINGLIST_H
#include "CStringList.h"          /* string container */
#define GUCEF_CORE_CSTRINGLIST_H
#endif /* GUCEF_CORE_CSTRINGLIST_H ? */

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
 *      Forward declaration of classes used.
 */
class CSysConsole;
class CISysConsoleCmdHandler;

/*-------------------------------------------------------------------------*/

/**
 *
 */
class EXPORT_CPP CSysConsoleClient
{
        protected:
        
        CSysConsoleClient( void );
        
        CSysConsoleClient( const CSysConsoleClient& src );
                      
        virtual ~CSysConsoleClient();
        
        CSysConsoleClient& operator=( const CSysConsoleClient& src );
        
        bool RegisterCmd( const CString& path                ,
                          const CString& command             ,
                          const CStringList& args            ,
                          CISysConsoleCmdHandler* cmdhandler );                        

        void UnregisterCmd( const CString& path    ,
                            const CString& command );
        
        void LeaveDir( void );
        
        bool EnterDir( const CString& dirname );
        
        bool JumpTo( const CString& path );
        
        const CString& GetPath( void ) const;
        
        bool Execute( const CString& funcname    ,
                      const CStringList& arglist ,
                      CStringList& resultdata    );
        
        CStringList GetDirList( void ) const;
        
        CStringList GetCmdList( void ) const;
        
        virtual void OnReturnData( const CStringList& data );
        
        private:
        friend class CSysConsole;
                
        void* channel;
        CString _path;
        
        private:
        CSysConsole* _system;         
                
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSYSCONSOLECLIENT_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-05-2005 :
        - Applied file layout standard

--------------------------------------------------------------------------*/

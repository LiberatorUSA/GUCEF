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

#ifndef GUCEF_CORE_CSYSCONSOLE_H
#define GUCEF_CORE_CSYSCONSOLE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CSTRINGLIST_H
#include "CStringList.h"          /* string container */
#define GUCEF_CORE_CSTRINGLIST_H
#endif /* GUCEF_CORE_CSTRINGLIST_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CISYSCONSOLECMDHANDLER_H
#include "CISysConsoleCmdHandler.h"
#define GUCEF_CORE_CISYSCONSOLECMDHANDLER_H
#endif /* GUCEF_CORE_CISYSCONSOLECMDHANDLER_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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
class CSysConsoleClient;

/*-------------------------------------------------------------------------*/

/**
 *      Central hub for system console command interaction.
 *      system console clients can use the system console to
 *      iterate the command listing and manipulate it.
 *      handlers can then process commands with there parameters
 *      as sent by system console clients.
 *
 *      Think of it as a DOS-console/Bash but instead of a storage
 *      medium you can move trough a tree of system commands.
 */ 
class GUCEFCORE_EXPORT_CPP CSysConsole : public CIConfigurable         ,
                                         public CISysConsoleCmdHandler
{
        public:
        
        static CSysConsole* Instance( void );
                                    
        bool RegisterCmd( const CString& path                ,
                          const CString& command             ,
                          const CStringList& args            ,
                          CISysConsoleCmdHandler* cmdhandler );                             

        void UnregisterCmd( const CString& path    ,
                            const CString& command );
                            
        bool RegisterAlias( const CString& aliasname ,
                            const CString& path      ,
                            const CString& function  );
                            
        bool UnregisterAlias( const CString& aliasname ,
                              const CString& path      ,
                              const CString& function  );                                                        

        /**
         *      Attempts to store the given tree in the file
         *      given according to the method of the codec metadata
         *
         *      @param tree the data tree you wish to store
         *      @return wheter storing the tree was successfull
         */
        virtual bool SaveConfig( CDataNode& tree );
                                    
        /**
         *      Attempts to load data from the given file to the 
         *      root node given. The root data will be replaced 
         *      and any children the node may already have will be deleted.
         *
         *      @param treeroot pointer to the node that is to act as root of the data tree
         *      @return whether building the tree from the given file was successfull.
         */                                    
        virtual bool LoadConfig( const CDataNode& treeroot );

        public:
        
        struct SCmdChannel;
        struct SFunctionHook;
                        
        private:
        friend class CSysConsoleClient;        

        void LeaveDir( CSysConsoleClient* client );
        
        bool EnterDir( CSysConsoleClient* client ,
                       const CString& dirname );
        
        bool JumpTo( CSysConsoleClient* client ,
                     const CString& path );
        
        bool Execute( CSysConsoleClient* client  ,
                      const CString& funcname    ,
                      const CStringList& arglist ,
                      CStringList& resultdata    );
        
        CStringList GetDirList( const CSysConsoleClient* client ) const;
        
        CStringList GetCmdList( const CSysConsoleClient* client ) const;
        
        void InitClient( CSysConsoleClient* client );
        
        void UnregClient( CSysConsoleClient* client );

        private:
        friend class CGUCEFCOREModule;
        
        static void Deinstance( void );
        
        private:

        struct SAliasData
        {
            CString path;
            CString function;
        };
        typedef struct SAliasData TAliasData;
        typedef std::map< CString, TAliasData > TAliasList;
        
        CSysConsole( void );
        CSysConsole( const CSysConsole& src );        
        ~CSysConsole();
        CSysConsole& operator=( const CSysConsole& src );
        
        struct SCmdChannel* FindChannel( struct SCmdChannel* curchannel ,
                                         const CString& name            );

        struct SCmdChannel* BuildTree( struct SCmdChannel* curchannel ,
                                       const CString& path            );
                                       
        void DelTree( struct SCmdChannel* tree );                                                
                                          
        struct SCmdChannel* WalkTree( struct SCmdChannel* curchannel ,
                                      const CString& path            ,
                                      CString& leftover              );
                                         
        struct SFunctionHook* FindFunction( const struct SCmdChannel* curchannel ,
                                            const CString& functionname          );
                                            
        TAliasData* FindAliasFunction( const CString& aliasname );
        
        bool OnSysConsoleCommand( const CString& path     ,
                                  const CString& command  ,
                                  const CStringList& args ,
                                  CStringList& resultdata );
        
        struct SCmdChannel* _root;
        TAliasList _aliases;
        
        static CSysConsole* _instance;
        static MT::CMutex _datalock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSYSCONSOLE_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-05-2005 :
        - Applied file layout standard

--------------------------------------------------------------------------*/

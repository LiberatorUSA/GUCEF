/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_CORE_CSYSCONSOLECLIENT_H
#define GUCEF_CORE_CSYSCONSOLECLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"               /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

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
class GUCEFCORE_EXPORT_CPP CSysConsoleClient
{
    protected:
    
    typedef std::vector< CString > TStringVector;
    
    CSysConsoleClient( void );
    
    CSysConsoleClient( const CSysConsoleClient& src );
                  
    virtual ~CSysConsoleClient();
    
    CSysConsoleClient& operator=( const CSysConsoleClient& src );
    
    bool RegisterCmd( const CString& path                ,
                      const CString& command             ,
                      const TStringVector& arglist       ,
                      CISysConsoleCmdHandler* cmdhandler );                        

    void UnregisterCmd( const CString& path    ,
                        const CString& command );
    
    void LeaveDir( void );
    
    bool EnterDir( const CString& dirname );
    
    bool JumpTo( const CString& path );
    
    const CString& GetPath( void ) const;
    
    bool Execute( const CString& funcname      ,
                  const TStringVector& arglist ,
                  TStringVector& resultdata    );
    
    TStringVector GetDirList( void ) const;
    
    TStringVector GetCmdList( void ) const;
    
    virtual void OnReturnData( const TStringVector& data );
    
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

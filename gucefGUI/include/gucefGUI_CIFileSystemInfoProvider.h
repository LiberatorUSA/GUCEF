/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H
#define GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

#ifndef GUCEF_GUI_MACROS_H
#include "gucefGUI_macros.h"
#define GUCEF_GUI_MACROS_H
#endif /* GUCEF_GUI_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Interface that allows GUI widgets to obtain file system information.
 */
class GUCEF_GUI_PUBLIC_CPP CIFileSystemInfoProvider
{
    public:
    
    typedef std::set< CString > TStringSet;
    
    CIFileSystemInfoProvider( void );
    
    virtual ~CIFileSystemInfoProvider();
    
    virtual bool GetDirectoryList( const CString& path   ,
                                   const CString& filter ,
                                   TStringSet& list      ) = 0;

    virtual bool GetFileList( const CString& path   ,
                              const CString& filter ,
                              TStringSet& list      ) = 0;

    virtual bool GetArchiveList( const CString& path   ,
                                 const CString& filter ,
                                 TStringSet& list      ) = 0;
                                                             
    private:
    
    CIFileSystemInfoProvider( const CIFileSystemInfoProvider& src );
    CIFileSystemInfoProvider& operator=( const CIFileSystemInfoProvider& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/

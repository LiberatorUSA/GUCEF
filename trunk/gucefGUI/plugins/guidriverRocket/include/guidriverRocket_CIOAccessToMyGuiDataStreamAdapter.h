/*
 *  guceMyGUIOgre: glue module for the MyGUI+Ogre GUI backend
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCE_MYGUIOGRE_CIOACCESSTOMYGUIDATASTREAMADAPTER_H
#define GUCE_MYGUIOGRE_CIOACCESSTOMYGUIDATASTREAMADAPTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef __MYGUI_H__
#include "MyGUI.h"
#define __MYGUI_H__
#endif /* __MYGUI_H__ ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCE_MYGUIOGRE_MACROS_H
#include "guceMyGUIOgre_macros.h"      /* often used macros */
#define GUCE_MYGUIOGRE_MACROS_H
#endif /* GUCE_MYGUIOGRE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCE {
namespace MYGUIOGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/


class CIOAccessToMyGUIDataStreamAdapter : public MyGUI::IDataStream
{
    public:
    
    CIOAccessToMyGUIDataStreamAdapter( GUCEF::CORE::CIOAccess& ioAccess );
    
    virtual ~CIOAccessToMyGUIDataStreamAdapter();

    virtual bool eof();
    
    virtual std::size_t size();
    
    virtual void readline( std::string& _source, MyGUI::Char _delim = '\n' );
    
    virtual std::size_t read( void* _buf, std::size_t _count );
    
    private:
    
    CIOAccessToMyGUIDataStreamAdapter( const CIOAccessToMyGUIDataStreamAdapter& src );
    CIOAccessToMyGUIDataStreamAdapter& operator=( const CIOAccessToMyGUIDataStreamAdapter& src );
    
    private:
    
    GUCEF::CORE::CIOAccess* m_ioAccess;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIOGRE */
}; /* namespace GUCE */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCE_MYGUIOGRE_CIOACCESSTOMYGUIDATASTREAMADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/

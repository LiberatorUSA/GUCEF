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

#ifndef GUCEF_GUI_CTEXTBOX_H
#define GUCEF_GUI_CTEXTBOX_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_CWIDGET_H
#include "gucefGUI_CWidget.h"
#define GUCEF_GUI_CWIDGET_H
#endif /* GUCEF_GUI_CWIDGET_H ? */

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

class GUCEF_GUI_PUBLIC_CPP CTextbox : public CWidget
{
    public:
        
    static const CORE::CEvent SelectionChangedEvent;
    static const CORE::CEvent TextChangedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    typedef std::vector< CString > TStringVector;

    CTextbox( void );
    
    virtual ~CTextbox();
    
    virtual const CString& GetClassTypeName( void ) const;
    
    virtual UInt32 GetLineCount( void );
    
    virtual bool Clear( void );
    
    virtual bool GetLineAtIndex( const UInt32 lineIndex ,
                                 CString& lineText      ) const;
    
    virtual bool InsertLineAtIndex( const UInt32 lineIndex ,
                                    const CString& text    );
    
    virtual bool AppendLine( const CString& text );
    
    virtual CString GetTextAsString( void ) const;
    
    virtual TStringVector GetText( void ) const;
    
    virtual CString GetSelectedText( void ) const;
    
    private:
    
    CTextbox( const CTextbox& src );
    CTextbox& operator=( const CTextbox& src );    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUI_CTEXTBOX_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 10-02-2008 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/

/*
 *  guceGUI: GUCE module providing GUI functionality
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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "gucefGUI_CFileSystemDialog.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const GUCEF::CORE::CEvent CFileSystemDialog::OkPressedEvent = "GUCE::GUI::CFileSystemDialog::OkPressedEvent";
const GUCEF::CORE::CEvent CFileSystemDialog::CancelPressedEvent = "GUCE::GUI::CFileSystemDialog::CancelPressedEvent";
const GUCEF::CORE::CEvent CFileSystemDialog::FilterChangedEvent = "GUCE::GUI::CFileSystemDialog::FilterChangedEvent";
const GUCEF::CORE::CEvent CFileSystemDialog::SelectionChangedEvent = "GUCE::GUI::CFileSystemDialog::SelectionChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CFileSystemDialog::CFileSystemDialog( void )
    : CFormEx()                  ,
      m_window( NULL )           , 
      m_okButton( NULL )         ,
      m_cancelButton( NULL )     ,
      m_fsGridView( NULL )       ,
      m_filterCombobox( NULL )   ,
      m_selectionEditbox( NULL )
{GUCEF_TRACE;

    RegisterEvents();

}

/*-------------------------------------------------------------------------*/
    
CFileSystemDialog::~CFileSystemDialog()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CFileSystemDialog::SetFileSystemInfoProvider( CIFileSystemInfoProvider* provider )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CFileSystemDialog::SetCurrentPath( const CString& path )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
const CString&
CFileSystemDialog::GetCurrentPath( void ) const
{GUCEF_TRACE;

    static CString dummyStr;
    return dummyStr;
}

/*-------------------------------------------------------------------------*/

void
CFileSystemDialog::RegisterEvents( void )
{GUCEF_TRACE;

    OkPressedEvent.Initialize();
    CancelPressedEvent.Initialize();
    FilterChangedEvent.Initialize();
    SelectionChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CCombobox*
CFileSystemDialog::GetFilterCombobox( void )
{GUCEF_TRACE;

    return m_filterCombobox;
}

/*-------------------------------------------------------------------------*/

CEditbox*
CFileSystemDialog::GetSelectionEditbox( void )
{GUCEF_TRACE;

    return m_selectionEditbox;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemDialog::IsItemADirectory( const CString& name ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CFileSystemDialog::SetAllowedSelection( const bool allowDirSelect  ,
                                        const bool allowFileSelect )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CFileSystemDialog::ShowModal( void )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
CWindow*
CFileSystemDialog::GetWindow( void )
{GUCEF_TRACE;

    return m_window;
}

/*-------------------------------------------------------------------------*/
    
CButton*
CFileSystemDialog::GetOkButton( void )
{GUCEF_TRACE;

    return m_okButton;
}

/*-------------------------------------------------------------------------*/
    
CButton*
CFileSystemDialog::GetCancelButton( void )
{GUCEF_TRACE;

    return m_cancelButton;
}

/*-------------------------------------------------------------------------*/

CGridView*
CFileSystemDialog::GetFileSystemGridView( void )
{GUCEF_TRACE;

    return m_fsGridView;
}
   
/*-------------------------------------------------------------------------*/
    
void
CFileSystemDialog::RefreshView( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
void
CFileSystemDialog::SetAllowMultiSelect( const bool allow )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CFileSystemDialog::GetAllowMultiSelect( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
void
CFileSystemDialog::SetSelectedItems( const TStringVector& items )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
const CFileSystemDialog::TStringVector&
CFileSystemDialog::GetSelectedItems( void ) const
{GUCEF_TRACE;

    static TStringVector dummyList;
    return dummyList;
}

/*-------------------------------------------------------------------------*/
    
void
CFileSystemDialog::SetItemDisplayTypes( const bool showDirs  ,
                                        const bool showFiles )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CFileSystemDialog::OnPreLayoutLoad( void )
{GUCEF_TRACE;

    CFormEx::OnPreLayoutLoad();
    
    m_okButton = NULL;
    m_cancelButton = NULL;
    m_window = NULL;
    m_fsGridView = NULL;
    m_filterCombobox = NULL;
    m_selectionEditbox = NULL;
}

/*-------------------------------------------------------------------------*/

void
CFileSystemDialog::OnPostLayoutLoad( void )
{GUCEF_TRACE;

    CFormEx::OnPostLayoutLoad();
    
    m_okButton = static_cast< CButton* >( GetWidget( "OkButton" ) );
    m_cancelButton = static_cast< CButton* >( GetWidget( "CancelButton" ) );
    m_window = static_cast< CWindow* >( GetWidget( "FileSystemDialogWindow" ) );
    m_fsGridView = static_cast< CGridView* >( GetWidget( "FileSystemGridView" ) );
    m_filterCombobox = static_cast< CCombobox* >( GetWidget( "FilterCombobox" ) );
    m_selectionEditbox = static_cast< CEditbox* >( GetWidget( "SelectionEditbox" ) ); 
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUI */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

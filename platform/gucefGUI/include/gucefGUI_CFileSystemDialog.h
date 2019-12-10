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

#ifndef GUCEF_GUI_CFILESYSTEMDIALOG_H
#define GUCEF_GUI_CFILESYSTEMDIALOG_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H
#include "gucefGUI_CIFileSystemInfoProvider.h"
#define GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H
#endif /* GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H ? */

#ifndef GUCEF_GUI_CWINDOW_H
#include "gucefGUI_CWindow.h"
#define GUCEF_GUI_CWINDOW_H
#endif /* GUCEF_GUI_CWINDOW_H ? */

#ifndef GUCEF_GUI_CBUTTON_H
#include "gucefGUI_CButton.h"
#define GUCEF_GUI_CBUTTON_H
#endif /* GUCEF_GUI_CBUTTON_H ? */

#ifndef GUCEF_GUI_CCOMBOBOX_H
#include "gucefGUI_CCombobox.h"
#define GUCEF_GUI_CCOMBOBOX_H
#endif /* GUCEF_GUI_CCOMBOBOX_H ? */

#ifndef GUCEF_GUI_CEDITBOX_H
#include "gucefGUI_CEditbox.h"
#define GUCEF_GUI_CEDITBOX_H
#endif /* GUCEF_GUI_CEDITBOX_H ? */

#ifndef GUCEF_GUI_CGRIDVIEW_H
#include "gucefGUI_CGridView.h"
#define GUCEF_GUI_CGRIDVIEW_H
#endif /* GUCEF_GUI_CGRIDVIEW_H ? */

#ifndef GUCEF_GUI_CFORMEX_H
#include "gucefGUI_CFormEx.h"
#define GUCEF_GUI_CFORMEX_H
#endif /* GUCEF_GUI_CFORMEX_H ? */

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

class GUCEF_GUI_PUBLIC_CPP CFileSystemDialog : public CFormEx
{
    public:
    
    static const CORE::CEvent OkPressedEvent;
    static const CORE::CEvent CancelPressedEvent;
    static const CORE::CEvent FilterChangedEvent;
    static const CORE::CEvent SelectionChangedEvent;
    
    static void RegisterEvents( void );    

    public:
    
    typedef std::vector< CString >  TStringVector;
    
    CFileSystemDialog( void );
    
    virtual ~CFileSystemDialog();
    
    virtual bool ShowModal( void );
    
    CWindow* GetWindow( void );
    
    CButton* GetOkButton( void );
    
    CButton* GetCancelButton( void );
    
    CGridView* GetFileSystemGridView( void );
    
    CCombobox* GetFilterCombobox( void );
    
    CEditbox* GetSelectionEditbox( void );
    
    virtual void RefreshView( void );
    
    virtual void SetAllowMultiSelect( const bool allow );
    
    virtual bool GetAllowMultiSelect( void ) const;
    
    virtual void SetSelectedItems( const TStringVector& item );
    
    virtual const TStringVector& GetSelectedItems( void ) const;
    
    virtual void SetItemDisplayTypes( const bool showDirs  ,
                                      const bool showFiles );

    virtual void SetAllowedSelection( const bool allowDirSelect  ,
                                      const bool allowFileSelect );

    virtual void SetFileSystemInfoProvider( CIFileSystemInfoProvider* provider );

    virtual void SetCurrentPath( const CString& path );
    
    virtual const CString& GetCurrentPath( void ) const;
    
    virtual bool IsItemADirectory( const CString& name ) const;
        
    protected:
    
    virtual void OnPreLayoutLoad( void );
    
    virtual void OnPostLayoutLoad( void );
    
    private:
    
    CFileSystemDialog( const CFileSystemDialog& src );
    CFileSystemDialog& operator=( const CFileSystemDialog& src );
    
    private:
    
    typedef std::set< CString > TStringSet;
    
    CWindow* m_window;
    CButton* m_okButton;
    CButton* m_cancelButton;
    CGridView* m_fsGridView;
    CCombobox* m_filterCombobox;
    CEditbox* m_selectionEditbox;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUI */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CFILESYSTEMDIALOG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 05-09-2007 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/

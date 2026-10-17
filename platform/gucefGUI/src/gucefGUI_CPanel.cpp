/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *
 *  Copyright (C) 1998 - 2026.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_CGUICONTEXT_H
#include "gucefGUI_CGUIContext.h"
#define GUCEF_GUI_CGUICONTEXT_H
#endif /* GUCEF_GUI_CGUICONTEXT_H ? */

#include "gucefGUI_CPanel.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPanel::CPanel( void )
    : CWidget( "Panel" )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

CPanel::CPanel( const CString& widgetTypeName )
    : CWidget( widgetTypeName )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

CPanel::~CPanel()
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

CWidget*
CPanel::Create( void )
{GUCEF_TRACE;

    return GUCEF_NEW CPanel();
}

/*-------------------------------------------------------------------------*/

bool
CPanel::GetNaturalSize( Int32& width, Int32& height ) const
{GUCEF_TRACE;

    width = height = 0;
    CGUIContext* context = GetContext();
    if ( GUCEF_NULL == context )
        return false;

    const CString& type = GetType();
    if ( type == "HBox" || type == "VBox" )
    {
        Int32 padding = GetIntProperty( "Padding" );
        Int32 spacing = GetIntProperty( "Spacing" );
        bool horizontal = type == "HBox";
        Int32 count = 0;
        const TWidgetVector& children = GetChildren();
        for ( UInt32 i=0; i<(UInt32)children.size(); ++i )
        {
            if ( !children[ i ]->IsVisible() )
                continue;
            CUVector size;
            children[ i ]->GetSize( size );
            Int32 w = size.GetX().GetPixelValue();
            Int32 h = size.GetY().GetPixelValue();
            if ( horizontal )
            {
                width += w;
                if ( h > height ) height = h;
            }
            else
            {
                height += h;
                if ( w > width ) width = w;
            }
            ++count;
        }
        if ( count > 1 )
        {
            if ( horizontal ) width += ( count - 1 ) * spacing;
            else height += ( count - 1 ) * spacing;
        }
        width += 2 * padding;
        height += 2 * padding;
        return true;
    }

    CString image = GetProperty( "BackgroundImage" );
    IMAGE::TPixelMapPtr pixels;
    UInt32 revision = 0;
    if ( !image.IsNULLOrEmpty() && context->ResolveImage( image, pixels, revision ) )
    {
        width = (Int32) pixels->GetWidthInPixels();
        height = (Int32) pixels->GetHeightInPixels();
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const CString&
CPanel::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::GUI::CPanel";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

#include "gucefGUI_CVideoSettings.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
#include <windows.h>
#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {
         
/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CVideoSettings::CVideoSettings( void )
    : CIConfigurable()          ,
      m_isFullscreen( false )   ,
      m_hasVSync( false )       ,
      m_antiAliasingFactor( 0 ) ,
      m_displayMode()
{GUCEF_TRACE;

    m_displayMode.depthInBits = 32;
    m_displayMode.frequency = 60;
    m_displayMode.widthInPixels = 800;
    m_displayMode.heightInPixels = 600;
}

/*-------------------------------------------------------------------------*/
    
CVideoSettings::CVideoSettings( const CVideoSettings& src )
    : CIConfigurable()                                 ,
      m_isFullscreen( src.m_isFullscreen )             ,
      m_hasVSync( src.m_hasVSync )                     ,
      m_antiAliasingFactor( src.m_antiAliasingFactor ) ,
      m_displayMode( src.m_displayMode )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CVideoSettings::~CVideoSettings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CVideoSettings&
CVideoSettings::operator=( const CVideoSettings& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_isFullscreen = src.m_isFullscreen;
        m_hasVSync = src.m_hasVSync;
        m_antiAliasingFactor = src.m_antiAliasingFactor;
        m_displayMode = src.m_displayMode;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
void
CVideoSettings::SetFullscreenState( const bool isFullscreen )
{GUCEF_TRACE;

    m_isFullscreen = isFullscreen;
}

/*-------------------------------------------------------------------------*/
    
bool
CVideoSettings::GetFullscreenState( void ) const
{GUCEF_TRACE;

    return m_isFullscreen;
}

/*-------------------------------------------------------------------------*/

void
CVideoSettings::SetVSyncState( const bool hasVSync )
{GUCEF_TRACE;

    m_hasVSync = hasVSync;
}

/*-------------------------------------------------------------------------*/
    
bool
CVideoSettings::GetVSyncState( void ) const
{GUCEF_TRACE;

    return m_hasVSync;
}

/*-------------------------------------------------------------------------*/

void
CVideoSettings::SetResolutionWidthInPixels( const UInt32 widthInPixels )
{GUCEF_TRACE;

    m_displayMode.widthInPixels = widthInPixels;
}

/*-------------------------------------------------------------------------*/

UInt32
CVideoSettings::GetResolutionWidthInPixels( void ) const
{GUCEF_TRACE;

    return m_displayMode.widthInPixels;
}

/*-------------------------------------------------------------------------*/

void
CVideoSettings::SetResolutionHeightInPixels( const UInt32 heightInPixels )
{GUCEF_TRACE;

    m_displayMode.heightInPixels = heightInPixels;
}

/*-------------------------------------------------------------------------*/

UInt32
CVideoSettings::GetResolutionHeightInPixels( void ) const
{GUCEF_TRACE;

    return m_displayMode.heightInPixels;
}

/*-------------------------------------------------------------------------*/

void
CVideoSettings::SetResolutionDepthInBits( const UInt32 depthInBits )
{GUCEF_TRACE;

    m_displayMode.depthInBits = depthInBits;
}

/*-------------------------------------------------------------------------*/

UInt32
CVideoSettings::GetResolutionDepthInBits( void ) const
{GUCEF_TRACE;

    return m_displayMode.depthInBits;
}

/*-------------------------------------------------------------------------*/
    
void
CVideoSettings::SetResolution( const UInt32 widthInPixels  ,
                               const UInt32 heightInPixels ,
                               const UInt32 depthInPixels  )
{GUCEF_TRACE;

    m_displayMode.widthInPixels = widthInPixels;
    m_displayMode.heightInPixels = heightInPixels;
    m_displayMode.depthInBits = depthInPixels;
}

/*-------------------------------------------------------------------------*/

void
CVideoSettings::GetResolution( UInt32& widthInPixels  ,
                               UInt32& heightInPixels ,
                               UInt32& depthInPixels  )
{GUCEF_TRACE;

    widthInPixels = m_displayMode.widthInPixels;
    heightInPixels = m_displayMode.heightInPixels;
    depthInPixels = m_displayMode.depthInBits;
}

/*-------------------------------------------------------------------------*/

void
CVideoSettings::SetAntiAliasingFactor( const UInt32 newFactor )
{GUCEF_TRACE;

    m_antiAliasingFactor = newFactor;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CVideoSettings::GetAntiAliasingFactor( void ) const
{GUCEF_TRACE;

    return m_antiAliasingFactor;
}

/*-------------------------------------------------------------------------*/

void
CVideoSettings::SetFrequency( const UInt32 frequency )
{GUCEF_TRACE;

    m_displayMode.frequency = frequency;
}

/*-------------------------------------------------------------------------*/

UInt32
CVideoSettings::GetFrequency( void ) const
{GUCEF_TRACE;

    return m_displayMode.frequency;
}

/*-------------------------------------------------------------------------*/

bool
CVideoSettings::SaveConfig( GUCEF::CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
                                                                   
bool
CVideoSettings::LoadConfig( const GUCEF::CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CVideoSettings::SetDisplayMode( const TDisplayMode& displayMode )
{GUCEF_TRACE;

    m_displayMode = displayMode;
}

/*-------------------------------------------------------------------------*/
    
void
CVideoSettings::GetDisplayMode( TDisplayMode& displayMode ) const
{GUCEF_TRACE;

    displayMode = m_displayMode;
}

/*-------------------------------------------------------------------------*/

bool
CVideoSettings::RetrieveSettingsFromOS( CVideoSettings& settings )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    DEVMODE displayMode;
    memset( &displayMode, 0, sizeof( DEVMODE ) );
    displayMode.dmSize = sizeof( DEVMODE );
    TDisplayMode currentMode;
    
    if ( TRUE == EnumDisplaySettings( NULL                  ,
                                      ENUM_CURRENT_SETTINGS ,
                                      &displayMode          ) )
    {
        currentMode.widthInPixels = displayMode.dmPelsWidth; 
        currentMode.heightInPixels = displayMode.dmPelsHeight;
        currentMode.depthInBits = displayMode.dmBitsPerPel;
        currentMode.frequency = displayMode.dmDisplayFrequency;
        settings.SetDisplayMode( currentMode );
        
        return true;
    }     
    return false;
    
    #else
    
    return false;
    
    #endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

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
 
#ifndef GUCEF_GUI_CVIDEOSETTINGS_H
#define GUCEF_GUI_CVIDEOSETTINGS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_GUI_CVIDEOOPTIONS_H
#include "gucefGUI_CVideoOptions.h"
#define GUCEF_GUI_CVIDEOOPTIONS_H
#endif /* GUCEF_GUI_CVIDEOOPTIONS_H ? */

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

class GUCEF_GUI_PUBLIC_CPP CVideoSettings : public CORE::CIConfigurable
{
    public:
    
    typedef CVideoOptions::TDisplayMode TDisplayMode;
    
    CVideoSettings( void );
    
    CVideoSettings( const CVideoSettings& src );
    
    virtual ~CVideoSettings();
    
    CVideoSettings& operator=( const CVideoSettings& src );
    
    void SetDisplayMode( const TDisplayMode& displayMode );
    
    void GetDisplayMode( TDisplayMode& displayMode ) const;
    
    void SetFullscreenState( const bool isFullscreen );
    
    bool GetFullscreenState( void ) const;

    void SetVSyncState( const bool hasVSync );
    
    bool GetVSyncState( void ) const;
    
    void SetResolutionWidthInPixels( const UInt32 widthInPixels );
    
    UInt32 GetResolutionWidthInPixels( void ) const;
    
    void SetResolutionHeightInPixels( const UInt32 heightInPixels );
    
    UInt32 GetResolutionHeightInPixels( void ) const;
    
    void SetResolutionDepthInBits( const UInt32 depthInBits );
    
    UInt32 GetResolutionDepthInBits( void ) const;
    
    void SetResolution( const UInt32 widthInPixels  ,
                        const UInt32 heightInPixels ,
                        const UInt32 depthInBits    );

    void GetResolution( UInt32& widthInPixels  ,
                        UInt32& heightInPixels ,
                        UInt32& depthInBits    );
                        
    void SetAntiAliasingFactor( const UInt32 newFactor );
    
    UInt32 GetAntiAliasingFactor( void ) const;
    
    void SetFrequency( const UInt32 frequency );
    
    UInt32 GetFrequency( void ) const;

    virtual bool SaveConfig( GUCEF::CORE::CDataNode& tree );
                                                                   
    virtual bool LoadConfig( const GUCEF::CORE::CDataNode& treeroot );
    
    static bool RetrieveSettingsFromOS( CVideoSettings& settings );
        
    private:
    
    bool m_isFullscreen;
    bool m_hasVSync;
    UInt32 m_antiAliasingFactor;
    TDisplayMode m_displayMode;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CVIDEOSETTINGS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-04-2005 :
       - Initial version of this file.

---------------------------------------------------------------------------*/

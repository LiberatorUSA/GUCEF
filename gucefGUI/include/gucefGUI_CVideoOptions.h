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
 
#ifndef GUCEF_GUI_CVIDEOOPTIONS_H
#define GUCEF_GUI_CVIDEOOPTIONS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <vector>

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

class CVideoSettings;

/*-------------------------------------------------------------------------*/

class GUCEF_GUI_PUBLIC_CPP CVideoOptions
{
    public:
    
    struct SDisplayMode
    {
        UInt32 widthInPixels;
        UInt32 heightInPixels;
        UInt32 depthInBits;
        UInt32 frequency;     /**< display frequency in frames per second */
    };
    typedef struct SDisplayMode TDisplayMode;
    typedef std::vector< TDisplayMode > TDisplayModeVector;
    
    CVideoOptions( void );
    
    CVideoOptions( const CVideoOptions& src );
    
    virtual ~CVideoOptions();
    
    CVideoOptions& operator=( const CVideoOptions& src );
    
    /**
     *  Checks the given settings against the options to see if
     *  it is a possible option.
     */
    bool AreSettingsPossible( const CVideoSettings& settings ) const;
    
    const TDisplayModeVector& GetDisplayModes( void ) const;
    
    void GetDisplayModesForResolution( const UInt32 widthInPixels  , 
                                       const UInt32 heightInPixels ,
                                       TDisplayModeVector& modes   ) const;
    
    void Clear( void );
    
    static bool RetrieveOptionsFromOS( CVideoOptions& options );
        
    private:

    TDisplayModeVector m_displayModes;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CVIDEOOPTIONS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-04-2005 :
       - Initial version of this file.

---------------------------------------------------------------------------*/

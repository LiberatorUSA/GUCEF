/*
 *  gucefIMAGE: GUCEF module providing image utilities
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

#ifndef GUCEF_IMAGE_CIMAGEGLOBAL_H
#define GUCEF_IMAGE_CIMAGEGLOBAL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_IMAGE_MACROS_H
#include "gucefIMAGE_macros.h"
#define GUCEF_IMAGE_MACROS_H
#endif /* GUCEF_IMAGE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CImageCodecPluginManager;
class CImageCodecRegistry;

/*-------------------------------------------------------------------------*/

/**
 *  Singular singleton providing access to all global Image systems
 */
class GUCEF_IMAGE_EXPORT_CPP CImageGlobal
{
    public:

    static CImageGlobal* Instance( void );

    CImageCodecPluginManager& GetImageCodecPluginManager( void );

    CImageCodecRegistry& GetImageCodecRegistry( void );

    private:
    friend class CModule;

    static void Deinstance( void );

    private:

    CImageGlobal( void );

    ~CImageGlobal();

    void Initialize( void );

    private:

    CImageCodecPluginManager* m_imageCodecPluginManager;
    CImageCodecRegistry* m_imageCodecRegistry;
    
    static CImageGlobal* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CIMAGEGLOBAL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/

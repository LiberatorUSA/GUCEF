/*
 *  guidriverCEGUI: glue module for the CEGUI GUI backend
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

#ifndef GUCEF_GUIDRIVERCEGUI_VFSRESOURCEPROVIDER_H
#define GUCEF_GUIDRIVERCEGUI_VFSRESOURCEPROVIDER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef _CEGUIBase_h_
#include "CEGUI/Base.h"
#define _CEGUIBase_h_
#endif /* _CEGUIBase_h_ ? */

#ifndef _CEGUIResourceProvider_h_
#include "CEGUI/ResourceProvider.h"
#define _CEGUIResourceProvider_h_
#endif /* _CEGUIResourceProvider_h_ ? */

#ifndef GUCEF_GUIDRIVERCEGUI_MACROS_H
#include "guidriverCEGUI_macros.h" 
#define GUCEF_GUIDRIVERCEGUI_MACROS_H
#endif /* GUCEF_GUIDRIVERCEGUI_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERCEGUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_GUIDRIVERCEGUI_EXPORT_CPP VfsResourceProvider : public ::CEGUI::ResourceProvider
{
    public :

    VfsResourceProvider();

    virtual ~VfsResourceProvider();

    /*!
    \brief
        Set the directory associated with a given resource group identifier.

    \param resourceGroup
        The resource group identifier whose directory is to be set.

    \param directory
        The directory to be associated with resource group identifier
        \a resourceGroup

    \return
        Nothing.
    */
    virtual void setResourceGroupDirectory( const CEGUI::String& resourceGroup, const CEGUI::String& directory );

    /*!
    \brief
        Return the directory associated with the specified resource group
        identifier.

    \param resourceGroup
        The resource group identifier for which the associated directory is to
        be returned.

    \return
        String object describing the directory currently associated with resource
        group identifier \a resourceGroup.

    \note
        This member is not defined as being const because it may cause
        creation of an 'empty' directory specification for the resourceGroup
        if the resourceGroup has not previously been accessed.
    */
    virtual const CEGUI::String& getResourceGroupDirectory( const CEGUI::String& resourceGroup );

    /*!
    \brief
        clears any currently set directory for the specified resource group
        identifier.

    \param resourceGroup
        The resource group identifier for which the associated directory is to
        be cleared.
    */
    virtual void clearResourceGroupDirectory( const CEGUI::String& resourceGroup );

    virtual void loadRawDataContainer( const CEGUI::String& filename, CEGUI::RawDataContainer& output, const CEGUI::String& resourceGroup );

    virtual void unloadRawDataContainer( CEGUI::RawDataContainer& data );

    virtual size_t getResourceGroupFileNames( std::vector<CEGUI::String>& out_vec ,
                                              const CEGUI::String& file_pattern   ,
                                              const CEGUI::String& resource_group );

    void setDefaultResourceGroup( const CEGUI::String& resourceGroup );

    private:

    typedef std::map< CEGUI::String, CEGUI::String >    TCEStringMap;

    TCEStringMap m_groupMap;
    CEGUI::String m_defaultResourceGroup;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERCEGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUIDRIVERCEGUI_VFSRESOURCEPROVIDER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2010 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
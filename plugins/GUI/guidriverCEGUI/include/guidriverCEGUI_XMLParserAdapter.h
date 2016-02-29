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

#ifndef GUCEF_GUIDRIVERCEGUI_XMLPARSERADAPTER_H
#define GUCEF_GUIDRIVERCEGUI_XMLPARSERADAPTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef _CEGUIXMLParser_h_
#include "CEGUI/XMLParser.h"
#define _CEGUIXMLParser_h_
#endif /* _CEGUIXMLParser_h_ ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

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

class GUCEF_GUIDRIVERCEGUI_EXPORT_CPP XMLParserAdapter : public ::CEGUI::XMLParser
{
    public:
    
    XMLParserAdapter( void );

    ~XMLParserAdapter();

    // Implementation of public abstract interface
    virtual void parseXML( ::CEGUI::XMLHandler& handler              , 
                           const ::CEGUI::RawDataContainer& filename , 
                           const ::CEGUI::String& schemaName         );

    void parseDataNodes( ::CEGUI::XMLHandler& handler     ,
                         const CORE::CDataNode& dataNodes );

    protected:
    
    // Implementation of abstract interface.
    virtual bool initialiseImpl( void );
    virtual void cleanupImpl( void );

    private:

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr m_codec;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERCEGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUIDRIVERCEGUI_XMLPARSERADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2010 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/

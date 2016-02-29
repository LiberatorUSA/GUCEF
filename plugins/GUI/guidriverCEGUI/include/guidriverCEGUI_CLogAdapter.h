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

#ifndef GUCEF_GUIDRIVERCEGUI_CLOGADAPTER_H
#define GUCEF_GUIDRIVERCEGUI_CLOGADAPTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef _CEGUILogger_h_
#include "CEGUI/Logger.h"         /* CEGUI base class for it's logging functionality */
#define _CEGUILogger_h_
#endif /* _CEGUILogger_h_ ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

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

/**
 *  Adapter class that redirects all CEGUI logging to the GUCEF log manager
 */ 
class GUCEF_GUIDRIVERCEGUI_EXPORT_CPP CLogAdapter : public ::CEGUI::Logger
{
    public:
    
    /*!
    \brief
        Add an event to the log.

    \param message
        String object containing the message to be added to the event log.

    \param level
        LoggingLevel for this message.  If \a level is greater than the current set logging level, the message is not logged.

    \return
        Nothing
    */
    virtual void logEvent( const CEGUI::String& message                , 
                           CEGUI::LoggingLevel level = CEGUI::Standard );

    /*!
    \brief
        The Log output is determined in GUCEF and this function is a non-operation

    \param filename
        Name of the file to put log messages.

    \param append
        - true if events should be added to the end of the current file.
        - false if the current contents of the file should be discarded.
     */
    virtual void setLogFilename( const CEGUI::String& filename , 
                                 bool append = false           );
   

    CLogAdapter( void );
    virtual ~CLogAdapter();

    private:

    CLogAdapter( const CLogAdapter& src );
    CLogAdapter& operator=( const CLogAdapter& src );    
    
    private:
    
    GUCEF::CORE::CLogManager* m_logManager;
    static CLogAdapter* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERCEGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUIDRIVERCEGUI_CLOGADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/

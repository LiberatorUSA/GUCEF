/*
 *  gucefPATCHER: GUCEF RAD module providing a patch delivery system
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
#define GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#include "gucefPATCHER_macros.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEFPATCHER_EXPORT_CPP CPatchSetFileEngineEvents
{
    public:
    
    static const CORE::CEvent FileListProcessingStartedEvent;
    static const CORE::CEvent LocalFileIsOKEvent;
    static const CORE::CEvent LocalFileSizeMismatchEvent;
    static const CORE::CEvent LocalFileHashMismatchEvent;
    static const CORE::CEvent LocalFileNotFoundEvent;
    static const CORE::CEvent LocalFileReplacedEvent;
    static const CORE::CEvent FileRetrievalStartedEvent;
    static const CORE::CEvent FileRetrievalCompleteEvent;
    static const CORE::CEvent FileRetrievalErrorEvent;
    static const CORE::CEvent FileStorageErrorEvent;
    static const CORE::CEvent FileListProcessingCompleteEvent;
    static const CORE::CEvent FileListProcessingAbortedEvent;
    
    static void RegisterEvents( void );
    
    protected:
    
    CPatchSetFileEngineEvents( void );
    virtual ~CPatchSetFileEngineEvents();
    
    private:
    CPatchSetFileEngineEvents( const CPatchSetFileEngineEvents& src );            /**< not implemented */
    CPatchSetFileEngineEvents& operator=( const CPatchSetFileEngineEvents& src ); /**< not implemented */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/
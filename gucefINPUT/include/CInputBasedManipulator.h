/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef CINPUTBASEDMANIPULATOR_H
#define CINPUTBASEDMANIPULATOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDVSTRING_H
#include "CDVString.h"                /* platform string implementation */
#define CDVSTRING_H
#endif /* CDVSTRING_H ? */

#ifndef CIINPUTACTIONHANDLER_H
#include "CIInputActionHandler.h"     /* interface class for action mapped input handlers */
#define CIINPUTACTIONHANDLER_H
#endif /* CIINPUTACTIONHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Abstract base class for input based manipulators.
 *      instances automaticly register with the profile manager
 */
class EXPORT_CPP CInputBasedManipulator : public CIInputActionHandler
{
        public:
        
        CInputBasedManipulator( const CORE::CString& name );
        
        virtual ~CInputBasedManipulator();
        
        const CORE::CString& GetName( void ) const;
        
        void SetInputEnabled( const bool enabled );
        
        /**
         *      implements IsInputEnabled() of CIInputActionHandler
         *      returns whether this manipulator is active and ie
         *      is recieving/processing input events.
         */
        virtual bool IsInputEnabled( void ) const;
        
        void SetHandlingOrder( const UInt32 handlingOrder );
        
        UInt32 GetHandlingOrder( void ) const;
        
        private:
        
        CInputBasedManipulator( void );
        CInputBasedManipulator( const CInputBasedManipulator& src );
        CInputBasedManipulator& operator=( const CInputBasedManipulator& src );
        
        private:
        
        
        CORE::CString m_name;
        bool m_enabled;
        UInt32 m_handlingOrder;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CINPUTBASEDMANIPULATOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 13-09-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/

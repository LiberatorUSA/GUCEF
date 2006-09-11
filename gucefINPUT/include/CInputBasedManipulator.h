/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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

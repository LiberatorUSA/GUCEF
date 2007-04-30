/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef CACTIVECOMPUMP_H
#define CACTIVECOMPUMP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCORE_H
#include "gucefCORE.h"                /* gucefCORE library API */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFMT_H
#include "gucefMT.h"                  /* gucefMT library API */
#define GUCEFMT_H
#endif /* GUCEFMT_H ? */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* build switches and macros */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CActiveComPump : public MT::CActiveObject
{
        public:                
        
        virtual ~CActiveComPump();

        protected:
        
        virtual bool OnTaskStart( void* taskdata );
        
        virtual bool OnTaskCycle( void* taskdata );
        
        virtual void OnTaskEnd( void* taskdata );
        
        private:
        friend class CCom;
        
        CActiveComPump( void );
        
        private:        
        CActiveComPump( const CActiveComPump& src );
        CActiveComPump& operator=( const CActiveComPump& src );
        
        MT::CMutex* _datalock;
        CORE::CDynamicArray* _sockets;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CACTIVECOMPUMP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-02-2005 :
        - Initial version

-----------------------------------------------------------------------------*/

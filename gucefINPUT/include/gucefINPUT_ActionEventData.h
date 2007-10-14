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

#ifndef GUCEF_INPUT_ACTIONEVENTDATA_H
#define GUCEF_INPUT_ACTIONEVENTDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H
#include "gucefINPUT_CMouseButtonEventData.h"
#define GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H
#endif /* GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H ? */

#ifndef GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H
#include "gucefINPUT_CMouseMovedEventData.h"
#define GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H
#endif /* GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H ? */

#ifndef GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H
#include "gucefINPUT_CKeyStateChangedEventData.h"
#define GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H
#endif /* GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H ? */

#ifndef GUCEF_INPUT_CKEYMODSTATECHANGEDEVENTDATA_H
#include "gucefINPUT_CKeyModStateChangedEventData.h"
#define GUCEF_INPUT_CKEYMODSTATECHANGEDEVENTDATA_H
#endif /* GUCEF_INPUT_CKEYMODSTATECHANGEDEVENTDATA_H ? */

#ifndef GUCEF_INPUT_CTCONCRETEACTIONEVENTDATA_H
#include "gucefINPUT_CTConcreteActionEventData.h"
#define GUCEF_INPUT_CTCONCRETEACTIONEVENTDATA_H
#endif /* GUCEF_INPUT_CTCONCRETEACTIONEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CTConcreteActionEventData< CMouseButtonEventData >          TMouseButtonActionEventData;
typedef CTConcreteActionEventData< CMouseMovedEventData >           TMouseMovedActionEventData;
typedef CTConcreteActionEventData< CKeyStateChangedEventData >      TKeyStateChangedActionEventData;
typedef CTConcreteActionEventData< CKeyModStateChangedEventData >   TKeyModStateChangedActionEventData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_ACTIONEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-09-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/

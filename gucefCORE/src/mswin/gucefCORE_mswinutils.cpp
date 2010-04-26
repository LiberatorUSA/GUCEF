/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_mswinutils.h"

#ifdef GUCEF_MSWIN_BUILD

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#if _MSC_VER >= 1300    // for VC 7.0
  // from ATL 7.0 sources
  #ifndef _delayimp_h
  extern "C" IMAGE_DOS_HEADER __ImageBase;
  #endif
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

HMODULE 
GetCurrentModuleHandle( void )
{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

  // Here's a trick that will get you the handle of the module
  // you're running in without any a-priori knowledge:
  // http://www.dotnet247.com/247reference/msgs/13/65259.aspx

  MEMORY_BASIC_INFORMATION mbi;
  static int dummy;
  VirtualQuery( &dummy, &mbi, sizeof(mbi) );

  return reinterpret_cast<HMODULE>(mbi.AllocationBase);

#else    // VC 7.0

  // from ATL 7.0 sources

  return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */

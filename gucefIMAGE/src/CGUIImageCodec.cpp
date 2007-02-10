/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#include "CGUIImageCodec.h"

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

CGUIImageCodec::CGUIImageCodec( void )
    : CICodec()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CGUIImageCodec::CGUIImageCodec( const CGUIImageCodec& src )
    : CICodec( src )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CGUIImageCodec&
CGUIImageCodec::operator=( const CGUIImageCodec& src )
{TRACE;

    if ( &src != this )
    {
        CICodec::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
CGUIImageCodec::~CGUIImageCodec()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CGUIImageCodec::Encode( CORE::CIOAccess& source ,
                        CORE::CIOAccess& dest   )
{TRACE;

    // Since we are encoding from the native format to the native format nothing has to
    // be altered and we can simply copy the data
    dest.Write( source );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIImageCodec::Decode( CORE::CIOAccess& source ,
                        CORE::CIOAccess& dest   )
{TRACE;

    // Since we are decoding from the native format to the native format nothing has to
    // be altered and we can simply copy the data
    dest.Write( source );
    return true;
}

/*-------------------------------------------------------------------------*/
                         
CORE::CString
CGUIImageCodec::GetFamilyName( void ) const
{TRACE;

    return "ImageCodec";
}

/*-------------------------------------------------------------------------*/
    
CORE::CString
CGUIImageCodec::GetType( void ) const
{TRACE;
    
    return "gui";
}

/*-------------------------------------------------------------------------*/
    
CORE::CICloneable*
CGUIImageCodec::Clone( void ) const
{TRACE;

    return new CGUIImageCodec( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
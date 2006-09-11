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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#include "CException.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CException::CException( void )
{
    assert( 0 );
    /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CException::CException( const char* m_originFile   ,
                        const Int32 m_originLineNr )
    : m_originFile( m_originFile )     ,
      m_originLineNr( m_originLineNr )
{
}

/*-------------------------------------------------------------------------*/

CException::CException( const CException& src )
    : m_originFile( src.m_originFile )     ,
      m_originLineNr( src.m_originLineNr )
{
}

/*-------------------------------------------------------------------------*/

CException::~CException()
{
    
}

/*-------------------------------------------------------------------------*/

CException&
CException::operator=( const CException& src )
{	//TICS !GOLC-004: is protected
    if ( this != &src )
    {
        m_originFile = src.m_originFile;
        m_originLineNr = src.m_originLineNr;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

const char* 
CException::GetOriginFile( void ) const
{
    return m_originFile;
}

/*-------------------------------------------------------------------------*/

Int32 
CException::GetOriginLineNr( void ) const
{
    return m_originLineNr;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

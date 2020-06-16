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

#include <assert.h>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#include "gucefCORE_CIOAccessToIOStream.h"      /* definition of the class implemented here */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CIOAccessToIOStream::CIOAccessToIOStream( CIOAccess& access )
    : std::iostream( GUCEF_NULL )     
    , m_access( &access )
    , m_gCount( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::~CIOAccessToIOStream()
{GUCEF_TRACE;

    m_access = GUCEF_NULL;
} 

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_ostream& 
CIOAccessToIOStream::put( base_char_type c )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        m_access->Write( &c, sizeof(base_char_type), 1 );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_ostream& 
CIOAccessToIOStream::flush()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        m_access->Flush();
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_ostream& 
CIOAccessToIOStream::seekp( base_pos_type pos )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        m_access->Setpos( (UInt32) pos );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_ostream& 
CIOAccessToIOStream::seekp( base_off_type off, std::ios_base::seekdir way )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        m_access->Seek( (Int32) off, (Int32) way );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_pos_type 
CIOAccessToIOStream::tellp()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        return (UInt32) m_access->Tell();
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_ostream& 
CIOAccessToIOStream::write( const base_char_type* s, std::streamsize n )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        m_access->Write( s, sizeof(base_char_type), (UInt32) n );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

std::streamsize 
CIOAccessToIOStream::gcount() const
{GUCEF_TRACE;

    return m_gCount;
}

/*-------------------------------------------------------------------------*/

int 
CIOAccessToIOStream::get()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        int result = (int) m_access->GetChar();
        m_gCount = 1;
        return result;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::get( char& c )
{GUCEF_TRACE;
    
    if ( GUCEF_NULL != m_access )
    {
        c = m_access->GetChar();
        m_gCount = 1;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::get( char* s, std::streamsize n )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        return get( s, n, '\n' );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::get( char* s, std::streamsize n, char delim )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        std::streamsize i=0;
        for ( ; i<n; ++i )
        {
            if ( m_access->Eof() )
                break;

            char testChar = m_access->GetChar();
            if ( testChar != delim )
            {
                (*s) = testChar;
                ++s;
            }
            else
            {
                UInt32 pos = m_access->Tell();
                if ( pos > 0 )
                {
                    --pos; --i;
                    m_access->Setpos( pos );
                }
                break;
            }
        }
        if ( i == n && n > 0 )
        {
            --s; // ran out of space in output buffer. Decrement 1 space to add mandatory null terminator
        }
        (*s) = '\0';
        m_gCount = i;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::get( std::streambuf& sb )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        return get( sb, '\n' );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::get( std::streambuf& sb, char delim )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        std::streamsize charsTransferred=0;
        while ( !m_access->Eof() )
        {   
            char testChar = m_access->GetChar();
            if ( testChar != delim )
            {
                if ( EOF == sb.sputc( testChar ) )
                {
                    unget();
                    m_gCount = charsTransferred;
                    break;
                }
                ++charsTransferred;
            }
            else
            {
                unget();
                m_gCount = charsTransferred;
                break;
            }
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::getline( char* s, std::streamsize n )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        UInt32 pos = m_access->Tell();
        CString line = m_access->ReadLine();
        size_t smallest = n > line.Length() ? line.Length() : n;
        strncpy( s, line.C_String(), smallest );
        UInt32 newPos = m_access->Tell();
        m_gCount = newPos - pos;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::getline( char* s, std::streamsize n, char delim )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        //m_access->Read( s, sizeof(char), (UInt32) n );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::ignore( std::streamsize n, int delim )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        std::streamsize i=0;
        while ( !m_access->Eof() && i<n )
        {
            char testChar = m_access->GetChar();
            ++i;

            if ( testChar == delim )
            {
                break;
            }
        }
        m_gCount = i;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

int 
CIOAccessToIOStream::peek()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        UInt32 offset = m_access->Tell();
        int result = (int) m_access->GetChar();
        m_access->Setpos( offset );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::putback( char c )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        UInt32 offset = m_access->Tell();
        if ( offset > 0 )
        {
            --offset;
            m_access->Setpos( offset );

            m_access->Write( &c, sizeof(c), 1 );

            m_access->Setpos( offset );
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::read( char* s, std::streamsize n )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        m_access->Read( s, sizeof(char), n );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

std::streamsize 
CIOAccessToIOStream::readsome( char* s, std::streamsize n )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        //m_access->( s, sizeof(char), n );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::seekg( std::ios_base::streampos pos )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        m_access->Setpos( (UInt32) pos );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::seekg( std::ios_base::streamoff off, std::ios_base::seekdir way )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        m_access->Seek( (UInt32) off, (Int32) way );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

int 
CIOAccessToIOStream::sync()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        return 0;
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

std::ios_base::streampos 
CIOAccessToIOStream::tellg()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        return (std::ios_base::streampos) m_access->Tell();
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CIOAccessToIOStream::base_istream& 
CIOAccessToIOStream::unget()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_access )
    {
        UInt32 offset = m_access->Tell();
        if ( offset > 0 )
        {
            --offset;
            m_access->Setpos( offset );
        }
    }
    m_gCount = 0;
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

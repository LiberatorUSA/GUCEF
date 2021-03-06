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

#ifndef GUCEF_CORE_CIOACCESSTOIOSTREAM_H
#define GUCEF_CORE_CIOACCESSTOIOSTREAM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdio.h>
#include <iostream>

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"         
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"        /* base class for all media manipulators */
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename char_type, typename traits_type = std::char_traits< char_type > >
class TIOAccessToStreambuff : public std::basic_streambuf< char_type, traits_type >
{
    private:

    CIOAccess* m_access;
    
    public:
 
    using Base = std::basic_streambuf< char_type, traits_type >;
    using int_type = typename Base::int_type;
    using pos_type = typename traits_type::pos_type;
    using off_type = typename traits_type::off_type;
    using streamsize = typename std::streamsize;

    TIOAccessToStreambuff( CIOAccess& access )
        : Base()
        , m_access( &access )
    {
    }

    virtual ~TIOAccessToStreambuff()
    {
        m_access = GUCEF_NULL;
    }

    virtual int_type overflow( int_type c ) override
    {
        if ( GUCEF_NULL != m_access )
        {
            if ( 1 == m_access->Write( &c, sizeof(int_type), 1 ) )
                return traits_type::not_eof( c );    
        }
        return traits_type::eof();
    }

    virtual int_type underflow() override
    {
        if ( GUCEF_NULL != m_access )
        {
            char_type c;
            if ( 1 == m_access->Read( &c, sizeof(c), 1 ) )
                return traits_type::to_int_type( c );    
        }
        return traits_type::eof();
    }

    virtual int sync() override
    {
        if ( GUCEF_NULL != m_access )
        {
            m_access->Flush(); 
        }
        return 0;
    }

    virtual pos_type seekoff( off_type offset, std::ios_base::seekdir origin, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out ) override
    {
        // change position by offset, according to way and mode
        if ( GUCEF_NULL != m_access )
        {
            Int32 fOrigin = -1;
            switch ( origin )
            {
                case std::ios_base::beg: fOrigin = SEEK_SET; break;
                case std::ios_base::cur: fOrigin = SEEK_CUR; break;
                case std::ios_base::end: fOrigin = SEEK_END; break;
                default: return pos_type(off_type(-1));
            }

            if ( 0 == m_access->Seek( (Int32) offset, fOrigin ) )
                return (pos_type) m_access->Tell(); 
        }
        return pos_type(off_type(-1));
    }

    virtual pos_type seekpos( pos_type pos, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out ) override
    {
        // change to specified position, according to mode
        if ( GUCEF_NULL != m_access )
        {
            if ( 0 == m_access->Setpos( (UInt32) pos ) )
                return (pos_type) m_access->Tell(); 
        }
        return pos_type(off_type(-1));
    }

    virtual streamsize xsgetn( char_type* outBuffer, streamsize charsToRead ) 
    {
        if ( GUCEF_NULL != m_access )
        {
            UInt32 elementsRead = m_access->Read( outBuffer, sizeof(char_type), (UInt32)charsToRead );
            return (streamsize) elementsRead;    
        }
        return traits_type::eof();
    }
};

/*-------------------------------------------------------------------------*/

/**
 *  Basic and simplistic wrapper for IOAccess to STL style std iostream style data access.
 */
class GUCEF_CORE_PUBLIC_CPP CIOAccessToIOStream : public std::iostream
{
    public:

    typedef TIOAccessToStreambuff< char > ByteStreambuffFromIOAccess;
    
    CIOAccessToIOStream( CIOAccess& access );

    virtual ~CIOAccessToIOStream();

    private:

    CIOAccessToIOStream( void );                                       /**< not implemented */
    CIOAccessToIOStream( const CIOAccessToIOStream& src );             /**< not implemented */
    CIOAccessToIOStream& operator=( const CIOAccessToIOStream& src );  /**< not implemented */

    private:

    CIOAccess* m_access;
    ByteStreambuffFromIOAccess m_stlAdapter;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIOACCESSTOIOSTREAM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-07-2006 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/

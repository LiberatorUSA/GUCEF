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

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"         
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

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

class CDynamicBuffer;

/*-------------------------------------------------------------------------*/

/**
 *  Basic and simplistic wrapper for IOAccess to STL style std iostream style data access.
 */
class GUCEF_CORE_PUBLIC_CPP CIOAccessToIOStream : public std::iostream
{
    public:

    typedef std::basic_ostream< char, std::char_traits< char > >                base_ostream;
    typedef std::basic_istream< char, std::char_traits< char > >                base_istream;
    typedef std::basic_iostream< char, std::char_traits< char > >::char_type    base_char_type;
    typedef std::char_traits< char >::pos_type                                  base_pos_type;
    typedef std::basic_iostream< char, std::char_traits< char > >::off_type     base_off_type;
    
    CIOAccessToIOStream( CIOAccess& access );

    virtual ~CIOAccessToIOStream();

    virtual base_ostream& put( base_char_type c );

    virtual base_ostream& flush();

    virtual base_ostream& seekp( base_pos_type pos );

    virtual base_ostream& seekp( base_off_type off, std::ios_base::seekdir way );

    virtual base_pos_type tellp();

    virtual base_ostream& write( const base_char_type* s, std::streamsize n );

    virtual std::streamsize gcount() const;

    virtual int get();

    virtual base_istream& get( char& c );

    virtual base_istream& get( char* s, std::streamsize n );

    virtual base_istream& get( char* s, std::streamsize n, char delim );

    virtual base_istream& get( std::streambuf& sb );

    virtual base_istream& get( std::streambuf& sb, char delim );

    virtual base_istream& getline( char* s, std::streamsize n );

    virtual base_istream& getline( char* s, std::streamsize n, char delim );

    virtual base_istream& ignore( std::streamsize n = 1, int delim = EOF );

    virtual int peek();

    virtual base_istream& putback( char c );

    virtual base_istream& read( char* s, std::streamsize n );

    virtual std::streamsize readsome( char* s, std::streamsize n );

	virtual base_istream& seekg( std::ios_base::streampos pos );

    virtual base_istream& seekg( std::ios_base::streamoff off, std::ios_base::seekdir way );

    virtual int sync();

    virtual std::ios_base::streampos tellg();

    virtual base_istream& unget();

    private:

    CIOAccessToIOStream( void );                                       /**< not implemented */
    CIOAccessToIOStream( const CIOAccessToIOStream& src );             /**< not implemented */
    CIOAccessToIOStream& operator=( const CIOAccessToIOStream& src );  /**< not implemented */

    private:

    CIOAccess* m_access;
    std::streamsize m_gCount;
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

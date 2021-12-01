/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ?  */

#include "gucefCORE_CUrlEncoder.h"

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

void
CUrlEncoder::EncodeToAscii( const CORE::CUtf8String& strToBeEncoded ,
                            CORE::CAsciiString& encodedStr          )
{GUCEF_TRACE;

    encodedStr.Clear(); // @TODO: optimize memory management
    
    for ( UInt32 i=0; i<strToBeEncoded.Length(); ++i )
    {
        // @TODO: Optimize: We need to check every code point and seeks are expensive. 
        Int32 codePoint = strToBeEncoded.CodepointAtIndex( i );

        if ( ( codePoint >= '0' && codePoint <= '9' ) || // digits 0-9 are allowed
             ( codePoint >= 'A' && codePoint <= 'Z' ) || // letters A-Z are allowed
             ( codePoint >= 'a' && codePoint <= 'z' ) || // letters a-z are allowed
             ( codePoint == '-' || codePoint == '.' || codePoint == '_' || codePoint == '~' ) ) // a few special chars are allowed: "-", ".", "_", "~"
        {
            // Since we checked the ranges we can safely cast to a char
            encodedStr += (char) codePoint;
        }
        else
        {
            char hexCharA = 0;
            char hexCharB = 0;
            ConvertByteToHexChars( codePoint, hexCharA, hexCharB );
        // @TODO: handle multiple bytes    
            encodedStr += '%' + hexCharA + hexCharB;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CUrlEncoder::EncodeToAscii( const CORE::CAsciiString& strToBeEncoded , 
                            CORE::CAsciiString& encodedStr           )
{GUCEF_TRACE;

    encodedStr.Clear(); // @TODO: optimize memory management
    
    // Since the input is just ASCII we only have to worry about a few cases and no variable length encoding
    for ( UInt32 i=0; i<strToBeEncoded.Length(); ++i )
    {
        char codePoint = strToBeEncoded[ i ];
        if ( ( codePoint >= '0' && codePoint <= '9' ) || // digits 0-9 are allowed
             ( codePoint >= 'A' && codePoint <= 'Z' ) || // letters A-Z are allowed
             ( codePoint >= 'a' && codePoint <= 'z' ) || // letters a-z are allowed
             ( codePoint == '-' || codePoint == '.' || codePoint == '_' || codePoint == '~' ) ) // a few special chars are allowed: "-", ".", "_", "~"
        {
            encodedStr += codePoint;
        }
        else
        {
            char hexCharA = 0;
            char hexCharB = 0;
            ConvertByteToHexChars( codePoint, hexCharA, hexCharB );
            
            encodedStr += '%' + hexCharA + hexCharB;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CUrlEncoder::DecodeFromAscii( const CORE::CAsciiString& strToBeDecoded , 
                              CORE::CUtf8String& decodedStr            )
{GUCEF_TRACE;

    decodedStr.Clear(); // @TODO: optimize memory management
    
    // Since the input is just ASCII we only have to worry about a few cases and no variable length encoding
    for ( UInt32 i=0; i<strToBeDecoded.Length(); ++i )
    {
        char codePoint = strToBeDecoded[ i ];
        if ( codePoint == '%' )
        {
            // Since we have found an encoding marker we need to check the next 2 chars
            if ( i+2<strToBeDecoded.Length() )
            {
                char hexCharA = strToBeDecoded[ i+1 ];
                char hexCharB = strToBeDecoded[ i+2 ];

                // ConvertHexCharsToByte
                
                Int32 utfCodePoint = 0;
                decodedStr += utfCodePoint;
            }
            else
            {
                return;
            }
        }
        else
        {
            // ASCII fits within UTF8 so a cast will do
            decodedStr += (Int32) codePoint;
        }
    }
}

/*-------------------------------------------------------------------------*/

void 
CUrlEncoder::Encode( const CORE::CUtf8String& strToBeEncoded, CString& encodedStr )
{GUCEF_TRACE;

    #if ( GUCEF_DEFAULT_STRING_FORMAT == GUCEF_DATATYPE_ASCII_STRING )
    EncodeToAscii( strToBeEncoded, encodedStr );
    #else
    CORE::CAsciiString encodedAsciiStr;
    EncodeToAscii( strToBeEncoded, encodedAsciiStr );
    encodedStr = ToUtf8String( encodedAsciiStr );
    #endif
}

/*-------------------------------------------------------------------------*/

void
CUrlEncoder::Encode( const CORE::CAsciiString& strToBeEncoded, CString& encodedStr )
{GUCEF_TRACE;

    #if ( GUCEF_DEFAULT_STRING_FORMAT == GUCEF_DATATYPE_ASCII_STRING )
    EncodeToAscii( strToBeEncoded, encodedStr );
    #else
    CORE::CAsciiString encodedAsciiStr;
    EncodeToAscii( strToBeEncoded, encodedAsciiStr );
    encodedStr = ToUtf8String( encodedAsciiStr );
    #endif
}

/*-------------------------------------------------------------------------*/

void
CUrlEncoder::Decode( const CORE::CUtf8String& strToBeDecoded, CUtf8String& decodedStr )
{GUCEF_TRACE;

    DecodeFromAscii( ToAsciiString( strToBeDecoded ), decodedStr );
}

/*-------------------------------------------------------------------------*/

void
CUrlEncoder::Decode( const CORE::CAsciiString& strToBeDecoded, CUtf8String& decodedStr )
{GUCEF_TRACE;

    DecodeFromAscii( strToBeDecoded, decodedStr );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

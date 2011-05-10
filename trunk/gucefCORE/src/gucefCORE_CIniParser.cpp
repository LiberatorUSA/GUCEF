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

#include <stdio.h>

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#include "gucefCORE_CIniParser.h"

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

CIniParser::CIniParser( void )
    : m_iniData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CIniParser::CIniParser( const CIniParser& src )
    : m_iniData( src.m_iniData )
{GUCEF_TRACE;

}
    
/*-------------------------------------------------------------------------*/
    
CIniParser::~CIniParser()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIniParser&    
CIniParser::operator=( const CIniParser& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_iniData = src.m_iniData;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
bool
CIniParser::SaveTo( CDataNode& rootNode ) const
{GUCEF_TRACE;

    return false;
}
    
/*-------------------------------------------------------------------------*/
    
bool
CIniParser::SaveTo( const CString& filename ) const
{GUCEF_TRACE;

    FILE* fptr = fopen( filename.C_String(), "wb" );
    if ( NULL != fptr )
    {
        const CString valueSepStr = GUCEF_EOL;
        TIniMap::const_iterator i = m_iniData.begin();
        while ( i != m_iniData.end() )
        {
            const CString& keyString = GUCEF_EOL "[" + (*i).first + "]" GUCEF_EOL;
            CString values = (*i).second.GetAllPairs( valueSepStr );
            
            fwrite( keyString.C_String(), keyString.Length(), 1, fptr );
            fwrite( values.C_String(), values.Length(), 1, fptr );
            
            ++i;
        }        
        fclose( fptr );
    }
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
CIniParser::LoadFrom( const CDataNode& rootNode )
{GUCEF_TRACE;

    return false;
}
    
/*-------------------------------------------------------------------------*/

bool
CIniParser::IsCharIndexWithinQuotes( const CString& testString , 
                                     UInt32 charIndex          ,
                                     Int32 quotationStartIndex ,
                                     Int32 quotationEndIndex   )
{GUCEF_TRACE;

    // initial sanity check
    if ( charIndex+1 >= testString.Length() ) return false;
    
    // go through string identifying quoted sections as we go and checking whether
    // this encompasses the char given
    Int32 firstQuoteIndex = -1;
    bool firstQuoteFound = false;
    for ( UInt32 i=0; i<testString.Length(); ++i )
    {
        if ( testString[ i ] == '\"' )
        {
            if ( !firstQuoteFound )
            {
                if ( i > charIndex )
                {
                    // We went beyond the search char yet no opening quote
                    // was found this this char is not within a quoted section
                    quotationStartIndex = -1;
                    quotationEndIndex = -1;
                    return false;
                }                
                firstQuoteFound = true;
                firstQuoteIndex = i;
            }
            else
            {
                // this is a closing quote
                if ( i > charIndex )
                {
                    // this is an enclosing quotation section
                    quotationStartIndex = firstQuoteIndex;
                    quotationEndIndex = i;
                    return true;
                }
                firstQuoteFound = false;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

Int32
CIniParser::FindIndexOfNonQuotedEquals( const CString& testString )
{GUCEF_TRACE;

    Int32 equalsIndex = testString.HasChar( '=', 0, true );
    if ( equalsIndex > -1 )
    {
        Int32 dummy = 0;
        if ( IsCharIndexWithinQuotes( testString  , 
                                      equalsIndex ,
                                      dummy       ,
                                      dummy       ) )
        {
            // this is not the equals we want, it is quoted
            // lets try again
            // we will allow:
            //     " my key with a = string " = " my value "
            // where there is whitespace after the quoted section
            equalsIndex = testString.HasChar( '=', equalsIndex+1, true );
            if ( equalsIndex > -1 )
            {
                if ( !IsCharIndexWithinQuotes( testString  , 
                                               equalsIndex ,
                                               dummy       ,
                                               dummy       ) )
                {
                    return equalsIndex;
                }
                // else:
                // this is not valid, the quoted section should have been followed by a 
                // non quoted equals
            }
        }
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

CString
CIniParser::StripQuotation( const CString& testString )
{
    CString resultStr = testString.Trim( true ).Trim( false );
    if ( resultStr.Length() > 1 )
    {
        if ( ( resultStr[ 0 ] == '\"' ) && ( resultStr[ resultStr.Length()-1 ] == '\"' ) )
        {
            return resultStr.SubstrFromRange( 1, resultStr.Length()-2 );
        }
    }
    return resultStr;
}

/*-------------------------------------------------------------------------*/
    
bool
CIniParser::LoadFrom( const CString& filename )
{GUCEF_TRACE;

    // @TODO: take escape sequences into account
    
    CFileAccess fileAccess( filename, "rb" );
    if ( fileAccess.IsValid() )
    {
        CString sectionName;
        while ( !fileAccess.Eof() )
        {
            // Get the current line from the file, trimming white space on both ends
            CString line = fileAccess.ReadLine().Trim( true ).Trim( false );
            
            // Get rid of any trailing commentary on this line
            Int32 commentaryIndex = line.HasChar( ';', false );
            if ( commentaryIndex > -1 )
            {
                Int32 dummy = 0;
                if ( !IsCharIndexWithinQuotes( line, commentaryIndex, dummy, dummy ) )
                {
                    // we found a semicolon which is not contained within a quotation
                    // thus this is a commentary section which we should remove
                    line = line.CutChars( line.Length()-(commentaryIndex+1), false ).Trim( false );
                }
            }
            
            if( line.Length() > 0 )
            {
                // Check if this is a section tag line
                if ( ( line[ 0 ] == '[' ) && ( line[ line.Length()-1 ] == ']' ) )
                {
                    sectionName = line.SubstrFromRange( 1, line.Length()-2 );
                }
                else
                {
                    Int32 equalsIndex = FindIndexOfNonQuotedEquals( line );
                    if ( equalsIndex > -1 )
                    {
                        // get the key and value strings
                        CString sectionBeforeEquals = StripQuotation( line.SubstrFromRange( 0, equalsIndex-1 ) );
                        CString sectionAfterEquals = StripQuotation( line.SubstrFromRange( equalsIndex+1, line.Length()-1 ) );
                        
                        if ( ( sectionBeforeEquals.Length() > 0 ) &&
                             ( sectionAfterEquals.Length() > 0 )   )
                        {
                            CValueList* valueList = NULL;
                            TIniMap::iterator i = m_iniData.find( sectionName );
                            if ( i == m_iniData.end() )
                            {
                                valueList = &m_iniData[ sectionName ];
                                valueList->SetAllowDuplicates( true );
                                valueList->SetAllowMultipleValues( true );
                            }
                            else
                            {
                                valueList = &(*i).second;
                            }
                            
                            valueList->Set( sectionBeforeEquals, sectionAfterEquals );
                        }
                    }
                    // else:
                    // Line with junk on it we do not support
                    // we will try and be robust and ignore this line
                }
            }
        }
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

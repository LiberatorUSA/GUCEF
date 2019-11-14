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

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

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

void
CIniParser::Clear( void )
{GUCEF_TRACE;

    m_iniData.clear();
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::SaveTo( CDataNode& rootNode ) const
{GUCEF_TRACE;

    TIniData::const_iterator i = m_iniData.begin();
    while ( i != m_iniData.end() )
    {
        const TIniSection& iniSection = (*i);
        const CValueList& sectionData = iniSection.sectionData;

        CString nodeName = iniSection.sectionName.SubstrToChar( '\\', false );
        CString sectionName;

        if ( nodeName.Length()+1 < iniSection.sectionName.Length() )
        {
            sectionName = iniSection.sectionName.CutChars( nodeName.Length()+1, false );
        }

        // We use the ordering in the ini of the different sections to implicitly denote going up and down
        // a tree thus allowing branches to be defined in an ini file which otherwise does not support this concept
        CDataNode* sectionNodeParent = NULL;
        if ( !sectionName.IsNULLOrEmpty() )
        {
            CDataNode::TDataNodeVector existingPaths = rootNode.SearchForAll( sectionName, '\\', true, true );
            if ( !existingPaths.empty() )
            {
                sectionNodeParent = *existingPaths.rbegin();
            }
            else
            {
                sectionNodeParent = rootNode.Structure( sectionName, '\\' );
            }
        }
        else
        {
            sectionNodeParent = &rootNode;
        }

        // Make a new child node symbolizing the end of the sequence of the section name
        // for example [My/New/Sequence] as a section name in the ini would result in
        // the whole sequence creates as nodes with Sequence being the parent node
        // where we add the key/value pairs from the ini section
        CDataNode* sectionNode = sectionNodeParent->AddChild( nodeName );

        CValueList::TValueMap::const_iterator n = sectionData.GetDataBeginIterator();
        while ( n != sectionData.GetDataEndIterator() )
        {
            const CString& key = (*n).first;
            const CValueList::TStringVector& values = (*n).second;

            // Since key/value pairs in an ini section have no uniqueness constraint we cannot
            // set the pairs as attributes. Instead they are child nodes using the simplistic value representation
            CValueList::TStringVector::const_iterator m = values.begin();
            while ( m != values.end() )
            {
                CDataNode* keyValueNode = sectionNode->AddChild( key );
                keyValueNode->SetValue( (*m) );

                ++m;
            }
            ++n;
        }
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::SaveTo( CIOAccess& file ) const
{GUCEF_TRACE;

    if ( file.IsValid() && file.IsWriteable() )
    {
        const CString valueSepStr = GUCEF_EOL;

        TIniData::const_iterator i = m_iniData.begin();
        while ( i != m_iniData.end() )
        {
            CString keyString = GUCEF_EOL "[" + (*i).sectionName + "]" GUCEF_EOL;
            CString values = (*i).sectionData.GetAllPairs( valueSepStr );

            file.Write( keyString );
            file.Write( values );

            ++i;
        }
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::SaveTo( CString& outputIniString ) const
{GUCEF_TRACE;

    const CString valueSepStr = GUCEF_EOL;

    TIniData::const_iterator i = m_iniData.begin();
    while ( i != m_iniData.end() )
    {
        outputIniString += GUCEF_EOL "[" + (*i).sectionName + "]" GUCEF_EOL;
        outputIniString += (*i).sectionData.GetAllPairs( valueSepStr );
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::LoadFrom( const CDataNode& rootNode )
{GUCEF_TRACE;

    return LoadFrom( rootNode, NULL );
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::HasChildWithValueOrAttribs( const CDataNode& node )
{GUCEF_TRACE;

    CDataNode::const_iterator n = node.Begin();
    while ( n != node.End() )
    {
        bool childHasValue = (*n)->GetValue().Length() > 0;
        bool childHasAttribs = (*n)->GetAttCount() > 0;

        if ( childHasValue || childHasAttribs )
        {
            return true;
        }
        ++n;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::LoadFrom( const CDataNode& node       ,
                      TIniSection* currentSection )
{GUCEF_TRACE;

    TIniSection* iniSection = currentSection;

    // A node is a section node if it has no value set and no attributes
    const CString& value = node.GetValue();
    if ( node.GetAttCount() == 0 && value.IsNULLOrEmpty() )
    {
        // Check if this is just an link node which or the end delimiter of a section identifier
        // We do this by checking if the node has 1 or more children with a value or attributes.

        if ( HasChildWithValueOrAttribs( node ) )
        {
            // Base the new section path off of the path to the root node
            CString sectionName = node.GetPathToRoot( '\\', true );

            // Add each attribute as a key value pair is this section
            TIniSection dummySection;
            m_iniData.push_back( dummySection );
            iniSection = &(*m_iniData.rbegin());
            iniSection->sectionName = sectionName;
        }
    }
    else
    {
        if ( NULL != iniSection )
        {
            if ( !value.IsNULLOrEmpty() )
            {
                iniSection->sectionData.Set( node.GetName(), node.GetValue() );
            }

            // Add this section if there are attributes for this section
            if ( node.GetAttCount() > 0 )
            {
                // Add all attributes as key/value pairs in the ini
                // Note that the ini has no uniqueness constraint so that constraint on the data
                // stored as attributes is lost when representing the data via the ini format
                CDataNode::TAttributeMap::const_iterator i = node.AttributeBegin();
                while ( i != node.AttributeEnd() )
                {
                    iniSection->sectionData.Set( (*i).first, (*i).second.value );
                    ++i;
                }
            }
        }
    }

    // Repeat recursively for all the children
    CDataNode::const_iterator n = node.Begin();
    while ( n != node.End() )
    {
        if ( !LoadFrom( *(*n), iniSection ) )
        {
            return false;
        }
        ++n;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::IsCharIndexWithinQuotes( const CString& testString  ,
                                     UInt32 charIndex           ,
                                     Int32& quotationStartIndex ,
                                     Int32& quotationEndIndex   )
{GUCEF_TRACE;

    // initial sanity check
    if ( charIndex+1 >= testString.Length() ) return false;

    // go through string identifying quoted sections as we go and checking whether
    // this encompasses the char given
    Int32 quoteIndex = -1;
    bool firstQuoteFound = false;
    bool quoteFound = false;
    for ( UInt32 i=0; i<testString.Length(); ++i )
    {
        if ( i >= charIndex && !firstQuoteFound )
        {
            // there was no quotation char until now and we are at
            // the search char thus it logically cannot be between quotes
            quotationStartIndex = -1;
            quotationEndIndex = -1;
            return false;
        }

        if ( testString[ i ] == '\"' )
        {
            if ( !firstQuoteFound )
            {
                firstQuoteFound = true;
            }

            if ( !quoteFound )
            {
                if ( i > charIndex )
                {
                    // We went beyond the search char yet no opening quote
                    // was found this this char is not within a quoted section
                    quotationStartIndex = -1;
                    quotationEndIndex = -1;
                    return false;
                }
                quoteFound = true;
                quoteIndex = i;
            }
            else
            {
                // this is a closing quote
                if ( i > charIndex )
                {
                    // this is an enclosing quotation section
                    quotationStartIndex = quoteIndex;
                    quotationEndIndex = i;
                    return true;
                }
                quoteFound = false;
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
        else
        {
            // There are no quotes surrounding this equals so we are done
            return equalsIndex;
        }
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

CString
CIniParser::StripQuotation( const CString& testString )
{GUCEF_TRACE;

    CString resultStr = testString.Trim( true ).Trim( false );
    if ( resultStr.Length() > 1 )
    {
        if ( ( resultStr[ 0 ] == '\"' ) && ( resultStr[ resultStr.Length()-1 ] == '\"' ) )
        {
            return resultStr.SubstrFromRange( 1, resultStr.Length()-1 );
        }
    }
    return resultStr;
}

/*-------------------------------------------------------------------------*/

CIniParser::TIniData&
CIniParser::GetData( void )
{GUCEF_TRACE;

    return m_iniData;
}

/*-------------------------------------------------------------------------*/

const CIniParser::TIniData&
CIniParser::GetData( void ) const
{GUCEF_TRACE;

    return m_iniData;
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::LoadFrom( const CString& iniText )
{GUCEF_TRACE;

    CDynamicBuffer stringBuffer;
    stringBuffer.LinkTo( iniText.C_String(), iniText.Length() );
    CDynamicBufferAccess stringBufferAccess( &stringBuffer, false );
    return LoadFrom( stringBufferAccess );
}

/*-------------------------------------------------------------------------*/

bool
CIniParser::LoadFrom( CIOAccess& fileAccess )
{GUCEF_TRACE;

    // @TODO: take escape sequences into account
    if ( fileAccess.IsValid() )
    {
        bool isNewSection = true;
        CString sectionName;
        while ( !fileAccess.Eof() )
        {
            // Get the current line from the file, trimming white space on both ends
            CString line = fileAccess.ReadLine().Trim( true ).Trim( false );
            if( line.Length() > 0 )
            {
                // Get rid of any trailing commentary on this line
                Int32 commentaryIndex = line.HasChar( ';', false );
                if ( commentaryIndex > -1 )
                {
                    Int32 dummy = 0;
                    if ( !IsCharIndexWithinQuotes( line, commentaryIndex, dummy, dummy ) )
                    {
                        // we found a semicolon which is not contained within a quotation
                        // thus this is a commentary section which we should remove
                        line = line.CutChars( line.Length()-commentaryIndex, false ).Trim( false );
                    }
                }

                if( line.Length() > 0 )
                {
                    // Check if this is a section tag line
                    if ( ( line[ 0 ] == '[' ) && ( line[ line.Length()-1 ] == ']' ) )
                    {
                        sectionName = line.SubstrFromRange( 1, line.Length()-1 );
                        isNewSection = true;
                    }
                    else
                    {
                        Int32 equalsIndex = FindIndexOfNonQuotedEquals( line );
                        if ( equalsIndex > -1 )
                        {
                            // get the key and value strings
                            CString sectionBeforeEquals = StripQuotation( line.SubstrFromRange( 0, equalsIndex ) );
                            CString sectionAfterEquals = StripQuotation( line.SubstrFromRange( equalsIndex+1, line.Length() ) );

                            if ( ( sectionBeforeEquals.Length() > 0 ) &&
                                 ( sectionAfterEquals.Length() > 0 )   )
                            {
                                if ( isNewSection )
                                {
                                    TIniSection dummySection;
                                    m_iniData.push_back( dummySection );
                                    TIniSection& newSection = (*m_iniData.rbegin());
                                    newSection.sectionName = sectionName;
                                    newSection.sectionData.SetAllowDuplicates( true );
                                    newSection.sectionData.SetAllowMultipleValues( true );

                                    isNewSection = false;
                                }

                                TIniSection& newSection = (*m_iniData.rbegin());
                                newSection.sectionData.Set( sectionBeforeEquals, sectionAfterEquals );
                            }
                        }
                        // else:
                        // Line with junk on it we do not support
                        // we will try and be robust and ignore this line
                    }
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

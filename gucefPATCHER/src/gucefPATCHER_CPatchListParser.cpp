/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#include "gucefPATCHER_CPatchListParser.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchListParser::CPatchListParser( void )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CPatchListParser::~CPatchListParser()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchListParser::ParsePatchList( const CORE::CDataNode& patchListData ,
                                  TPatchList& patchList                ) const
{TRACE;

    const CORE::CDataNode* patchListNode = patchListData.Find( "PatchList" );
    
    if ( patchListNode != NULL )
    {
        const CORE::CDataNode* patchListEntry( NULL );
        CORE::CDataNode::const_iterator i = patchListNode->ConstBegin();
        while ( i != patchListNode->ConstEnd() )
        {
            patchListEntry = (*i);
            if ( patchListEntry->GetName().Equals( "PatchSetEntry", false ) )
            {
                // Validate that the patch set has a name property
                const CORE::CDataNode::TNodeAtt* attData = patchListEntry->GetAttribute( "Name" );
                if ( attData != NULL )
                {
                    // A name was found, we can continue on to parse the patch set's locations
                    const CORE::CString& patchSetName = attData->value;
                    
                    const CORE::CDataNode* patchSetLocEntry( NULL );
                    CORE::CDataNode::const_iterator n = patchListEntry->ConstBegin();
                    while ( n != patchListEntry->ConstEnd() )
                    {
                        // Get the information we need from each location entry
                        patchSetLocEntry = (*n);
                        if ( patchSetLocEntry->GetName().Equals( "Location", false ) )
                        {
                            // Get the URL of the patch set location
                            attData = patchListEntry->GetAttribute( "URL" );
                            if ( attData != NULL )
                            {
                                // We have a URL
                                const CORE::CString& patchSetLocURL = attData->value;
                                
                                // Get the codec name for the patch set data
                                attData = patchListEntry->GetAttribute( "Codec" ); 
                                if ( attData != NULL )
                                {
                                    // We have the codec name
                                    const CORE::CString& patchSetDataCodec = attData->value;
                                    
                                    // We have all the info we need for this location
                                    // we can now add the location to the list
                                    TPatchSetLocation location;
                                    location.codec = patchSetDataCodec;
                                    location.URL = patchSetLocURL;
                                    TPatchSetLocations& locationList = patchList[ patchSetName ];
                                    locationList.push_back( location );
                                }
                            }
                        }
                        
                        ++n;
                    }
                }
            }
                           
            ++i;
        }
    } 
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace PATCHER */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
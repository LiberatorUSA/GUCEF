/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2003.  All rights reserved.
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
#include <map>

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefPATCHER_CPatchSetParser.h"	/* definition of this class */

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

CPatchSetParser::CPatchSetParser( void )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CPatchSetParser::CPatchSetParser( const CPatchSetParser& src )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CPatchSetParser::~CPatchSetParser()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CPatchSetParser&
CPatchSetParser::operator=( const CPatchSetParser& src )
{TRACE;

    if ( &src != this )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetParser::ValidateAndParseFileLocEntries( const CORE::CDataNode& patchSetFileNode ,
                                                 TFileEntry& fileEntry                   ) const
{TRACE;

    const CORE::CDataNode* locationEntry( NULL );
    CORE::CDataNode::const_iterator i = patchSetFileNode.ConstBegin();
    while ( i != patchSetFileNode.ConstEnd() )
    {
        locationEntry = (*i);
        if ( locationEntry->GetName().Equals( "FileLocation", false ) )
        {
            // Validate that the file location has a URL property
            const CORE::CDataNode::TNodeAtt* attData = locationEntry->GetAttribute( "URL" );
            if ( attData != NULL )
            {
                TFileLocation fileLocation;
                fileLocation.URL = attData->value;
                
                // Check for an optional codec property
                const CORE::CDataNode::TNodeAtt* attData = locationEntry->GetAttribute( "Codec" );
                if ( attData != NULL )
                {
                    fileLocation.codec = attData->value;
                    
                    // Check for an optional codec parameter property
                    const CORE::CDataNode::TNodeAtt* attData = locationEntry->GetAttribute( "CodecParams" );
                    if ( attData != NULL )
                    {
                        fileLocation.codecParams = attData->value;
                    }                    
                }
                
                // Add the file location to our list
                fileEntry.fileLocations.push_back( fileLocation );
            }
            else
            {
                return false;
            }
        }
        
        ++i;
    }
    
    // A file entry must have at least 1 file location entry
    if ( fileEntry.fileLocations.size() >= 1 )
    {
        return true;
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetParser::ValidateAndParseFileEntry( const CORE::CDataNode& patchSetFileNode ,
                                            TFileEntry& fileEntry                   ) const
{TRACE;

    // Validate that the file has a name property
    const CORE::CDataNode::TNodeAtt* attData = patchSetFileNode.GetAttribute( "Name" );
    if ( attData != NULL )
    {
        // We found our filename
        const CORE::CString& filename = attData->value;

        // Validate that the file has a size property
        const CORE::CDataNode::TNodeAtt* attData = patchSetFileNode.GetAttribute( "Size" );
        if ( attData != NULL )
        {
            // We found our file's size
            const CORE::CString& fileSizeStr = attData->value;
            UInt32 fileSize = CORE::StringToInt32( fileSizeStr );
            
            // Validate that the file has a hash property
            const CORE::CDataNode::TNodeAtt* attData = patchSetFileNode.GetAttribute( "Hash" );
            if ( attData != NULL )
            {
                const CORE::CString& fileHash = attData->value;
                
                // Our file entry has everything we expected
                // We will now parse the file location entries for this file
                if ( ValidateAndParseFileLocEntries( patchSetFileNode ,
                                                     fileEntry        ) )
                {
                    // Success,.. the file and it's locations have been parsed
                    fileEntry.hash = fileHash;
                    fileEntry.name = filename;
                    fileEntry.sizeInBytes = fileSize;

                    return true;
                }
            }                       
        }                    
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetParser::ValidateAndParseDirEntry( const CORE::CDataNode& patchSetDirNode ,
                                           TDirEntry& dirEntry                    ) const
{TRACE;

    // Validate that the directory has a name property
    const CORE::CDataNode::TNodeAtt* attData = patchSetDirNode.GetAttribute( "Name" );
    if ( attData != NULL )
    {
        // We found our directory name
        const CORE::CString& dirName = attData->value;

        // Validate that the directory has a total size property
        const CORE::CDataNode::TNodeAtt* attData = patchSetDirNode.GetAttribute( "TotalSize" );
        if ( attData != NULL )
        {
            // We found our directory's total size
            const CORE::CString& dirTotalSizeStr = attData->value;
            UInt32 totalDirSize = CORE::StringToInt32( dirTotalSizeStr );
            
            // Validate that the directory has a hash property
            const CORE::CDataNode::TNodeAtt* attData = patchSetDirNode.GetAttribute( "Hash" );
            if ( attData != NULL )
            {
                // Our directory entry has everything we expected
                // It can now be added to the patch set
                dirEntry.hash = attData->value;
                dirEntry.name = dirName;
                dirEntry.sizeInBytes = totalDirSize;
                
                return true;
            }                       
        }                    
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetParser::ParseAndWalkDirTree( const CORE::CDataNode& patchSetDirNode ,
                                      TDirEntry& parentDir                   ) const
{
    // The directory info has been validated and parsed
    // We now iterate the directory entries
    const CORE::CDataNode* patchSetDirEntry( NULL );
    CORE::CDataNode::const_iterator i = patchSetDirNode.ConstBegin();
    while ( i != patchSetDirNode.ConstEnd() )
    {                
        patchSetDirEntry = (*i);
        if ( patchSetDirEntry->GetName().Equals( "Dir", false ) )
        {        
            TDirEntry dirEntry;
            if ( ValidateAndParseDirEntry( *patchSetDirEntry ,
                                           dirEntry          ) )
            {
                // Add the sub-directory
                parentDir.subDirs.push_back( dirEntry );
                TDirEntry& addedDir = parentDir.subDirs.back();
                
                // Parse and walk the sub-dir
                if ( !ParseAndWalkDirTree( *patchSetDirEntry ,
                                           addedDir          ) )
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        if ( patchSetDirEntry->GetName().Equals( "File", false ) )
        {
            // Validate the file entry
            TFileEntry fileEntry;
            if ( ValidateAndParseFileEntry( *patchSetDirEntry ,
                                            fileEntry         ) )
            {
                // Add the file to the directory
                parentDir.files.push_back( fileEntry );
            }
            else
            {
                return false;
            }
        }
        
        ++i;
    }
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetParser::ParseTopLevelDir( const CORE::CDataNode& patchSetDirNode ,
                                   TPatchSet& patchSet                    ) const
{TRACE;
    
    TDirEntry dirEntry;
    if ( ValidateAndParseDirEntry( patchSetDirNode ,
                                   dirEntry        ) )
    {
        if ( ParseAndWalkDirTree( patchSetDirNode ,
                                  dirEntry        ) )
        {
            patchSet.push_back( dirEntry );
            return true;
        }
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetParser::ParsePatchSet( const CORE::CDataNode& patchSetData ,
                                TPatchSet& patchSet                 ) const
{TRACE;

    const CORE::CDataNode* patchSetNode = patchSetData.Find( "PatchSet" );

    if ( patchSetNode != NULL )
    {
        // We found our patch set, now we go trough all top-level directories
        const CORE::CDataNode* patchSetEntry( NULL );
        CORE::CDataNode::const_iterator i = patchSetNode->ConstBegin();
        while ( i != patchSetNode->ConstEnd() )
        {
            patchSetEntry = (*i);
            if ( patchSetEntry->GetName().Equals( "Dir", false ) )
            {
                // Parse the directory
                if ( !ParseTopLevelDir( *patchSetEntry ,
                                        patchSet       ) )
                {
                    // Malformed patch set data structure found
                    return false;
                }
            }
            
            ++i;
        }
        
        return true;
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetParser::ParseAndWalkDirTree( const TDirEntry& patchSetDir   ,
                                      CORE::CDataNode& parentDirNode ) const
{TRACE;

    // Set all attributes of this dir node
    CORE::CDataNode newNode;
    newNode.SetName( "Dir" );
    newNode.SetAttribute( "Name", patchSetDir.name );
    newNode.SetAttribute( "TotalSize", CORE::Int32ToString( patchSetDir.sizeInBytes ) );
    newNode.SetAttribute( "Hash", patchSetDir.hash );
    
    // Recursively add all sub-directories
    for ( UInt32 i=0; i<patchSetDir.subDirs.size(); ++i )
    {
        if ( !ParseAndWalkDirTree( patchSetDir.subDirs[ i ] ,
                                   newNode                  ) )
        {
            return false;
        }
    }
    
    // Now we add the files
    const TFileEntry* fileEntry = NULL;
    CORE::CDataNode fileNode;
    fileNode.SetName( "File" );
    for ( UInt32 i=0; i<patchSetDir.files.size(); ++i )
    {
        fileEntry = &patchSetDir.files[ i ];
        fileNode.SetAttribute( "Name", fileEntry->name );
        fileNode.SetAttribute( "Hash", fileEntry->hash );
        fileNode.SetAttribute( "Size", CORE::Int32ToString( fileEntry->sizeInBytes ) );
        
        const TFileLocations& locList = fileEntry->fileLocations;
        const TFileLocation* fileLoc = NULL;
        CORE::CDataNode fileLocNode;
        fileLocNode.SetName( "FileLocation" );
        for ( UInt32 n=0; n<locList.size(); ++n )
        {
            fileLoc = &locList[ n ];
            fileLocNode.SetAttribute( "URL", fileLoc->URL );
            fileLocNode.SetAttribute( "Codec", fileLoc->codec );
            fileLocNode.SetAttribute( "CodecParams", fileLoc->codecParams );
            
            fileNode.AddChild( fileLocNode );
        }
        
        newNode.AddChild( fileNode );
        fileNode.DelSubTree();
    }
    
    // Now we copy our entire 'newNode' tree below the parent node
    parentDirNode.AddChild( newNode );    
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetParser::ParsePatchSet( const TPatchSet& patchSetData ,
                                CORE::CDataNode& patchSet     ) const
{TRACE;
    
    patchSet.SetName( "PatchSet" );
    patchSet.DelSubTree();
    patchSet.ClearAttributes();
    
    for ( UInt32 i=0; i<patchSetData.size(); ++i )
    {
        const TDirEntry& dirEntry = patchSetData[ i ];
        if ( !ParseAndWalkDirTree( dirEntry ,
                                   patchSet ) )
        {
            return false;
        }
    }
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

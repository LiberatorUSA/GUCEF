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
#include "dvcppstringutils.h"
#include "CStringList.h"
#include "gucefPATCHER_CPatchSetParserEventHandler.h"
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

void 
CPatchSetParser::ParseSourceList( const CORE::CDataNode& rootNode ,
                                  TSourceList& sourceList         )
{TRACE;
        const CORE::CDataNode* sourceListNode = rootNode.Find( "SourceList" );

        const CORE::CDataNode* sourceEntry( NULL );
        CORE::CDataNode::const_iterator i = sourceListNode->ConstBegin();
        while ( i != sourceListNode->ConstEnd() )
        {
            sourceEntry = (*i);
            if ( sourceEntry->GetName().Equals( "Source", false ) )
            {
                // we found an entry, parse the data and add to our list
                TSourceInfo entryInfo;
                entryInfo.Tag = sourceEntry->GetAttribute( "Tag" )->value;
                entryInfo.URL = sourceEntry->GetAttribute( "URL" )->value;
                entryInfo.Alternative = sourceEntry->GetAttribute( "Alternative" )->value;
                entryInfo.Codec = sourceEntry->GetAttribute( "Codec" )->value;
                entryInfo.CodecParams = sourceEntry->GetAttribute( "CodecParams" )->value;
                
                sourceList[ entryInfo.Tag ] = entryInfo;
            }                
            ++i;
        }       
}

/*-------------------------------------------------------------------------*/

void
CPatchSetParser::ProcessPatchListImp( const CORE::CDataNode& rootNode   ,
                                      const CORE::CString localPath     ,
                                      TEventHandlerList& eventHandlers  )
{TRACE; /*
    CORE::CStringList ignoreList;
    ignoreList.Append( "Source" );            
               
    const CORE::CDataNode* listEntry( NULL );
    CORE::CDataNode::const_iterator i( rootNode.ConstBegin() );
    while ( i != rootNode.ConstEnd() )
    {
        listEntry = (*i);
        if ( listEntry->GetName() == "Dir" )
        {                        
            // append the dir name to the local path
            CORE::CString subPath( localPath );            
            if ( subPath.Length() )
            {
                subPath += '/';
            }
            subPath += listEntry->GetAttribute( "Name" )->value;
            
            // perform a diff check on the sub-dir
            SubtractOldFromNewLocalListImp( oldLocalListRoot ,
                                            newLocalListNode ,
                                            localPath        ,
                                            diffList         );
        }
        else 
        if ( listEntry->GetName() == "File" )
        {
            CORE::CString filePath = localPath + "/" + listEntry->GetAttribute( "Name" )->value;
            
            // Try and locate the path + file in the old list
            const CORE::CDataNode* oldListEntry( FindPathInLocalList( oldLocalListRoot ,
                                                                      localPath        ) );
                                                           
            if ( oldListEntry )
            {                        
                // compare the datanodes, all attributes should match except the source tag
                if( !oldListEntry->Compare( *listEntry ,
                                            ignoreList ) )
                {
                    // the two entries differ by more then just the 'Source' field
                    diffList.AddChild( *listEntry );
                }
                
                // if we get here then the enties are considdered identical so no difference
                // is detected.
            }
            else
            {
                // The old list does not have this entry so it belongs in the diff
                diffList.AddChild( *listEntry );
            }                 
        }           
        ++i;
    }    */
}

/*-------------------------------------------------------------------------*/

void
CPatchSetParser::ProcessPatchList( const CORE::CDataNode& rootNode  ,
                                   TEventHandlerList& eventHandlers )
{TRACE;
    const CORE::CDataNode* patchSetNode = rootNode.Find( "PatchSet" );
    if ( patchSetNode )
    {
        // lets get started shall we,.. get the patchset name
        CORE::CString patchSetName( patchSetNode->GetAttribute( "Name" )->value );
        TEventHandlerList::iterator i( eventHandlers.begin() );
        while ( i != eventHandlers.end() )
        {
            (*i)->OnPatchSetStart( patchSetName );
            ++i;
        }

        // First we get the list of recource sources
        TSourceList sourceList;
        ParseSourceList( *patchSetNode, sourceList );
        
        // No point in proceding if we have no sources
        if ( sourceList.size() > 0 )
        {  
            const CORE::CDataNode* localListNode = patchSetNode->Find( "Local" );
            if ( localListNode )
            {
                // we found our dir/file list root node, we obtain some basic attributes of the tree
                CORE::CString localRootPath( CORE::RelativePath( localListNode->GetAttribute( "RootPath" )->value ) );                
                if ( 0 == localRootPath.Length() )
                {
                    // as common for file access,.. if no explicit path is given then the working dir
                    // is used as a root.
                    localRootPath = CORE::RelativePath( "$WORKDIR$" );
                }
                
                // call the recursive tree walker to process the dir structure
                ProcessPatchListImp( *localListNode ,
                                     localRootPath  ,
                                     eventHandlers  );
            }
        }
        
        i = eventHandlers.begin();
        while ( i != eventHandlers.end() )
        {
            (*i)->OnPatchSetEnd( patchSetName );
            ++i;
        }
    }
    else
    {
        TEventHandlerList::iterator i( eventHandlers.begin() );
        while ( i != eventHandlers.end() )
        {
            (*i)->OnParserError();
            ++i;
        }   
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode*
CPatchSetParser::FindPathInLocalListImp( const CORE::CDataNode& currentNode ,
                                         const CORE::CString& path          )
{TRACE;
    CORE::CString dirName( path.SubstrToChar( '/' ) );
    bool lookingForDir( dirName.Length() > 0 );
    
    const CORE::CDataNode* n;    
    CORE::CDataNode::const_iterator i( currentNode.ConstBegin() );
    
    if ( lookingForDir )
    {
        // we are looking for a sub-dir node
        while ( i != currentNode.ConstEnd() )
        {
            n = (*i);
            if ( n->GetName() == "Dir" )
            {
                if ( n->GetAttribute( "Name" )->value == dirName )
                {
                    CORE::CString subPath( path.CutChars( dirName.Length()+1, true ) );
                    return FindPathInLocalListImp( *n, subPath );
                }
            }                  
            ++i;
        }
    }
    else
    {
        // we are looking for the file node
        while ( i != currentNode.ConstEnd() )
        {                        
            n = (*i);
            if ( n->GetName() == "File" )
            {
                if ( n->GetAttribute( "Name" )->value == path )
                {
                    return n;
                }                
            }                  
            ++i;
        }    
    }        
    return NULL;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode*
CPatchSetParser::FindPathInLocalList( const CORE::CDataNode& rootNode ,
                                      const CORE::CString& path       )
{TRACE;
     const CORE::CDataNode* localListNode = rootNode.Find( "Local" );
     if ( localListNode )
     {
        return FindPathInLocalListImp( *localListNode, path );
     }
     return NULL;
}

/*-------------------------------------------------------------------------*/

void
CPatchSetParser::SubtractOldFromNewLocalListImp( const CORE::CDataNode& oldLocalListRoot ,
                                                 const CORE::CDataNode& newLocalListNode ,
                                                 const CORE::CString& localPath          ,
                                                 CORE::CDataNode& diffList               )
{TRACE;
    CORE::CStringList ignoreList;
    ignoreList.Append( "Source" );            
               
    const CORE::CDataNode* listEntry( NULL );
    CORE::CDataNode::const_iterator i( newLocalListNode.ConstBegin() );
    while ( i != newLocalListNode.ConstEnd() )
    {
        listEntry = (*i);
        if ( listEntry->GetName() == "Dir" )
        {            
            // we always copy the new dir structure to the diff list
            diffList.AddChild( *listEntry );
            
            // append the dir name to the local path
            CORE::CString subPath( localPath );            
            if ( subPath.Length() )
            {
                subPath += '/';
            }
            subPath += listEntry->GetAttribute( "Name" )->value;
            
            // perform a diff check on the sub-dir
            SubtractOldFromNewLocalListImp( oldLocalListRoot ,
                                            newLocalListNode ,
                                            localPath        ,
                                            diffList         );
        }
        else 
        if ( listEntry->GetName() == "File" )
        {
            CORE::CString filePath = localPath + "/" + listEntry->GetAttribute( "Name" )->value;
            
            // Try and locate the path + file in the old list
            const CORE::CDataNode* oldListEntry( FindPathInLocalList( oldLocalListRoot ,
                                                                      localPath        ) );
                                                           
            if ( oldListEntry )
            {                        
                // compare the datanodes, all attributes should match except the source tag
                if( !oldListEntry->Compare( *listEntry ,
                                            ignoreList ) )
                {
                    // the two entries differ by more then just the Source field
                    diffList.AddChild( *listEntry );
                }
                
                // if we get here then the enties are considdered identical so no difference
                // is detected.
            }
            else
            {
                // The old list does not have this entry so it belongs in the diff
                diffList.AddChild( *listEntry );
            }                 
        }           
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CPatchSetParser::SubtractOldFromNewLocalList( const CORE::CDataNode& oldLocalList ,
                                              const CORE::CDataNode& newLocalList ,
                                              CORE::CDataNode& diffList           )
{TRACE;
    assert( &oldLocalList );
    assert( &newLocalList );
    assert( &diffList );
    
    const CORE::CDataNode* oldLocalListNode = oldLocalList.Find( "Local" );
    const CORE::CDataNode* newLocalListNode = newLocalList.Find( "Local" );
    
    if ( oldLocalListNode && newLocalListNode )
    {        
        if ( *oldLocalListNode == *newLocalListNode )
        {                    
            // call recursive tree walker to parse the structure
            CORE::CString localPath;
            SubtractOldFromNewLocalListImp( *oldLocalListNode ,
                                            *newLocalListNode ,
                                            localPath         ,
                                            diffList          );                                            
        }
        else
        {
            // if the root nodes differ then the entire tree is considdered to
            // be different. The diffList will be the entire newLocalList
            diffList.Copy( newLocalList );
        }
    }
    else
    if ( newLocalListNode )
    {
        // if there is no correct old list the entire tree is considdered to
        // be different. The diffList will be the entire newLocalList
        diffList.Copy( newLocalList );    
    }          
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

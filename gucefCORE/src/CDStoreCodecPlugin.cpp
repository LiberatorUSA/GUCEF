/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "dvoswrap.h"            /* OS wrapping functionality */
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_DSTORECODECPLUGINSTRUCTS_H
#include "DStoreCodecPluginStructs.h"   /* shared structs */
#define GUCEF_CORE_DSTORECODECPLUGINSTRUCTS_H
#endif /* GUCEF_CORE_DSTORECODECPLUGINSTRUCTS_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"          /* data storage node class,.. this is what it's all about */
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"        /* file recource access */
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#include "CDStoreCodecPlugin.h"  /* definition of the class implemented here */

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
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SParserData
{
        CDataNode* root;
        CDataNode* curnode;
        CString error;
        Int32 errorcode;
};

typedef struct SParserData TParserData;

/*-------------------------------------------------------------------------*/

enum
{
        DSTOREPLUG_INIT            = 0   ,
        DSTOREPLUG_SHUTDOWN              ,
        DSTOREPLUG_NAME                  ,
        DSTOREPLUG_COPYRIGHT             ,
        DSTOREPLUG_VERSION               ,
        DSTOREPLUG_TYPE                  ,
        
        DSTOREPLUG_DEST_FILE_OPEN        ,
        DSTOREPLUG_DEST_FILE_CLOSE       ,
        DSTOREPLUG_BEGIN_NODE_STORE      ,
        DSTOREPLUG_END_NODE_STORE        ,
        DSTOREPLUG_STORE_NODE_ATT        ,
        DSTOREPLUG_BEGIN_NODE_CHILDREN   ,
        DSTOREPLUG_END_NODE_CHILDREN     ,        
        
        DSTOREPLUG_SRC_FILE_OPEN         ,
        DSTOREPLUG_SRC_FILE_CLOSE        ,
        DSTOREPLUG_SET_READ_HANDLERS     ,                                     
        DSTOREPLUG_START_READING         ,
        
        DSTOREPLUG_LASTFPTR
};

/*-------------------------------------------------------------------------*/

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Init )                ( void** plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Shutdown )              ( void** plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Name )           ( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Copyright )      ( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const TVersion* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Version )    ( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Type )           ( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Dest_File_Open )      ( void** plugdata, void** filedata, TIOAccess* file ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Dest_File_Close )       ( void** plugdata, void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Begin_Node_Store )      ( void** plugdata, void** filedata, const char* nodename, UInt32 attscount, UInt32 haschildren ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_End_Node_Store )        ( void** plugdata, void** filedata, const char* nodename, UInt32 attscount, UInt32 haschildren ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Store_Node_Att )        ( void** plugdata, void** filedata, const char* nodename, UInt32 attscount, UInt32 attindex, const char* attname, const char* attvalue, UInt32 haschildren ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Begin_Node_Children )   ( void** plugdata, void** filedata, const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX; 
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_End_Node_Children )     ( void** plugdata, void** filedata, const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Src_File_Open )       ( void** plugdata, void** filedata, TIOAccess* file ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Src_File_Close )        ( void** plugdata, void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Set_Read_Handlers )     ( void** plugdata, void** filedata, const TReadHandlers* rhandlers, void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDSTOREPLUGFPTR_Start_Reading )       ( void** plugdata, void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnTreeBeginHandler( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        TParserData* pd = static_cast<TParserData*>(privdata);
        pd->errorcode = 0;
        pd->error = NULL;
        pd->curnode = NULL;
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX 
OnTreeEndHandler( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        /* currently not used */
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX 
OnNodeBeginHandler( void* privdata       , 
                    const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        TParserData* pd = static_cast<TParserData*>(privdata);
        if ( pd )
        {
                if ( pd->curnode )
                {
                        CDataNode newchild( nodename );
                        pd->curnode = pd->curnode->AddChild( newchild );
                        return;
                }
                
                /*
                *      First node
                */
                pd->curnode = pd->root;
                pd->curnode->SetName( nodename ); 
        }                
}

/*-------------------------------------------------------------------------*/
                    
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
OnNodeEndHandler( void* privdata       , 
                  const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        TParserData* pd = static_cast<TParserData*>(privdata);
        if ( pd )
        {
                if ( pd->curnode )
                {
                        pd->curnode = pd->curnode->GetParent();
                }        
        }
}

/*-------------------------------------------------------------------------*/
                  
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
OnNodeAttHandler( void* privdata       , 
                  const char* nodename , 
                  const char* attname  , 
                  const char* attvalue ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        TParserData* pd = static_cast<TParserData*>(privdata);
        if ( pd )
        {
                if ( pd->curnode )
                {
                        pd->curnode->SetAttribute( attname, attvalue );
                }                        
        }
}

/*-------------------------------------------------------------------------*/
                  
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
OnNodeChildrenBeginHandler( void* privdata       ,
                            const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        /* currently not used */
}
                                          
/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX 
OnNodeChildrenEndHandler( void* privdata       , 
                          const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        /* currently not used */
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnParserErrorHandler( void* privdata          ,
                      Int32 errorcode         , 
                      const char* description ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        TParserData* pd = static_cast<TParserData*>(privdata);
        pd->error = description;
        pd->errorcode = errorcode;
        
        DEBUGOUTPUTss( "DStore Codec error: ", description );
}                      

/*-------------------------------------------------------------------------*/

CDStoreCodecPlugin::CDStoreCodecPlugin( const CDStoreCodecPlugin& src )
{TRACE;
        
        /* dummy, don't use */
}                   

/*-------------------------------------------------------------------------*/

CDStoreCodecPlugin::CDStoreCodecPlugin( void )
{TRACE;        
        /* dummy, don't use */
}

/*-------------------------------------------------------------------------*/

CDStoreCodecPlugin::CDStoreCodecPlugin( const CString& pluginfile )
        : _id( 0 )                  ,
          _sohandle( NULL )         ,
          _pluginfile( pluginfile )
{TRACE;
        
        _ref._plugin = this;
     
        _sohandle = LoadModuleDynamicly( pluginfile.C_String() );
        if ( !_sohandle ) return;               
        
        _fptable[ DSTOREPLUG_INIT ] = GetFunctionAddress( _sohandle         ,
                                                          "DSTOREPLUG_Init" ,
                                                          1*sizeof(void*)   );
        _fptable[ DSTOREPLUG_SHUTDOWN ] = GetFunctionAddress( _sohandle             ,
                                                              "DSTOREPLUG_Shutdown" ,
                                                              1*sizeof(void*)       );
        _fptable[ DSTOREPLUG_NAME ] = GetFunctionAddress( _sohandle         ,
                                                          "DSTOREPLUG_Name" ,
                                                          1*sizeof(void*)   );
        _fptable[ DSTOREPLUG_COPYRIGHT ] = GetFunctionAddress( _sohandle              ,
                                                               "DSTOREPLUG_Copyright" ,
                                                               1*sizeof(void*)        );
        _fptable[ DSTOREPLUG_VERSION ] = GetFunctionAddress( _sohandle            ,
                                                             "DSTOREPLUG_Version" ,
                                                             1*sizeof(void*)      );                                                               
        _fptable[ DSTOREPLUG_TYPE ] = GetFunctionAddress( _sohandle         ,
                                                          "DSTOREPLUG_Type" ,
                                                          1*sizeof(void*)   );
        _fptable[ DSTOREPLUG_DEST_FILE_OPEN ] = GetFunctionAddress( _sohandle                   ,
                                                                    "DSTOREPLUG_Dest_File_Open" ,
                                                                    3*sizeof(void*)             );
        _fptable[ DSTOREPLUG_DEST_FILE_CLOSE ] = GetFunctionAddress( _sohandle                    ,
                                                                     "DSTOREPLUG_Dest_File_Close" ,
                                                                     2*sizeof(void*)              );
        _fptable[ DSTOREPLUG_BEGIN_NODE_STORE ] = GetFunctionAddress( _sohandle                    ,
                                                                     "DSTOREPLUG_Begin_Node_Store" ,
                                                                     3*sizeof(void*)+8             );                                                                                                                                                                                                   
        _fptable[ DSTOREPLUG_END_NODE_STORE ] = GetFunctionAddress( _sohandle                   ,
                                                                    "DSTOREPLUG_End_Node_Store" ,
                                                                    3*sizeof(void*)+8           );
        _fptable[ DSTOREPLUG_STORE_NODE_ATT ] = GetFunctionAddress( _sohandle                   ,
                                                                    "DSTOREPLUG_Store_Node_Att" ,
                                                                    5*sizeof(void*)+12          );                                                                                                                                                                                                            
        _fptable[ DSTOREPLUG_BEGIN_NODE_CHILDREN ] = GetFunctionAddress( _sohandle                        ,
                                                                         "DSTOREPLUG_Begin_Node_Children" ,
                                                                         3*sizeof(void*)                  );
        _fptable[ DSTOREPLUG_END_NODE_CHILDREN ] = GetFunctionAddress( _sohandle                      ,
                                                                       "DSTOREPLUG_End_Node_Children" ,
                                                                       3*sizeof(void*)                );                                                                         
        _fptable[ DSTOREPLUG_SRC_FILE_OPEN ] = GetFunctionAddress( _sohandle                  ,
                                                                   "DSTOREPLUG_Src_File_Open" ,
                                                                   2*sizeof(void*)            );
        _fptable[ DSTOREPLUG_SRC_FILE_CLOSE ] = GetFunctionAddress( _sohandle                   ,
                                                                    "DSTOREPLUG_Src_File_Close" ,
                                                                    2*sizeof(void*)                   ); 
        _fptable[ DSTOREPLUG_SET_READ_HANDLERS ] = GetFunctionAddress( _sohandle                      ,
                                                                       "DSTOREPLUG_Set_Read_Handlers" ,
                                                                       4*sizeof(void*)                );
        _fptable[ DSTOREPLUG_START_READING ] = GetFunctionAddress( _sohandle                  ,
                                                                   "DSTOREPLUG_Start_Reading" ,
                                                                   2*sizeof(void*)            );                                                                                                                                                                                                                      
                                                                                                                       
        
        if ( ( !_fptable[ DSTOREPLUG_INIT ] ) ||
             ( !_fptable[ DSTOREPLUG_SHUTDOWN ] ) ||
             ( !_fptable[ DSTOREPLUG_NAME ] ) ||
             ( !_fptable[ DSTOREPLUG_COPYRIGHT ] ) ||
             ( !_fptable[ DSTOREPLUG_VERSION ] ) ||
             ( !_fptable[ DSTOREPLUG_TYPE ] ) ||
             ( !_fptable[ DSTOREPLUG_DEST_FILE_OPEN ] ) ||
             ( !_fptable[ DSTOREPLUG_DEST_FILE_CLOSE ] ) ||
             ( !_fptable[ DSTOREPLUG_BEGIN_NODE_STORE ] ) ||
             ( !_fptable[ DSTOREPLUG_END_NODE_STORE ] ) ||
             ( !_fptable[ DSTOREPLUG_STORE_NODE_ATT ] ) ||
             ( !_fptable[ DSTOREPLUG_BEGIN_NODE_CHILDREN ] ) ||
             ( !_fptable[ DSTOREPLUG_END_NODE_CHILDREN ] ) ||
             ( !_fptable[ DSTOREPLUG_SRC_FILE_OPEN ] ) ||
             ( !_fptable[ DSTOREPLUG_SRC_FILE_CLOSE ] ) ||
             ( !_fptable[ DSTOREPLUG_SET_READ_HANDLERS ] ) ||
             ( !_fptable[ DSTOREPLUG_START_READING ] ) )
        {
                UnloadModuleDynamicly( _sohandle );
                memset( _fptable, NULL, sizeof(void*) * DSTOREPLUG_LASTFPTR );
                _sohandle = NULL;
                
                DEBUGOUTPUT( "Invalid codec module" );
                return;        
        }
        
        /*
         *      Intialize the plugin module
         */
        ( (TDSTOREPLUGFPTR_Init) _fptable[ DSTOREPLUG_INIT ] )( &_plugdata );
        
        tsprintf( "DStoreCodec plugin initialized\n" );
        tsprintf( "  - Name: %s\n", GetName().C_String() );
        tsprintf( "  - Copyright/EULA: %s\n", GetCopyright().C_String() );        
}

/*-------------------------------------------------------------------------*/

CDStoreCodecPlugin::~CDStoreCodecPlugin()
{TRACE;        
        if ( _sohandle )
        {
                ( (TDSTOREPLUGFPTR_Shutdown) _fptable[ DSTOREPLUG_SHUTDOWN ] )( &_plugdata );
                UnloadModuleDynamicly( _sohandle );
        }
}

/*-------------------------------------------------------------------------*/

CDStoreCodecPlugin&
CDStoreCodecPlugin::operator=( const CDStoreCodecPlugin& src )
{TRACE;
        /* dummy, don't use */
        return *this;
}

/*-------------------------------------------------------------------------*/      

bool 
CDStoreCodecPlugin::IsValid( void ) const
{TRACE;       
        return _sohandle != NULL;
}

/*-------------------------------------------------------------------------*/

void
CDStoreCodecPlugin::StoreNode( const CDataNode* n ,
                               void** filedata    )
{TRACE;
        UInt32 count( n->GetAttCount() );
        const char* name( n->GetName().C_String() );
        const CDataNode::TNodeAtt* att;     
        
        /*
         *      Begin storing the node
         */
        ((TDSTOREPLUGFPTR_Begin_Node_Store)_fptable[ DSTOREPLUG_BEGIN_NODE_STORE ])( &_plugdata       , 
                                                                                     filedata         , 
                                                                                     name             , 
                                                                                     count            , 
                                                                                     n->HasChildren() );
                                                                                     
        /*
         *      Store all node attributes
         */                
        for ( UInt32 i=0; i<count; ++i )
        {
                att = n->GetAttribute( i );
                ((TDSTOREPLUGFPTR_Store_Node_Att)_fptable[ DSTOREPLUG_STORE_NODE_ATT ])( &_plugdata            , 
                                                                                         filedata              ,   
                                                                                         name                  ,  
                                                                                         count                 ,
                                                                                         i                     ,
                                                                                         att->name.C_String()  , 
                                                                                         att->value.C_String() ,
                                                                                         n->HasChildren()      );        
        }
        
        /*
         *      If the node has any children we will store them and their children ect.
         */
        if ( n->HasChildren() )
        { 
                ((TDSTOREPLUGFPTR_Begin_Node_Children)_fptable[ DSTOREPLUG_BEGIN_NODE_CHILDREN ])( &_plugdata , 
                                                                                                   filedata   ,
                                                                                                   name       );

                /*
                 *      Iterate the child node recursively storing each node level
                 */                                                                                              
                CDataNode::const_iterator i = n->ConstBegin();
                while ( i != n->ConstEnd() )
                {
                        StoreNode( (*i), filedata );
                        ++i;
                }
                                                                                                              
                ((TDSTOREPLUGFPTR_End_Node_Children)_fptable[ DSTOREPLUG_END_NODE_CHILDREN ])( &_plugdata , 
                                                                                               filedata   ,
                                                                                               name       );                                                                                                                        
        }
        
        /*
         *      We are finished with this node
         */
        ((TDSTOREPLUGFPTR_End_Node_Store)_fptable[ DSTOREPLUG_END_NODE_STORE ])( &_plugdata       , 
                                                                                 filedata         , 
                                                                                 name             ,
                                                                                 count            ,
                                                                                 n->HasChildren() );                                                                                                                                                                                               
}

/*-------------------------------------------------------------------------*/ 
             
bool 
CDStoreCodecPlugin::StoreDataTree( const CDataNode* tree   ,
                                   const CString& filename )
{TRACE;
        
    CFileAccess access( filename );
    if ( access.IsValid() )
    {
        return StoreDataTree( tree     ,
                              &access  );
    }
    return false;
}                                   

/*-------------------------------------------------------------------------*/

bool 
CDStoreCodecPlugin::StoreDataTree( const CDataNode* tree   ,
                                   CIOAccess* file         )
{TRACE;

    if ( !_sohandle ) 
    { 
            return false; 
    }
    
    /*
     *      Open the destination file
     */
    void* filedata;       
    if ( !((TDSTOREPLUGFPTR_Dest_File_Open)_fptable[ DSTOREPLUG_DEST_FILE_OPEN ])( &_plugdata, &filedata, file->CStyleAccess() ) ) 
    {  
            return false; 
    }
    
    /*
     *      Recursively traverse the tree storing each node and sub-triers as we go
     */
    StoreNode( tree      ,
               &filedata ); 
    
    /*
     *      We are finished,.. close the file
     */
    ((TDSTOREPLUGFPTR_Dest_File_Close)_fptable[ DSTOREPLUG_DEST_FILE_CLOSE ])( &_plugdata, &filedata );
    return true;
}
                                    
/*-------------------------------------------------------------------------*/

bool
CDStoreCodecPlugin::BuildDataTree( CDataNode* treeroot     ,
                                   const CString& filename )
                                   
{TRACE;       
        CFileAccess access( filename );
        if ( access.IsValid() )
        {
                return BuildDataTree( treeroot ,
                                      &access  );
        }
        return false;
}

/*-------------------------------------------------------------------------*/               
                                                                   
bool 
CDStoreCodecPlugin::BuildDataTree( CDataNode* treeroot ,
                                   CIOAccess* file     )
{TRACE;

        if ( !_sohandle ) 
        {  
                return false; 
        }
        
        /*
         *      Open the source file
         */
        void* filedata;       
        if ( !((TDSTOREPLUGFPTR_Src_File_Open)_fptable[ DSTOREPLUG_SRC_FILE_OPEN ])( &_plugdata, &filedata, file->CStyleAccess() ) ) 
        { 
                return false; 
        }

        /*
         *      Hook up our read event handlers
         */      
        void* privdata = new TParserData; 
        static_cast<TParserData*>(privdata)->root = treeroot;             
        TReadHandlers rhandlers;
        rhandlers.OnTreeBegin = OnTreeBeginHandler;
        rhandlers.OnTreeEnd = OnTreeEndHandler;
        rhandlers.OnNodeBegin = OnNodeBeginHandler;
        rhandlers.OnNodeEnd = OnNodeEndHandler;        
        rhandlers.OnNodeAtt = OnNodeAttHandler;
        rhandlers.OnNodeChildrenBegin = OnNodeChildrenBeginHandler;
        rhandlers.OnNodeChildrenEnd = OnNodeChildrenEndHandler;
        rhandlers.OnError = OnParserErrorHandler;
        ((TDSTOREPLUGFPTR_Set_Read_Handlers)_fptable[ DSTOREPLUG_SET_READ_HANDLERS ])( &_plugdata, &filedata, &rhandlers, privdata );                
        
        /*
         *      So far so good,.. delete whatever children the treeroot may alreay have
         *      and start reading the data. This should cause the handlers to be called when
         *      needed and they in turn construct our tree based on the events.
         */
        treeroot->DelSubTree(); 
        ((TDSTOREPLUGFPTR_Start_Reading)_fptable[ DSTOREPLUG_START_READING ])( &_plugdata, &filedata );
        delete static_cast<TParserData*>(privdata);
        
        /*
         *      We are finished,.. close the file
         */
        ((TDSTOREPLUGFPTR_Src_File_Close)_fptable[ DSTOREPLUG_SRC_FILE_CLOSE ])( &_plugdata, &filedata );        
        return true;
}

/*-------------------------------------------------------------------------*/

CString 
CDStoreCodecPlugin::GetTypeName( void ) const
{TRACE;
        return ((TDSTOREPLUGFPTR_Type)_fptable[ DSTOREPLUG_TYPE ])( _plugdata );
}
        
/*-------------------------------------------------------------------------*/        
        
CString 
CDStoreCodecPlugin::GetName( void ) const
{TRACE;
        return ((TDSTOREPLUGFPTR_Name)_fptable[ DSTOREPLUG_NAME ])( _plugdata );        
}
        
/*-------------------------------------------------------------------------*/        
        
CString 
CDStoreCodecPlugin::GetCopyright( void ) const
{TRACE;
        return ((TDSTOREPLUGFPTR_Copyright)_fptable[ DSTOREPLUG_COPYRIGHT ])( _plugdata );        
}

/*-------------------------------------------------------------------------*/

const TVersion* 
CDStoreCodecPlugin::GetVersion( void ) const
{TRACE;
        return ((TDSTOREPLUGFPTR_Version)_fptable[ DSTOREPLUG_VERSION ])( _plugdata );
}

/*-------------------------------------------------------------------------*/

void 
CDStoreCodecPlugin::SetPluginID( UInt32 pluginid )
{TRACE;        
        _id = pluginid;
}

/*-------------------------------------------------------------------------*/

UInt32
CDStoreCodecPlugin::GetPluginID( void ) const
{TRACE;
        return _id;
}

/*-------------------------------------------------------------------------*/

CString
CDStoreCodecPlugin::GetLocation( void ) const
{TRACE;
        return _pluginfile;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

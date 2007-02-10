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

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#include "CIOAccess.h"

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
fa_open( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        ( (CIOAccess*) access->privdata )->Open();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
fa_opened( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CIOAccess*) access->privdata )->Opened();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX
fa_close( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        ( (CIOAccess*) access->privdata )->Close();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
fa_readl( struct SIOAccess* access , 
          char **dest              ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        CString str = ( (CIOAccess*) access->privdata )->ReadLine();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );
        return str.Length();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
fa_reads( struct SIOAccess* access , 
          char **dest              ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        CString str = ( (CIOAccess*) access->privdata )->ReadString();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );        
        return str.Length();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX
fa_write( struct SIOAccess* access , 
          const void *srcData      , 
          UInt32 esize             , 
          UInt32 elements          ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CIOAccess*) access->privdata )->Write( srcData  ,
                                                         esize    ,
                                                         elements );
}

/*-------------------------------------------------------------------------*/
           
UInt32 GUCEF_CALLSPEC_PREFIX
fa_read( struct SIOAccess* access , 
         void *dest               , 
         UInt32 esize             , 
         UInt32 elements          ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CIOAccess*) access->privdata )->Read( dest     ,
                                                        esize    ,
                                                        elements );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX
fa_tell( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;

        return ( (CIOAccess*) access->privdata )->Tell();
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
fa_seek( struct SIOAccess* access , 
         UInt32 offset            , 
         Int32 origin             ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CIOAccess*) access->privdata )->Seek( offset ,
                                                        origin );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX 
fa_setpos( struct SIOAccess* access , 
           UInt32 pos               ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CIOAccess*) access->privdata )->Setpos( pos );
}

/*-------------------------------------------------------------------------*/
            
Int32 GUCEF_CALLSPEC_PREFIX 
fa_getc( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CIOAccess*) access->privdata )->GetChar();
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
fa_eof( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CIOAccess*) access->privdata )->Eof();
}

/*-------------------------------------------------------------------------*/

void GUCEF_CALLSPEC_PREFIX 
fa_free( void* mem ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        delete []((char*)mem);
}

/*-------------------------------------------------------------------------*/

CIOAccess::CIOAccess( void )
{TRACE;

    LinkCStyleAccess();
}

/*-------------------------------------------------------------------------*/

CIOAccess::CIOAccess( const CIOAccess& src )
{TRACE;

    LinkCStyleAccess();
}

/*-------------------------------------------------------------------------*/

CIOAccess::~CIOAccess()
{TRACE;

    memset( &m_cStyleAccess, 0, sizeof( TIOAccess ) );
}

/*-------------------------------------------------------------------------*/

CIOAccess& 
CIOAccess::operator=( const CIOAccess& src )
{
    if ( this != &src )
    {

    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CIOAccess::LinkCStyleAccess( void )
{TRACE;

    memset( &m_cStyleAccess, 0, sizeof( TIOAccess ) );
    
    m_cStyleAccess.close = fa_close;
    m_cStyleAccess.open = fa_open;
    m_cStyleAccess.write = fa_write;
    m_cStyleAccess.close = fa_close;                       
    m_cStyleAccess.opened = fa_opened;
    m_cStyleAccess.readl = fa_readl;
    m_cStyleAccess.reads = fa_reads;
    m_cStyleAccess.read = fa_read;
    m_cStyleAccess.tell = fa_tell;
    m_cStyleAccess.seek = fa_seek;
    m_cStyleAccess.setpos = fa_setpos;
    m_cStyleAccess.getc = fa_getc;
    m_cStyleAccess.eof = fa_eof;
    m_cStyleAccess.memfree = fa_free;
    m_cStyleAccess.privdata = this;
}

/*-------------------------------------------------------------------------*/

UInt32 
CIOAccess::ReadUntill( void *dest            ,
                       UInt32 bsize          ,
                       const void* delimiter ,
                       UInt32 delimsize      )
{
        //DEBUGOUTPUT( "CIOAccess::ReadUntill()" );
        
        GUCEF_BEGIN;
        if ( !bsize )
        {
                GUCEF_END;
                return 0;
        }        
        if ( !delimsize )
        {
                GUCEF_END_RET( UInt32, Read( dest  ,
                                                bsize ,
                                                1     ) );                                                 
        }
                
        char* buffer = new char[ delimsize ];
        char* bufwritepos = buffer;
        char* destbuf = static_cast<char*>( dest );
        UInt32 i;     
        for ( i=0; i<bsize; ++i )
        {
                if ( !Eof() )
                {                    
                        /*
                         *      Add a byte to our read buffer
                         */    
                        if ( bufwritepos - buffer < (Int32)delimsize-1 ) 
                        {
                                *bufwritepos = GetChar();
                                ++bufwritepos;
                        }
                        else
                        {
                                for ( UInt32 n=0; n<delimsize-1; ++n )
                                {
                                        buffer[ n ] = buffer[ n+1 ];                                        
                                }
                                *bufwritepos = GetChar();
                        }
                        
                        /*
                         *      Have we reached the delimiter ?
                         */
                        if ( memcmp( buffer, delimiter, delimsize ) == 0 )
                        {
                                delete []buffer;
                                destbuf[ i ] = '\0';
                                GUCEF_END;
                                return i;
                                
                        }
                        destbuf[ i ] = *bufwritepos;     
                }
                else
                {
                        delete []buffer;
                        destbuf[ i ] = '\0';
                        GUCEF_END;
                        return i;
                }                                        
        }
        delete []buffer;
        destbuf[ bsize-i ] = '\0';        
        GUCEF_END;
        return i;          
}

/*-------------------------------------------------------------------------*/

/**
 *      Skips bytes untill the delimiter is reached or
 *      untill the end of the file is reached.
 *      the actual number of bytes skipped is returned.
 *
 *      @param delimiter the delimiter bytes
 *      @param delimsize size of the delimiter segment
 *      @return the actual number of bytes skipped.
 */                           
UInt32 
CIOAccess::SkipUntill( const void* delimiter ,
                       UInt32 delimsize      )
{
        //DEBUGOUTPUT( "CIOAccess::SkipUntill()" );
        
        GUCEF_BEGIN;       
        if ( !delimsize )
        {
                Seek( 0, SEEK_END );                                             
        }
                
        char* buffer = new char[ delimsize ];
        char* bufwritepos = buffer;
        UInt32 pos = Tell();
        while( !Eof() )
        {                    
                /*
                 *      Add a byte to our read buffer
                 */    
                if ( bufwritepos - buffer < (Int32)delimsize-1 ) 
                {
                        *bufwritepos = GetChar();
                        ++bufwritepos;
                }
                else
                {
                        for ( UInt32 n=0; n<delimsize-1; ++n )
                        {
                                buffer[ n ] = buffer[ n+1 ];                                        
                        }
                        *bufwritepos = GetChar();
                }
                
                /*
                 *      Have we reached the delimiter ?
                 */
                if ( memcmp( buffer, delimiter, delimsize ) == 0 )
                {
                        delete []buffer;
                        GUCEF_END_RET( UInt32, Tell() - pos );
                }    
        }
        delete []buffer;
        GUCEF_END_RET( UInt32, Tell() - pos );                                            
}                       

/*-------------------------------------------------------------------------*/

UInt32
CIOAccess::Read( CDynamicBuffer& dest ,
                 UInt32 esize         ,
                 UInt32 elements      )
{TRACE;

    // First we ensure that we have enough space in the buffer to copy our data 
    // into the buffer manually
    dest.SetBufferSize( dest.GetDataSize() + ( esize * elements ) ,
                        false                                     );
                        
    // Perform the actual read
    UInt32 nrOfBytesRead = Read( static_cast< UInt8* >( dest.GetBufferPtr() ) + dest.GetDataSize() ,
                                 esize                                                             ,
                                 elements                                                          );
                                 
    // Correct the data delimiter in the buffer
    dest.SetDataSize( dest.GetDataSize() + nrOfBytesRead );
    
    return nrOfBytesRead;
}

/*-------------------------------------------------------------------------*/

TIOAccess*
CIOAccess::CStyleAccess( void )
{TRACE;

    return &m_cStyleAccess;
}

/*-------------------------------------------------------------------------*/

UInt32
CIOAccess::Write( CIOAccess& sourceData )
{TRACE;

    UInt32 byteCount=0, totalByteCount=0;
    CDynamicBuffer readBuffer( 1024, true );
    readBuffer.SetDataSize( 1024 );
    
    while ( !sourceData.Eof() )
    {
        byteCount = sourceData.Read( readBuffer.GetBufferPtr(), 1, 1024 );
        Write( readBuffer.GetBufferPtr(), byteCount, 1 );
        
        totalByteCount += byteCount;
    }
    
    return totalByteCount;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

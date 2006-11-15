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

CIOAccess::CIOAccess( void )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CIOAccess::CIOAccess( const CIOAccess& src )
{
        GUCEF_BEGIN;
        /* dummy, doesnt do anything */
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CIOAccess::~CIOAccess()
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CIOAccess& 
CIOAccess::operator=( const CIOAccess& src )
{
        GUCEF_BEGIN;
        /* dummy, doesnt do anything */
        GUCEF_END;
        return *this;
}

/*-------------------------------------------------------------------------*/

/**
 *      reads data untill the specified delimiter is reached.
 *      The data is written into the destination buffer untill the
 *      delimiter is reached or the end of the file is reached.
 *      The delimiter itself is not written to the destination buffer
 *
 *      @param dest the destination buffer for the data
 *      @param bsize size of the destination buffer
 *      @param delimiter data segment delimiter
 *      @param size of the data segment delimiter
 *      @return number of bytes written into dest
 */
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
    UInt32 nrOfBytesRead = Read( dest.GetBufferPtr() + dest.GetDataSize() ,
                                 esize                                    ,
                                 elements                                 );
                                 
    // Correct the data delimiter in the buffer
    dest.SetDataSize( dest.GetDataSize() + nrOfBytesRead );
    
    return nrOfBytesRead;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

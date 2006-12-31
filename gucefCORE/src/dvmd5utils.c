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

#include <stdio.h>
#include <string.h>

#ifndef md5_INCLUDED
#include "md5/md5.h"	     /* MD5 utilities */
#endif /* md5_INCLUDED ? */

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"	     /* function prototypes */
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace CORE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *	Function that returns an MD5 digest generated from the file given
 *	Returns 1 on success and 0 on failure.
 */
UInt32
md5frommfile( TIOAccess *access  ,
              UInt8 digest[ 16 ] )
{
        md5_state_t md5state;                  /* md5 generation info */
        UInt32 rbytes;
        UInt8 buffer[ 1024 ];

        memset( digest, 0, 16 );
        md5_init( &md5state );

        do
        {
                rbytes = access->read( access, buffer, 1, 1024 );
                if ( rbytes > 0 )
                {
                    md5_append( &md5state, buffer, rbytes );
                }
                
        } while ( rbytes );

        /*
         *	Finish up
         */
        md5_finish( &md5state, digest );
        return 1;
}

/*-------------------------------------------------------------------------*/

/**
 *	Converts an md5 digest into a string 48 chars long.
 *	Each digest value is converted to 3 characters.
 */
void
md5tostring( const UInt8 digest[ 16 ] ,
             char md5_str[ 48 ]       )
{
        char tmp_str[ 4 ];
        UInt32 tmp, i;
        tmp_str[ 3 ] = '\0';
        for ( i=0; i<16; i++ )
        {
                tmp = digest[ i ];
                sprintf( tmp_str, "%d", tmp );
                if ( tmp > 99 )
                {
                        strncpy( md5_str+(i*3), tmp_str, 3 );
                }
                else
                if ( tmp > 9 )
                {
                        *(md5_str+(i*3)) = '0';
                        strncpy( md5_str+(i*3)+1, tmp_str, 2 );
                }
                else
                {
                        *(md5_str+(i*3)) = '0';
                        *(md5_str+(i*3)+1) = '0';
                        strncpy( md5_str+(i*3)+2, tmp_str, 1 );
                }
        }
}

/*-------------------------------------------------------------------------*/

/**
 *	Converts the given null-terminated string 
 *      into an md5 digest 
 */
void
md5fromstring( const char* string ,
               Ubyte digest[ 16 ] )
{
        md5_state_t md5state;                  /* md5 generation info */

        memset( digest, 0, 16 );
        md5_init( &md5state );
        md5_append( &md5state, (const Ubyte*)string, (int)strlen( string ) );
        md5_finish( &md5state, digest );
}                   

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/




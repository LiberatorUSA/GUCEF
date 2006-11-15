/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef GUCEF_CORE_CIOACCESS_H
#define GUCEF_CORE_CIOACCESS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"          /* GUCEF string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_IOACCESS_H
#include "ioaccess.h"           /* C implementation wrapped by this class */
#define GUCEF_CORE_IOACCESS_H
#endif /* GUCEF_CORE_IOACCESS_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

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

class CDynamicBuffer;

/*-------------------------------------------------------------------------*/

/**
 *      Abstract base class for media independant recource access
 */
class GUCEFCORE_EXPORT_CPP CIOAccess : public CICloneable
{
        public:
        
        CIOAccess( void );
        
        CIOAccess( const CIOAccess& src );

        virtual ~CIOAccess();
        
        CIOAccess& operator=( const CIOAccess& src );

        /**
         *      open the resource for I/O
         */
        virtual void Open( void ) = 0;

        /**
         *      close the recource
         */
        virtual void Close( void ) = 0;

        /**
         *      is the recource opened for reading ?
         */
        virtual bool Opened( void ) const = 0;

        /**
         *      read a line of text
         *      The delimiter is the platform text file line
         *      delimiter.
         */
        virtual CString ReadLine( void ) = 0;
                
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
        UInt32 ReadUntill( void *dest            ,
                           UInt32 bsize          ,
                           const void* delimiter ,
                           UInt32 delimsize      );
                           
        /**
         *      Skips bytes untill the delimiter is reached or
         *      untill the end of the file is reached.
         *      the actual number of bytes skipped is returned.
         *
         *      @param delimiter the delimiter bytes
         *      @param delimsize size of the delimiter segment
         *      @return the actual number of bytes skipped.
         */                           
        UInt32 SkipUntill( const void* delimiter ,
                           UInt32 delimsize      );                                   

        /**
         *      Reads a string from the recource
         */
        virtual CString ReadString( void ) = 0;

        /**
         *      Attempts to read the specified number of bytes from the recourse
         */
        virtual UInt32 Read( void *dest      ,
                             UInt32 esize    ,
                             UInt32 elements ) = 0;

        UInt32 Read( CDynamicBuffer& dest ,
                     UInt32 esize         ,
                     UInt32 elements      );
                             
        /**
         *      Attempts to write the specified number of bytes to the recourse
         *      using srcdata as the data source.
         */        
        virtual UInt32 Write( const void* srcdata ,
                              UInt32 esize        ,
                              UInt32 elements     ) = 0;

        /**
         *      Get the current offset in bytes
         */
        virtual UInt32 Tell( void ) const = 0;

        /**
         *      jump to a different part of the recource
         */
        virtual Int32 Seek( UInt32 offset ,
                            Int32 origin  ) = 0;

        /**
         *      jump to the given offset in the recource
         */
        virtual UInt32 Setpos( UInt32 position ) = 0;
        
        /**
         *      returns the size of the recource in bytes
         */
        virtual UInt32 GetSize( void ) const = 0;

        /**
         *      Read a single character
         */
        virtual char GetChar( void ) = 0;

        /**
         *      are we at the end of the recource ?
         */
        virtual bool Eof( void ) const = 0;
        
        /**
         *      Is the only allowed access method reading ?
         *      If true the recource cannot be written to
         */
        virtual bool IsReadOnly( void ) const = 0;
        
        /**
         *      Is the only allowed access method writing ?
         *      if true the recource cannot be read.
         */
        virtual bool IsWriteOnly( void ) const = 0;
        
        /**
         *      Are both read AND write operations possible on the recource ?         
         */
        virtual bool IsReadAndWrite( void ) const = 0;                
        
        /**
         *      Is the access to the resource a valid one or
         *      has something gone wrong ?
         */
        virtual bool IsValid( void ) = 0;
        
        /**
         *  @return returns the size of the recource if possible. returns -1 if the size cannot be determined
         */
        virtual Int32 GetSize( void ) const = 0;

        virtual TIOAccess* CStyleAccess( void ) = 0;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIOACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 01-05-2005 :
        - Added ReadUntill()
          This member function will read data untill the given delimiter is 
          reached or the end of the file is reached.
- 25-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
 
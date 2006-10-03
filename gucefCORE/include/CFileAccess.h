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

#ifndef GUCEF_CORE_CFILEACCESS_H
#define GUCEF_CORE_CFILEACCESS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <stdio.h>

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"        /* base class for all media manipulators */
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

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

class GUCEFCORE_EXPORT_CPP CFileAccess : public CIOAccess
{
        public:
        
        CFileAccess( const CString& file     ,
                     const char* mode = "rb" );

        virtual ~CFileAccess();

        /**
         *      open the resource for I/O
         */
        virtual void Open( void );

        /**
         *      close the recource
         */
        virtual void Close( void );

        /**
         *      is the recource opened for reading ?
         */
        virtual bool Opened( void ) const;

        /**
         *      read a line of text
         */
        virtual CString ReadLine( void );

        /**
         *      Reads a string from the recource
         */
        virtual CString ReadString( void );

        /**
         *      Attempts to read the specified number of bytes from the recourse
         */
        virtual UInt32 Read( void *dest      ,
                             UInt32 esize    ,
                             UInt32 elements );
                             
        /**
         *      Attempts to write the specified number of bytes to the recourse
         *      using srcdata as the data source.
         */        
        virtual UInt32 Write( const void* srcdata ,
                              UInt32 esize        ,
                              UInt32 elements     );                             

        /**
         *      Get the current offset in bytes
         */
        virtual UInt32 Tell( void ) const;
        
        /**
         *      Get the size of the recource in bytes
         */        
        virtual UInt32 GetSize( void ) const;

        /**
         *      jump to a different part of the recource
         */
        virtual Int32 Seek( UInt32 offset ,
                            Int32 origin  );

        /**
         *      jump to the given offset in the recource
         */
        virtual UInt32 Setpos( UInt32 position );

        /**
         *      Read a single character
         */
        virtual char GetChar( void );

        /**
         *      are we at the end of the recource ?
         */
        virtual bool Eof( void ) const;
        
        virtual bool IsReadOnly( void ) const;
        
        /**
         *      Is the only allowed access method writing ?
         *      if true the recource cannot be read.
         */
        virtual bool IsWriteOnly( void ) const;
        
        /**
         *      Are both read AND write operations possible on the recource ?         
         */
        virtual bool IsReadAndWrite( void ) const;        
        
        /**
         *      Is the access to the resource a valid one or
         *      has something gone wrong ?
         */
        virtual bool IsValid( void );

        virtual TIOAccess* CStyleAccess( void );
        
        virtual CICloneable* Clone( void ) const;
        
        private:
        CFileAccess( void );

        bool _writeable;
        bool _readable;
        CString m_mode;        
        FILE* _file;
        TIOAccess _access;
        CString _filename;
        UInt32 _size;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CFILEACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 25-08-2005 :
        - Replaced the read/write bools with a const char* for a mode.
          This allows for more options plus a more fopen() like parameter list
- 26-04-2005 :
        - Fixed an bug in Close(): Calling Close() twice caused an access violation
- 26-03-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/

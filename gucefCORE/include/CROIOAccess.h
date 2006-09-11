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

#ifndef GUCEF_CORE_CROIOACCESS_H
#define GUCEF_CORE_CROIOACCESS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"          /* abstract base for IO Access */
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

/**
 *      Abstract base class for read-only media independant recource access
 *      Read-only media access implementations can inherit from this class
 *      to avoid having to implement the write interface for each decending
 *      implementation.
 */
class EXPORT_CPP CROIOAccess : public CIOAccess
{
        public:
        
        CROIOAccess( void );       

        virtual ~CROIOAccess();
        
        /**
         *      Is the only allowed access method reading ?
         *      If true the recource cannot be written to         
         *      Always returns true
         */
        virtual bool IsReadOnly( void ) const;
        
        /**
         *      Is the only allowed access method writing ?
         *      if true the recource cannot be read.
         *      Always returns false
         */
        virtual bool IsWriteOnly( void ) const;
        
        /**
         *      Are both read AND write operations possible on the recource ?         
         *      Always returns false
         */
        virtual bool IsReadAndWrite( void ) const;
        
        /**
         *      Attempts to write the specified number of bytes to the recourse
         *      using srcdata as the data source.
         *      Always returns 0, no actual write operation will be performed.
         */        
        virtual UInt32 Write( const void* srcdata ,
                              UInt32 esize        ,
                              UInt32 elements     );
                              
        protected:
        
        TIOAccess _access;                                                             
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CROIOACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-05-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/

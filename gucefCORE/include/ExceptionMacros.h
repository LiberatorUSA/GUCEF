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

#ifndef GUCEF_CORE_EXCEPTIONMACROS_H
#define GUCEF_CORE_EXCEPTIONMACROS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_macros.h"
#include "CException.h"
#include "CMsgException.h"
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  Macro for defining an exception class. Usefull for defining an exception
 *  class within the scope of your class definition. This makes exceptions 
 *  easy to find instead of hidden away in comment remarks but without 
 *  mucking up your class definition readability with exception definitions
 */
#define GUCEF_DEFINE_EXCEPTION( exportAPI, exceptionName )         \
                                                                   \
    class exportAPI exceptionName : public GUCEF::CORE::CException \
    {                                                              \
        public:                                                    \
        exceptionName( const char* originFile ,                    \
                       const int originLineNr );                   \
                                                                   \
        exceptionName( const exceptionName& src );                 \
                                                                   \
        virtual ~exceptionName();                                  \
                                                                   \
        private:                                                   \
        exceptionName& operator=( const exceptionName& src );      \
    };

/*--------------------------------------------------------------------------*/

/*
 *  Macro for defining a message exception class. 
 *  Usefull for defining an exception class within the scope of 
 *  your class definition. This makes exceptions 
 *  easy to find instead of hidden away in comment remarks but without 
 *  mucking up your class definition readability with exception definitions
 */
#define GUCEF_DEFINE_MSGEXCEPTION( exportAPI, exceptionName )         \
                                                                      \
    class exportAPI exceptionName : public GUCEF::CORE::CMsgException \
    {                                                                 \
        public:                                                       \
        exceptionName( const char* originFile ,                       \
                       const int originLineNr ,                       \
                       const char* errorMsg   );                      \
                                                                      \
        exceptionName( const exceptionName& src );                    \
                                                                      \
        virtual ~exceptionName();                                     \
                                                                      \
        private:                                                      \
        exceptionName( void );                                        \
        const exceptionName& operator=(const exceptionName&);         \
    };

/*--------------------------------------------------------------------------*/

/*
 *  Macro for defining a message exception class inlined
 *  This makes exceptions easy to find instead of hidden away in comment 
 *  remarks but without mucking up your class definition readability 
 *  with exception definitions
 */
#define GUCEF_DEFINE_INLINED_MSGEXCEPTION( exceptionName )               \
                                                                         \
    class exceptionName : public GUCEF::CORE::CMsgException              \
    {                                                                    \
        public:                                                          \
        exceptionName( const char* originFile ,                          \
                       const int originLineNr ,                          \
                       const char* errorMsg   )                          \
                                                                         \
            : GUCEF::CORE::CMsgException( originFile   ,                 \
                                          originLineNr ,                 \
                                          errorMsg     )                 \
        {                                                                \
                                                                         \
        }                                                                \
                                                                         \
        exceptionName( const exceptionName& src )                        \
            : GUCEF::CORE::CMsgException( src )                          \
        {                                                                \
                                                                         \
        }                                                                \
                                                                         \
        virtual ~exceptionName()                                         \
        {                                                                \
                                                                         \
        }                                                                \
                                                                         \
        private:                                                         \
                                                                         \
        exceptionName( void );                                           \
                                                                         \
        const exceptionName& operator=( const exceptionName& src );      \
    };

/*--------------------------------------------------------------------------*/

/*
 *  macro for implementing an exception class within the scope of 
 *  your class. This allows you to use per class typed exceptions.
 *  The macro will help to implement a number of exceptions classes
 *  with ease and consistancy
 */
#define GUCEF_IMPLEMENT_EXCEPTION( ownerScopeClass, exceptionName )         \
                                                                            \
ownerScopeClass::exceptionName::exceptionName( const char* originFile ,     \
                                               const int originLineNr )     \
    : GUCEF::CORE::CException( originFile   ,                               \
                               originLineNr )                               \
{                                                                           \
}                                                                           \
                                                                            \
ownerScopeClass::exceptionName::exceptionName( const exceptionName& src )   \
    : GUCEF::CORE::CException( src )                                        \
{                                                                           \
}                                                                           \
                                                                            \
ownerScopeClass::exceptionName::~exceptionName()                            \
{                                                                           \
}

/*--------------------------------------------------------------------------*/

/*
 *  macro for implementing an exception class within the scope of 
 *  your class. This allows you to use per class typed exceptions.
 *  The macro will help to implement a number of exceptions classes
 *  with ease and consistancy
 */
#define GUCEF_IMPLEMENT_MSGEXCEPTION( ownerScopeClass, exceptionName )      \
                                                                            \
ownerScopeClass::exceptionName::exceptionName( const char* originFile ,     \
                                               const int originLineNr ,     \
                                               const char* errorMsg   )     \
    : GUCEF::CORE::CMsgException( originFile   ,                            \
                               originLineNr ,                               \
                               errorMsg     )                               \
{                                                                           \
}                                                                           \
                                                                            \
ownerScopeClass::exceptionName::exceptionName( const exceptionName& src )   \
    : GUCEF::CORE::CMsgException( src )                                     \
{                                                                           \
}                                                                           \
                                                                            \
ownerScopeClass::exceptionName::~exceptionName()                            \
{                                                                           \
}

/*--------------------------------------------------------------------------*/

#undef GUCEF_ETHROW
#define GUCEF_ETHROW( exceptionName )  {throw exceptionName( __FILE__, __LINE__ );}

/*--------------------------------------------------------------------------*/

#undef GUCEF_EMSGTHROW
#define GUCEF_EMSGTHROW( exceptionName, errorMsg ) {throw exceptionName( __FILE__, __LINE__, (errorMsg) );}

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_EXCEPTIONMACROS_H ? */

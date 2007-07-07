    /*
     *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
     *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
     *
     *  This library is free software; you can redistribute it and/or
     *  modify it under the terms of the GNU Lesser General Public
     *  License as published by the Free Software Foundation; either
     *  version 2.1 of the License, or (at your option) any later version.
     *
     *  This library is distributed in the hope that it will be useful,
     *  but WITHOUT ANY WARRANTY; without even the implied warranty of
     *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     *  Lesser General Public License for more details.
     *
     *  You should have received a copy of the GNU Lesser General Public
     *  License along with this library; if not, write to the Free Software
     *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
     */

    #ifndef GUCEF_CORE_CTSTREAMABLEOBJ_H
    #define GUCEF_CORE_CTSTREAMABLEOBJ_H

    /*-------------------------------------------------------------------------//
    //                                                                         //
    //      INCLUDES                                                           //
    //                                                                         //
    //-------------------------------------------------------------------------*/ 

    #include <string.h>                     /* needed for memcpy() */

    #ifndef GUCEF_CORE_CISTREAMABLE_H
    #include "CIStreamable.h"               /* interface class for streamable objects */
    #define GUCEF_CORE_CISTREAMABLE_H
    #endif /* GUCEF_CORE_CISTREAMABLE_H ? */

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
     *  Template that allows a streaming interface to be added to simple
     *  types with ease. Note that it should not be used for more complex types
     *  that contain pointers etc.
     */
    template < typename T >
    class CTStreamableObj : public CIStreamable
    {
        public:
        
        CTStreamableObj( void );
        
        CTStreamableObj( const T& src );
        
        CTStreamableObj( const CTStreamableObj& src );
        
        virtual ~CTStreamableObj();
        
        CTStreamableObj& operator=( const CTStreamableObj& src );
        
        operator T& ();
        
        operator const T& () const;
        
        virtual bool StreamTo( void* destBuffer            ,
                               const UInt32 destBufferSize ) const;
                        
        virtual bool StreamFrom( const void* srcBuffer      ,
                                 const UInt32 srcBufferSize );
        
        virtual UInt32 GetStreamedSize( void ) const;
        
        virtual bool StreamToBuffer( CDynamicBuffer& buffer ) const;
        
        virtual bool StreamFromBuffer( const CDynamicBuffer& buffer );
        
        private:
        
        T m_data;
    };

    /*-------------------------------------------------------------------------//
    //                                                                         //
    //      UTILITIES                                                          //
    //                                                                         //
    //-------------------------------------------------------------------------*/

    template < typename T >
    CTStreamableObj< T >::CTStreamableObj( void )
        : CIStreamable() ,
          m_data()
    {GUCEF_TRACE;

    }

    /*-------------------------------------------------------------------------*/

    template < typename T >
    CTStreamableObj< T >::CTStreamableObj( const T& src )
        : CIStreamable() ,
          m_data( src )
    {GUCEF_TRACE;

    }

    /*-------------------------------------------------------------------------*/

    template < typename T >
    CTStreamableObj< T >::CTStreamableObj( const CTStreamableObj& src )
        : CIStreamable( src )  ,
          m_data( src.m_data )
    {GUCEF_TRACE;

    }

    /*-------------------------------------------------------------------------*/

    template < typename T >
    CTStreamableObj< T >::~CTStreamableObj()
    {GUCEF_TRACE;
        
    }

    /*-------------------------------------------------------------------------*/

    template < typename T >
    CTStreamableObj< T >&
    CTStreamableObj< T >::operator=( const CTStreamableObj& src )
    {GUCEF_TRACE;

        if ( &src != this )
        {
            CIStreamable::operator=( src );
            
            m_data = src.m_data;
        }
        return *this;
    }

    /*-------------------------------------------------------------------------*/

    template < typename T >
    CTStreamableObj< T >::operator T& ()
    {GUCEF_TRACE;

        return m_data;
    }

    /*-------------------------------------------------------------------------*/

    template < typename T >
    CTStreamableObj< T >::operator const T& () const
    {GUCEF_TRACE;

        return m_data;
    }

    /*-------------------------------------------------------------------------*/

    template < typename T >
    bool
    CTStreamableObj< T >::StreamTo( void* destBuffer            ,
                                    const UInt32 destBufferSize ) const
    {GUCEF_TRACE;
        
        if ( destBufferSize >= sizeof( T ) )
        {
            memcpy( destBuffer, &m_data, sizeof( T ) );
            return true;
        }
        return false;
    }

    /*-------------------------------------------------------------------------*/
                        
    template < typename T >
    bool
    CTStreamableObj< T >::StreamFrom( const void* srcBuffer      ,
                                      const UInt32 srcBufferSize )
    {GUCEF_TRACE;

        if ( srcBufferSize >= sizeof( T ) )
        {
            memcpy( &m_data, srcBuffer, sizeof( T ) );
            return true;
        }
        return false;  
    }
        
    /*-------------------------------------------------------------------------*/

    template < typename T >
    UInt32
    CTStreamableObj< T >::GetStreamedSize( void ) const
    {GUCEF_TRACE;
        
        return sizeof( T );
    }

    /*-------------------------------------------------------------------------*/
        
    template < typename T >
    bool
    CTStreamableObj< T >::StreamToBuffer( CDynamicBuffer& buffer ) const
    {GUCEF_TRACE;

        buffer.Append( &m_data, sizeof( T ) );
        return true;
    }

    /*-------------------------------------------------------------------------*/

    template < typename T >
    bool
    CTStreamableObj< T >::StreamFromBuffer( const CDynamicBuffer& buffer )
    {GUCEF_TRACE;

        try
        {
            m_data = buffer.AsConstType< T >();
        }
        catch ( CDynamicBuffer::EIllegalCast& )
        {
            return false;
        }
        return true;
    }

    /*-------------------------------------------------------------------------//
    //                                                                         //
    //      NAMESPACE                                                          //
    //                                                                         //
    //-------------------------------------------------------------------------*/

    }; /* namespace CORE */
    }; /* namespace GUCEF */

    /*-------------------------------------------------------------------------*/

    #endif /* GUCEF_CORE_CTSTREAMABLEOBJ_H ? */

    /*-------------------------------------------------------------------------//
    //                                                                         //
    //      Info & Changes                                                     //
    //                                                                         //
    //-------------------------------------------------------------------------//

    - 02-03-2007 :
            - Dinand: re-added this class

    -----------------------------------------------------------------------------*/
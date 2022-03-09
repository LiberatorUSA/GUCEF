/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#ifndef GUCEF_CORE_CTDATANODESERIALIZABLEOBJ_H
#define GUCEF_CORE_CTDATANODESERIALIZABLEOBJ_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIDATANODESERIALIZABLE_H
#include "gucefCORE_CIDataNodeSerializable.h"
#define GUCEF_CORE_CIDATANODESERIALIZABLE_H
#endif /* GUCEF_CORE_CIDATANODESERIALIZABLE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TEMPLATES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Tenplate to quickly and easily add a CIDataNodeSerializable interface implementation
 *  to any class. Main intended use is to add the interface to build-in types
 */
template< class T > 
class CTDataNodeSerializableObj : public CIDataNodeSerializable
{
    public:

    CTDataNodeSerializableObj( void );

    CTDataNodeSerializableObj( const CTDataNodeSerializableObj& src );

    CTDataNodeSerializableObj( const T& src );
        
    virtual ~CTDataNodeSerializableObj();
    
    CTDataNodeSerializableObj& operator=( const CTDataNodeSerializableObj& src );

    CTDataNodeSerializableObj& operator=( const T& src );

    inline T GetDataCopy( void ) const;
    
    inline T& GetData( void );
    
    inline const T& GetData( void ) const;

    inline operator const T&() const;

    inline operator T&();
    
    /**
     *  Attempts to serialize the object to a DOM created out of DataNode objects
     */
    virtual bool Serialize( CDataNode& domRootNode                        ,
                            const CDataNodeSerializableSettings& settings ) const GUCEF_VIRTUAL_OVERRIDE;

    static bool DataNodeSerializableObj_Serialize( const T& value                                ,
                                                   CDataNode& domRootNode                        ,
                                                   const CDataNodeSerializableSettings& settings );
    
    /**
     *  Attempts to serialize the object to a DOM created out of DataNode objects
     *
     *  @param domRootNode Node that acts as root of the DOM data tree from which to deserialize
     *  @return whether deserializing the object data from the given DOM was successfull.
     */
    virtual bool Deserialize( const CDataNode& domRootNode                  ,
                              const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;


    static bool DataNodeSerializableObj_Deserialize( T& value                                      ,
                                                     const T& defaultValueIfNeeded                 ,
                                                     const CDataNode& domRootNode                  ,
                                                     const CDataNodeSerializableSettings& settings );

    protected:
    T m_data;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class T >
CTDataNodeSerializableObj< T >::CTDataNodeSerializableObj( void )
    : CIDataNodeSerializable()
    , m_data()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T >
CTDataNodeSerializableObj< T >::CTDataNodeSerializableObj( const CTDataNodeSerializableObj< T >& src )
    : CIDataNodeSerializable( src )
    , m_data( src.m_data )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T >
CTDataNodeSerializableObj< T >::CTDataNodeSerializableObj( const T& src )
    : CIDataNodeSerializable()
    , m_data( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< class T >
CTDataNodeSerializableObj< T >::~CTDataNodeSerializableObj()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< class T >
CTDataNodeSerializableObj< T >& 
CTDataNodeSerializableObj< T >::operator=( const CTDataNodeSerializableObj< T >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CIDataNodeSerializable::operator=( src );
        m_data = src.m_data;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
template< class T >
CTDataNodeSerializableObj< T >& 
CTDataNodeSerializableObj< T >::operator=( const T& src )
{GUCEF_TRACE;

    m_data = src;
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T >
T&
CTDataNodeSerializableObj< T >::GetData( void )
{GUCEF_TRACE;

    return m_data;
}
/*-------------------------------------------------------------------------*/

template< typename T >
const T& 
CTDataNodeSerializableObj< T >::GetData( void ) const
{GUCEF_TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------*/

template< typename T >
T 
CTDataNodeSerializableObj< T >::GetDataCopy( void ) const
{GUCEF_TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline
CTDataNodeSerializableObj< T >::operator const T&() const
{GUCEF_TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline
CTDataNodeSerializableObj< T >::operator T&()
{GUCEF_TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------*/
    
template< class T >
bool 
CTDataNodeSerializableObj< T >::DataNodeSerializableObj_Serialize( const T& value                                ,
                                                                   CDataNode& domRootNode                        ,
                                                                   const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    domRootNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/
    
template< class T >
bool 
CTDataNodeSerializableObj< T >::Serialize( CDataNode& domRootNode                        ,
                                           const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    // This type of simplistic serialization does not differ between levels of detail
    return DataNodeSerializableObj_Serialize( m_data, domRootNode, settings );
}

/*-------------------------------------------------------------------------*/
    
template< class T >
bool 
CTDataNodeSerializableObj< T >::DataNodeSerializableObj_Deserialize( T& newValue                                   ,
                                                                     const T& defaultValueIfNeeded                 ,
                                                                     const CDataNode& domRootNode                  ,
                                                                     const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    newValue = domRootNode.GetValue().AsTValue< T >( defaultValueIfNeeded );
    return true;
}

/*-------------------------------------------------------------------------*/
    
template< class T >
bool 
CTDataNodeSerializableObj< T >::Deserialize( const CDataNode& domRootNode                  ,
                                             const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    return DataNodeSerializableObj_Deserialize( m_data, m_data, domRootNode, settings );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTDATANODESERIALIZABLEOBJ_H ? */

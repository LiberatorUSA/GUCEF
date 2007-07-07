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

#ifndef GUCEF_CORE_CTSTRINGSTREAMABLE_H
#define GUCEF_CORE_CTSTRINGSTREAMABLE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CTSTREAMABLEOBJ_H
#include "CTStreamableObj.h"               /* Template for streamable objects */
#define GUCEF_CORE_CTSTREAMABLEOBJ_H
#endif /* GUCEF_CORE_CTSTREAMABLEOBJ_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

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
 *  Template specialization for a stream-able string
 */
template <>
class CTStreamableObj< CString > : public CIStreamable
{
    public:
    
    virtual bool StreamTo( void* destBuffer            ,
                           const UInt32 destBufferSize ) const
    {GUCEF_TRACE;
        
        if ( destBufferSize >= m_data.Length()+1 )
        {
            memcpy( destBuffer, m_data.C_String(), m_data.Length()+1 );
            return true;
        }
        return false;
    }                    
    
    virtual bool StreamFrom( const void* srcBuffer      ,
                             const UInt32 srcBufferSize )
    {GUCEF_TRACE;

        m_data.Scan( static_cast< const char* >( srcBuffer ), srcBufferSize );
        return true;
    }
    
    virtual UInt32 GetStreamedSize( void ) const
    {GUCEF_TRACE;
        
        return m_data.Length()+1;
    }
    
    virtual bool StreamToBuffer( CDynamicBuffer& buffer ) const
    {GUCEF_TRACE;

        buffer.Append( m_data.C_String(), m_data.Length()+1 );
    }
    
    virtual bool StreamFromBuffer( const CDynamicBuffer& buffer )
    {GUCEF_TRACE;

        m_data.Scan( static_cast< const char* >( buffer.GetConstBufferPtr() ), buffer.GetDataSize() );
        return true;
    }    
    
    private:
    
    CString m_data;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTSTRINGSTREAMABLE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-06-2007 :
        - Dinand: Added this class

-----------------------------------------------------------------------------*/
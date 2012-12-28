/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CSERIALPORT_H
#define GUCEF_COMCORE_CSERIALPORT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_COMCORE_CICOMMUNICATIONPORT_H
#include "gucefCOMCORE_CICommunicationPort.h"
#define GUCEF_COMCORE_CICOMMUNICATIONPORT_H
#endif /* GUCEF_COMCORE_CICOMMUNICATIONPORT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/


class GUCEF_COMCORE_EXPORT_CPP CSerialPort : public virtual CICommunicationPort
{
    public:
    
    virtual ~CSerialPort();
    
    virtual CORE::CString GetPortType( void ) const;

    virtual const CORE::CString& GetClassTypeName( void ) const;

    virtual bool IsOpen( void ) const = 0;

    virtual bool Open( void ) = 0;

    virtual void Close( void );

    virtual UInt8 ReadByte( void ) = 0;

    virtual char ReadChar( void ) = 0;

    virtual CORE::CString ReadString( const Int32 maxStringLength ) = 0;

    virtual CORE::CString ReadLine( const Int32 maxStringLength ) = 0;

    virtual UInt32 ReadBytesTo( CORE::CDynamicBuffer& buffer ,
                                const UInt32 elementSize     ,
                                const UInt32 elementsToRead  ) = 0;

    virtual UInt32 GetDataSizeInBytesInReceiveBuffer( void ) = 0;

    virtual void SetReadTimeout( const UInt32 timeOutInMilliSecs ) = 0;

    virtual UInt32 GetReadTimeout( void ) const = 0;
    
    virtual void SetWriteTimeout( const UInt32 timeOutInMilliSecs ) = 0;

    virtual UInt32 GetWriteTimeout( void ) const = 0;

    virtual bool SetBaudRate( const UInt32 baudRate ) = 0;

    virtual UInt32 GetBaudRate( void ) const = 0;

    protected:

    CSerialPort( void );
    CSerialPort( const CSerialPort& src );
    CSerialPort& operator=( const CSerialPort& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CSERIALPORT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

---------------------------------------------------------------------------*/

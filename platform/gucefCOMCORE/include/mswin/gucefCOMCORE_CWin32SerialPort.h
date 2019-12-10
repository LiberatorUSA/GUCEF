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

#ifndef GUCEF_COMCORE_CWIN32SERIALPORT_H
#define GUCEF_COMCORE_CWIN32SERIALPORT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CSERIALPORT_H
#include "gucefCOMCORE_CSerialPort.h"
#define GUCEF_COMCORE_CSERIALPORT_H
#endif /* GUCEF_COMCORE_CSERIALPORT_H ? */

#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

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


class CWin32SerialPort : public virtual CSerialPort
{
    public:

    typedef std::vector< CORE::CString > TStringList;
    
    static CWin32SerialPort* Create( const CORE::CString& portName );
        
    virtual ~CWin32SerialPort();
    
    virtual CORE::CString GetPortType( void ) const;

    virtual const CORE::CString& GetClassTypeName( void ) const;

    virtual bool IsOpen( void ) const;

    virtual bool Open( void );

    virtual void Close( void );

    virtual UInt8 ReadByte( void );

    virtual char ReadChar( void );

    virtual CORE::CString ReadString( const Int32 maxStringLength );

    virtual CORE::CString ReadLine( const Int32 maxStringLength );

    virtual UInt32 ReadBytesTo( CORE::CDynamicBuffer& buffer ,
                                const UInt32 elementSize     ,
                                const UInt32 elementsToRead  );

    virtual UInt32 GetDataSizeInBytesInReceiveBuffer( void );

    virtual void SetReadTimeout( const UInt32 timeOutInMilliSecs );

    virtual UInt32 GetReadTimeout( void ) const;
    
    virtual void SetWriteTimeout( const UInt32 timeOutInMilliSecs );

    virtual UInt32 GetWriteTimeout( void ) const;

    virtual bool SetBaudRate( const UInt32 baudRate );

    virtual UInt32 GetBaudRate( void ) const;

    static void PopulatePortList( TStringList& portList );

    private:

    CWin32SerialPort( void );
    CWin32SerialPort( const CWin32SerialPort& src );
    CWin32SerialPort& operator=( const CWin32SerialPort& src );

    private:

    void* m_portHandle;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CWIN32SERIALPORT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

---------------------------------------------------------------------------*/

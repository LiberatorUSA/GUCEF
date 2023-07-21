/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

#undef Create

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

    typedef CORE::CString::StringVector TStringList;
    typedef CORE::CTSharedPtr< CWin32SerialPort, MT::CMutex >     CWin32SerialPortPtr;
    
    static CWin32SerialPortPtr Create( const CORE::CString& portName );
        
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

/*-------------------------------------------------------------------------*/

typedef CWin32SerialPort::CWin32SerialPortPtr   CWin32SerialPortPtr;

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

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

#ifndef GUCEF_COMCORE_CICOMMUNICATIONINTERFACE_H
#include "gucefCOMCORE_CICommunicationInterface.h"
#define GUCEF_COMCORE_CICOMMUNICATIONINTERFACE_H
#endif /* GUCEF_COMCORE_CICOMMUNICATIONINTERFACE_H ? */

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


class GUCEF_COMCORE_EXPORT_CPP CSerialPort : public virtual CICommunicationInterface
{
    public:
    
    virtual ~CSerialPort();
    
    virtual CORE::CString GetCommunicationInterfaceType( void ) const;

    virtual const CORE::CString& GetClassTypeName( void ) const;

    virtual bool IsOpen( void ) const = 0;

    virtual bool Open( void ) = 0;

    virtual void Close( void ) = 0;

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

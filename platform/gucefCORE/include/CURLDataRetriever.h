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

#ifndef GUCEF_CORE_CURLDATARETRIEVER_H
#define GUCEF_CORE_CURLDATARETRIEVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_CURL_H
#include "CURL.h"
#define GUCEF_CORE_CURL_H
#endif /* GUCEF_CORE_CURL_H ? */

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
 *  class that allows you to retrieve and subsequently store data
 *  using URL's
 */
class GUCEF_CORE_PUBLIC_CPP CURLDataRetriever : public CForwardingNotifier ,
                                                public CIURLEvents
{
    public:

    CURLDataRetriever( void );

    CURLDataRetriever( PulseGeneratorPtr pulseGenerator );

    virtual ~CURLDataRetriever();

    CURL& GetURL( void );

    const CURL& GetURL( void ) const;

    // utility function for using memory storage
    void SetMemoryBufferAsStorage( void );

    // utility function for using a file as storage
    void SetFileAsStorage( const CString& filePath );

    void SetIOAccess( CIOAccess* ioAccess );

    CIOAccess* GetIOAccess( void ) const;

    UInt64 GetTotalBytesReceived( void ) const;

    // Returns the total size of the resource in bytes if known
    // returns -1 if unknown
    Int64 GetTotalResourceBytes( void ) const;

    // returns the progress of the transfer as a %
    Float32 GetTransferProgress( void ) const;

    protected:

    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );

    private:

    CURLDataRetriever( const CURLDataRetriever& src );
    CURLDataRetriever& operator=( CURLDataRetriever& src );
    void Initialize( void );

    private:

    CIOAccess* m_ioAccess;
    CURL m_url;
    UInt64 m_totalBytesReceived;
    Int64 m_totalBytes;
    bool m_allDataReceived;
    bool m_ownStorage;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CURLDATARETRIEVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2006 :
        - Initial implementation

---------------------------------------------------------------------------*/

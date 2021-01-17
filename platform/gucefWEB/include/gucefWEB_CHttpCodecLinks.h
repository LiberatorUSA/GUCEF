/*
 *  gucefWEB: GUCEF module providing Web application functionality 
 *  for standardized protocols
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

#ifndef GUCEF_WEB_CHTTPCODECLINKS_H
#define GUCEF_WEB_CHTTPCODECLINKS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_WEB_CHTTPMIMETYPES_H
#include "gucefWEB_CHttpMimeTypes.h"
#define GUCEF_WEB_CHTTPMIMETYPES_H
#endif /* GUCEF_WEB_CHTTPMIMETYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_WEB_PUBLIC_CPP CHttpCodecLinks : public CORE::CObservingNotifier
{    
    public:

    typedef CORE::CDStoreCodecRegistry::TDStoreCodecPtr TMimeTypeCodecPtr;
    
    CHttpCodecLinks( void );
    
    CHttpCodecLinks( const CHttpCodecLinks& src );
    
    virtual ~CHttpCodecLinks();
    
    CHttpCodecLinks& operator=( const CHttpCodecLinks& src );

    virtual bool InitMimeCodecLinks( void );

    virtual bool InitEncodingCodecLinks( void );

    virtual TMimeTypeCodecPtr GetSerializationCodec( const CORE::CString& mimeType ) const;

    virtual TMimeTypeCodecPtr GetDeserializationCodec( const CORE::CString& mimeType ) const;

    virtual void GetSupportedSerializationMimeTypes( CORE::CString::StringVector& reps ) const;

    virtual void GetSupportedDeserializationMimeTypes( CORE::CString::StringVector& reps ) const;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    typedef std::map< CORE::CString, TMimeTypeCodecPtr > TStringToCodecMap;

    TStringToCodecMap m_serializeRepToCodecMap;
    TStringToCodecMap m_deserializeRepToCodecMap;
    TStringToCodecMap m_encodingRepToCodecMap;
    CORE::CString::StringVector m_deserializationReps;
    CORE::CString::StringVector m_serializationReps;
    MT::CMutex m_dataLock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CHTTPCODECLINKS_H ? */
   
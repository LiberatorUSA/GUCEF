/*
 *  gucefKAITAI: Platform module supporting the Kaitai binary description format
 *
 *  Copyright (C) 1998 - 2023.  Dinand Vanvelzen
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

#ifndef GUCEF_KAITAI_CGLOBALKAITAISCHEMAREGISTRY_H
#define GUCEF_KAITAI_CGLOBALKAITAISCHEMAREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#include "gucefCORE_CTEventHandlerFunctor.h"
#define GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#endif /* GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H ? */

#ifndef GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#include "gucefCORE_CGloballyConfigurable.h"
#define GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#endif /* GUCEF_CORE_CGLOBALLYCONFIGURABLE_H ? */

#ifndef GUCEF_KAITAI_CKAITAISCHEMAREGISTRY_H
#include "gucefKAITAI_CKaitaiSchemaRegistry.h"
#define GUCEF_KAITAI_CKAITAISCHEMAREGISTRY_H
#endif /* GUCEF_KAITAI_CKAITAISCHEMAREGISTRY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace KAITAI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_KAITAI_PRIVATE_CPP CGlobalKaitaiSchemaRegistry : public CKaitaiSchemaRegistry ,
                                                             public CORE::CGloballyConfigurable
{
    public:

    static const CORE::CString ClassTypeName; 

    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CKaitaiGlobal;

    CGlobalKaitaiSchemaRegistry( void );

    virtual ~CGlobalKaitaiSchemaRegistry();

    private:

    typedef CORE::CTEventHandlerFunctor< CGlobalKaitaiSchemaRegistry > TEventCallback;
    
    CGlobalKaitaiSchemaRegistry( const CGlobalKaitaiSchemaRegistry& src );              /**< not implemented, don't use */
    CGlobalKaitaiSchemaRegistry& operator=( const CGlobalKaitaiSchemaRegistry& src );   /**< not implemented, don't use */

    bool LoadSchemasFromSources( void );
    
    void RegisterEventHandlers( void );

    void
    OnVfsInitializationCompleted( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventId  ,
                                  CORE::CICloneable* eventData );

    class GUCEF_KAITAI_PRIVATE_CPP SchemaSource
    {
        public:

        SchemaSource();

        CORE::CString location;
        bool recursive;
        CORE::CString family;
    };

    typedef std::vector< SchemaSource > SchemaSourceVector; 
    
    private:

    bool m_vfsInitIsComplete;
    SchemaSourceVector m_vfsSchemaSources;
    SchemaSourceVector m_uriSchemaSources;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAITAI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_KAITAI_CGLOBALKAITAISCHEMAREGISTRY_H ? */

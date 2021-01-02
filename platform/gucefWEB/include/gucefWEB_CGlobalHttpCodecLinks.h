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

#ifndef GUCEF_WEB_CGLOBALHTTPCODECLINKS_H
#define GUCEF_WEB_CGLOBALHTTPCODECLINKS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_WEB_CHTTPCODECLINKS_H
#include "gucefWEB_CHttpCodecLinks.h"
#define GUCEF_WEB_CHTTPCODECLINKS_H
#endif /* GUCEF_WEB_CHTTPCODECLINKS_H ? */

#ifndef GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#include "gucefCORE_CTEventHandlerFunctor.h"
#define GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#endif /* GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H ? */


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

/**
 *  Adds auto updating of HTTP codec MIME type links based on global events
 */
class GUCEF_WEB_PUBLIC_CPP CGlobalHttpCodecLinks : public CHttpCodecLinks
{    
    public:

    CGlobalHttpCodecLinks( void );

    virtual ~CGlobalHttpCodecLinks();

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    typedef CORE::CTEventHandlerFunctor< CGlobalHttpCodecLinks > TEventCallback;

    virtual void OnEventThatMightHaveChangedCodecs( CORE::CNotifier* notifier                 ,
                                                    const CORE::CEvent& eventid               ,
                                                    CORE::CICloneable* eventdata = GUCEF_NULL );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CGLOBALHTTPCODECLINKS_H ? */
   
/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef CDRNCONTROLLERINTERFACE_H
#define CDRNCONTROLLERINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDRNCONTROLLER_H
#include "CDRNController.h"
#define CDRNCONTROLLER_H
#endif /* CDRNCONTROLLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/ 

class GUCEFCOM_EXPORT_CPP CDRNControllerInterface
{
        protected:
        friend class CDRNController;
        
        virtual bool OnDRNLogin( CDRNController& controller    ,
                                 const CORE::CString& username ,
                                 const CORE::CString& password ) = 0;
                            
        virtual void OnDRNLogout( CDRNController& controller    ,
                                  const CORE::CString& username ) = 0;
        
        virtual void OnDRNControllerOpenSocketsFailed( CDRNController& controller ) = 0;

        virtual void OnDRNMainComDataEncode( CDRNController& controller   ,
                                             const char* data             ,
                                             const UInt32 size            ,
                                             CORE::CDynamicBuffer& result ) = 0;
        
        virtual void OnDRNMainComDataDecode( CDRNController& controller   ,
                                             const char* data             ,
                                             const UInt32 size            ,
                                             CORE::CDynamicBuffer& result ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CDRNCONTROLLERINTERFACE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-07-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
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

#ifndef DRNPROTOCOL_H
#define DRNPROTOCOL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"       /* config & macros specific to GUCEF */
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

#ifndef GUCEF_COM_ETYPES_H
#include "gucefCOM_ETypes.h"                /* simple types used */
#define GUCEF_COM_ETYPES_H
#endif /* GUCEF_COM_ETYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define DRN_TCP_STRINGSEPERATOR                7

#define DRN_CONTROLLERCOM_LOGIN                10
#define DRN_CONTROLLERCOM_LOGIN_FAILED         11
#define DRN_CONTROLLERCOM_LOGIN_SUCCESS        12
#define DRN_CONTROLLERCOM_EVENT                13
#define DRN_CONTROLLERCOM_DATAITEM_UPDATE      14
#define DRN_CONTROLLERCOM_DATAITEM_INIT        15
#define DRN_CONTROLLERCOM_USERDATA             16
#define DRN_CONTROLLERCOM_NETWORK_INFO         17

#define DRN_PEERCOM_AUTHENTICATE               101
#define DRN_PEERCOM_AUTHENTICATION_FAILED      102
#define DRN_PEERCOM_AUTHENTICATION_SUCCESS     103
#define DRN_PEERCOM_EVENT                      104
#define DRN_PEERCOM_DATAITEM_UPDATE            105
#define DRN_PEERCOM_USERDATA                   106

/*-------------------------------------------------------------------------*/

#endif /* DRNPROTOCOL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-07-2005 :
        - initial version

-----------------------------------------------------------------------------*/
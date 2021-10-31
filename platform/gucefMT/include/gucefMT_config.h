/*
 *  gucefMT: GUCEF module providing multithreading solutions
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

#ifndef GUCEF_MT_CONFIG_H
#define GUCEF_MT_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"           /* GUCEF platform wide config */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"          /* GUCEF platform wide macros */
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      GUCEF_MT_BUILD_MODULE
 *
 *      It is best to set this in your project/make file, you can however
 *      set this here if your environment does not allow
 */
/* #define GUCEF_MT_BUILD_MODULE */

#define GUCEF_MT_PUBLIC_C_CODE
#define GUCEF_MT_PUBLIC_CPP_CODE

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-09-2006 :
       - Dinand: moved platform wide build switches to the GUCEF platform config file
- 25-02-2006 :
       - Dinand: Added GUCEFMT_ prefix to all defines
- 16-10-2004 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/


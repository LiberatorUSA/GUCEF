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

#ifndef GUCEF_KAITAI_MACROS_H
#define GUCEF_KAITAI_MACROS_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#include "gucefKAITAI_config.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/


/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */

#undef GUCEF_KAITAI_EXPORTSPEC
#ifdef GUCEF_KAITAI_BUILD_MODULE
  #define GUCEF_KAITAI_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUCEF_KAITAI_EXPORTSPEC GUCEF_IMPORT
#endif /* GUCEF_KAITAI_BUILD_MODULE ? */

#undef GUCEF_KAITAI_PUBLIC_CPP
#ifdef GUCEF_KAITAI_PUBLIC_CPP_CODE
  #define GUCEF_KAITAI_PUBLIC_CPP GUCEF_KAITAI_EXPORTSPEC
#else
  #define GUCEF_KAITAI_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_KAITAI_EXPORT_CPP_CODE */

#define GUCEF_KAITAI_PRIVATE_CPP GUCEF_HIDDEN

#undef GUCEF_KAITAI_PUBLIC_C
#ifdef GUCEF_KAITAI_PUBLIC_C_CODE
  #define GUCEF_KAITAI_PUBLIC_C GUCEF_KAITAI_EXPORTSPEC
#else
  #define GUCEF_KAITAI_PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEF_KAITAI_PUBLIC_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_KAITAI_MACROS_H ? */

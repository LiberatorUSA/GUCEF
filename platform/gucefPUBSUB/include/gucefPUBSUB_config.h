/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
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

#ifndef GUCEF_PUBSUB_CONFIG_H
#define GUCEF_PUBSUB_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */
#undef GUCEF_PUBSUB_EXPORT_CPP_CODE        /* do not change this line */ 
#undef GUCEF_PUBSUB_EXPORT_C_CODE          /* do not change this line */  
#define GUCEF_PUBSUB_EXPORT_CPP_CODE /* do you want to enable the C++ exports ? */
#define GUCEF_PUBSUB_EXPORT_C_CODE   /* do you want to enable the C exports ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CONFIG_H */

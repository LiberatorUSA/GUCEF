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

#ifndef GUCEF_CORE_C_DOCUMENT_PARSER_CALLBACKS_H
#define GUCEF_CORE_C_DOCUMENT_PARSER_CALLBACKS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_C_VARIANTDATA_H
#include "gucefCORE_c_variantdata.h"
#define GUCEF_CORE_C_VARIANTDATA_H
#endif /* GUCEF_CORE_C_VARIANTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace CORE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnDocumentBegin ) ( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnDocumentEnd ) ( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnNodeBegin ) ( void* privdata, const TVariantData* nodeId, int nodeType ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnNodeEnd ) ( void* privdata, const TVariantData* nodeId ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnNodeAttribute ) ( void* privdata, const TVariantData* nodeId, const TVariantData* attId, const TVariantData* attValue ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnNodeValue ) ( void* privdata, const TVariantData* nodeId, const TVariantData* nodeValue ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnNodeChildrenBegin ) ( void* privdata, const TVariantData* nodeId ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnNodeChildrenEnd ) ( void* privdata, const TVariantData* nodeId ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TDocParserCallback_OnParserError ) ( void* privdata, Int32 errorcode, const char* description ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------*/

/**
 *  Structure that hold all the function pointers to the event handlers
 *  for read events. Should be called by plugins as needed.
 */
struct SDocumentParserCallbacks
{
    TDocParserCallback_OnDocumentBegin     OnDocumentBegin;
    TDocParserCallback_OnDocumentEnd       OnDocumentEnd;
    TDocParserCallback_OnNodeBegin         OnNodeBegin;
    TDocParserCallback_OnNodeEnd           OnNodeEnd;
    TDocParserCallback_OnNodeAttribute     OnNodeAttribute;
    TDocParserCallback_OnNodeValue         OnNodeValue;
    TDocParserCallback_OnNodeChildrenBegin OnNodeChildrenBegin;
    TDocParserCallback_OnNodeChildrenEnd   OnNodeChildrenEnd;
    TDocParserCallback_OnParserError       OnParserError;    
    
    void* privateData;
};
typedef struct SDocumentParserCallbacks TDocumentParserCallbacks;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_C_DOCUMENT_PARSER_CALLBACKS_H ? */

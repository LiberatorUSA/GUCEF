/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CIMESSAGETRANSFORMER_H
#define GUCEF_COMCORE_CIMESSAGETRANSFORMER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CIMESSAGE_H
#include "gucefCOMCORE_CIMessage.h"
#define GUCEF_COMCORE_CIMESSAGE_H
#endif /* GUCEF_COMCORE_CIMESSAGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

enum MessageTransformResult
{
    MESSAGE_TRANSFORM_RESULT_UNKNOWN = 0,           /**< init state */
        
    MESSAGE_TRANSFORM_RESULT_SUCCESS = 1,           /**< succesful transform, use the transformed message */
    MESSAGE_TRANSFORM_RESULT_VOIDED_MSG = 2,        /**< success, output is voided meaning nothing. Do not process further */
    MESSAGE_TRANSFORM_RESULT_NOCHANGE = 3,          /**< success, no change in the message. Use the original */

    MESSAGE_TRANSFORM_RESULT_INTERNAL_ERROR = 4,    /**< internal error in the transformer not specific to the given message */
    MESSAGE_TRANSFORM_RESULT_INVALID_MSG = 5        /**< invalid message. The transformer requires certain message characteristics and the given message does not meet them */
};

/*-------------------------------------------------------------------------*/

/**
 *  Abstract base class for message transformers.
 * 
 *  Note that transformer implementations are allowed to link data from the original message to the transformed message.
 *  This means that the transformed message may contain references to data in the original message.
 *  This requires that the original message is not destroyed before the transformed message.
 */
class GUCEF_COMCORE_EXPORT_CPP CIMessageTransformer
{
    public:
    
    virtual ~CIMessageTransformer();

    virtual MessageTransformResult TransformMessage( const CIMessage& originalMessage, CIMessage& transformedMessage ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CIMESSAGETRANSFORMER_H ? */

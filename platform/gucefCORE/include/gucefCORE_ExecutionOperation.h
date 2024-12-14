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

#ifndef GUCEF_CORE_EXECUTIONOPERATION_H
#define GUCEF_CORE_EXECUTIONOPERATION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"   
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Enumeration of the operations supported by the execution engine
 */
enum EExecutionEngineOperation
{
    EXECENGOPERATION_UNKNOWN      = 0,
    
    EXECENGOPERATION_ENTER_SCOPE                   ,             /**< Entering a scope. May init scope stack variables */
    EXECENGOPERATION_EXIT_SCOPE                    ,             /**< Leaving a scope. May destruct scope stack variables */
    EXECENGOPERATION_EXECUTE_FUNCTION              ,
    EXECENGOPERATION_SCOPE_REPEAT_LOOP             ,
    EXECENGOPERATION_SCOPE_WHILE_BOOLEXPR_LOOP     ,
    EXEENGCOPERATION_IF_BRANCH_BOOLEXPR_EVALUATION ,
    EXECENGOPERATION_SWITCH_BRANCH_EVALUATION      ,

    EXECENGOPERATION_TYPE_CONVERSION               ,
    EXECENGOPERATION_TYPE_ID                       ,
    EXECENGOPERATION_TYPE_SIZEOF                   ,
    EXECENGOPERATION_TYPE_NAME                     ,

    EXECENGOPERATION_SIZEOF_VAR                    ,

    EXECENGOPERATION_BOOL_EVAL_EQUALS              ,
    EXECENGOPERATION_BOOL_EVAL_NOT_EQUALS          ,
    EXECENGOPERATION_BOOL_EVAL_GREATER_THAN        ,
    EXECENGOPERATION_BOOL_EVAL_GREATER_EQUALS_THAN ,
    EXECENGOPERATION_BOOL_EVAL_LESS_THAN           ,
    EXECENGOPERATION_BOOL_EVAL_LESS_EQUALS_THAN    ,
    EXECENGOPERATION_BOOL_EVAL_AND                 ,
    EXECENGOPERATION_BOOL_EVAL_OR                  ,
    EXECENGOPERATION_BOOL_EVAL_NOT                 ,

    EXECENGOPERATION_BITWISE_AND                   ,
    EXECENGOPERATION_BITWISE_OR                    ,
    EXECENGOPERATION_BITWISE_XOR                   ,

    EXECENGOPERATION_ARITHMETIC_ADD                ,
    EXECENGOPERATION_ARITHMETIC_SUBTRACT           ,
    EXECENGOPERATION_ARITHMETIC_MULTIPLY           ,
    EXECENGOPERATION_ARITHMETIC_DIVIDE             ,
    EXECENGOPERATION_ARITHMETIC_MODULO             ,
    EXECENGOPERATION_ARITHMETIC_NEGATE             ,
    EXECENGOPERATION_ARITHMETIC_INCREMENT          ,
    EXECENGOPERATION_ARITHMETIC_DECREMENT          ,

};

typedef enum EExecutionOperation TExecutionOperation;

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

#endif /* GUCEF_CORE_EXECUTIONOPERATION_H ? */

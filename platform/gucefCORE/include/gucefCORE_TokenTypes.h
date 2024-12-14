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

#ifndef GUCEF_CORE_TOKENTYPES_H
#define GUCEF_CORE_TOKENTYPES_H

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
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum EProgrammingTokenType
{
    PROGRAMMINGTOKENTYPE_UNKNOWN        = 0,

    PROGRAMMINGTOKENTYPE_KEYWORD        ,       /**< Reserved words that have a special meaning in the language syntax (e.g., if, else, while, return). */
    PROGRAMMINGTOKENTYPE_IDENTIFIER     ,       /**< Names given to variables, functions, classes, and other user-defined items (e.g., myVariable, calculateSum). */
    PROGRAMMINGTOKENTYPE_OPERATOR       ,       /**< Symbols that represent operations (e.g., +, -, *, /, &&, ||). */
    PROGRAMMINGTOKENTYPE_LITERAL        ,       /**< Literals: Constant values used directly in the code (e.g., 42, 3.14, 'a', "Hello, World!"). */
    PROGRAMMINGTOKENTYPE_COMMENT        ,       /**< Annotations in the code that are ignored by the compiler (e.g., single-line comment or multi-line comment ). */
    PROGRAMMINGTOKENTYPE_PREPROCESSOR   ,       /**< Directives that are processed by the preprocessor before the actual compilation begins (e.g., #include, #define). */
    PROGRAMMINGTOKENTYPE_DELIMITER      ,       /**< Delimiters are characters that are used to separate tokens (e.g., ;, ,, (), {}, []) .*/
    PROGRAMMINGTOKENTYPE_WHITESPACE             /**< Spaces, tabs, and newline characters that separate tokens but are generally ignored by the compiler except for their role in token separation. */
};

typedef enum EProgrammingTokenType TProgrammingTokenType;

/*-------------------------------------------------------------------------*/

enum EProgrammingTokenSubType
{
    PROGRAMMINGTOKENSUBTYPE_UNKNOWN     = 0,

    PROGRAMMINGTOKENSUBTYPE_KEYWORD_CONTROLFLOW,      /**< Control flow keywords (e.g., if, else, while, for, switch, case, break, continue, return). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_DATA,             /**< Data type keywords (e.g., int, float, char, double, void). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_MODIFIER,         /**< Modifier keywords (e.g., const, static, extern, volatile). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_STORAGE,          /**< Storage class keywords (e.g., auto, register, static, extern, mutable). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_ACCESS,           /**< Access control keywords (e.g., public, private, protected). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_NAMESPACE,        /**< Namespace keywords (e.g., namespace, using). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_EXCEPTION,        /**< Exception handling keywords (e.g., try, catch, throw). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_OPERATOR,         /**< Operator keywords (e.g., new, delete, sizeof, typeid). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_CAST,             /**< Cast keywords (e.g., static_cast, dynamic_cast, const_cast, reinterpret_cast). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_TEMPLATE,         /**< Template keywords (e.g., template, typename, class). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_PREPROCESSOR,     /**< Preprocessor keywords (e.g., #include, #define, #ifdef, #ifndef, #endif). */
    PROGRAMMINGTOKENSUBTYPE_KEYWORD_OTHER,            /**< Other keywords (e.g., true, false, nullptr). */

    PROGRAMMINGTOKENSUBTYPE_OPERATOR_ARITHMETIC,      /**< Arithmetic operators (e.g., +, -, *, /, %). */
    PROGRAMMINGTOKENSUBTYPE_OPERATOR_RELATIONAL,      /**< Relational operators (e.g., ==, !=, <, >, <=, >=). */
    PROGRAMMINGTOKENSUBTYPE_OPERATOR_LOGICAL,         /**< Logical operators (e.g., &&, ||, !). */
    PROGRAMMINGTOKENSUBTYPE_OPERATOR_BITWISE,         /**< Bitwise operators (e.g., &, |, ^, ~, <<, >>). */
    PROGRAMMINGTOKENSUBTYPE_OPERATOR_ASSIGNMENT,      /**< Assignment operators (e.g., =, +=, -=, *=, /= */
    PROGRAMMINGTOKENSUBTYPE_OPERATOR_INCREMENT,       /**< Increment operators (e.g., ++, --). */
    PROGRAMMINGTOKENSUBTYPE_OPERATOR_TERNARY,         /**< Ternary operator (e.g., ? :). */
    PROGRAMMINGTOKENSUBTYPE_OPERATOR_MEMBER,          /**< Member access operators (e.g., ., ->). */
    PROGRAMMINGTOKENSUBTYPE_OPERATOR_OTHER,           /**< Other operators (e.g., sizeof, typeid, new, delete). */
};

typedef enum EProgrammingTokenSubType TProgrammingTokenSubType;

/*-------------------------------------------------------------------------*/

enum EHumanLanguageTokenType
{
    HUMANLANGUAGETOKENTYPE_UNKNOWN       = 0,

    HUMANLANGUAGETOKENTYPE_WORDS            ,         /**< The basic units of meaning, which can be nouns, verbs, adjectives, adverbs, etc. (e.g., "cat", "run", "beautiful"). */
    HUMANLANGUAGETOKENTYPE_PUNCTUATION      ,         /**< Symbols that help structure sentences and convey meaning (e.g., periods, commas, question marks). */
    HUMANLANGUAGETOKENTYPE_WHITESPACE       ,         /**< Spaces, tabs, and newlines that separate words and sentences. */
    HUMANLANGUAGETOKENTYPE_NUMBERS          ,         /**< Digits and numerical expressions (e.g., "42", "3.14"). */
    HUMANLANGUAGETOKENTYPE_SYMBOLS          ,         /**< Symbols: Special characters that have specific meanings (e.g., "@", "#", "$"). */
    HUMANLANGUAGETOKENTYPE_PHRASES          ,         /**< Groups of words that function as a single unit within a sentence (e.g., "in the morning", "at the park"). */
    HUMANLANGUAGETOKENTYPE_SENTENCES        ,         /**< Complete thoughts expressed with a subject and predicate (e.g., "The cat sat on the mat."). */
    HUMANLANGUAGETOKENTYPE_PARAGRAPHS       ,         /**< Groups of sentences that form a coherent section of text. */

};

typedef enum EHumanLanguageTokenType THumanLanguageTokenType;

/*-------------------------------------------------------------------------*/

enum EHumanLanguageTokenSubType
{
    HUMANLANGUAGETOKENSUBTYPE_UNKNOWN    = 0,

    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_PERIOD,      /**< The period (.) is used to mark the end of a sentence. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_COMMA,       /**< The comma (,) is used to separate items in a list or to indicate a pause in a sentence. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_QUESTION,    /**< The question mark (?) is used to indicate a question. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_EXCLAMATION, /**< The exclamation mark (!) is used to indicate excitement or emphasis. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_COLON,       /**< The colon (:) is used to introduce a list or to separate clauses in a sentence. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_SEMICOLON,   /**< The semicolon (;) is used to separate independent clauses or items in a list. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_APOSTROPHE,  /**< The apostrophe (') is used to indicate possession or to form contractions. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_QUOTATION,   /**< The quotation mark (") is used to indicate direct speech or to enclose a quote. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_DASH,        /**< The dash (-) is used to indicate a break in thought or to set off a phrase. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_SLASH,       /**< The slash (/) is used to indicate a choice or to separate items in a list. */
    HUMANLANGUAGETOKENSUBTYPE_PUNCTUATION_OTHER,       /**< Other punctuation marks not covered by the above categories. */

    HUMANLANGUAGETOKENSUBTYPE_SYMBOLS_MATH,            /**< Mathematical symbols (e.g., +, -, *, /, =). */
    HUMANLANGUAGETOKENSUBTYPE_SYMBOLS_CURRENCY,        /**< Currency symbols (e.g., $, €, £, ¥). */
    HUMANLANGUAGETOKENSUBTYPE_SYMBOLS_ARROWS,          /**< Arrow symbols (e.g., ->, <-, =>, <=). */
    HUMANLANGUAGETOKENSUBTYPE_SYMBOLS_OTHER,           /**< Other symbols not covered by the above categories. */
};

typedef enum EHumanLanguageTokenSubType THumanLanguageTokenSubType;

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

#endif /* GUCEF_CORE_TOKENTYPES_H ? */

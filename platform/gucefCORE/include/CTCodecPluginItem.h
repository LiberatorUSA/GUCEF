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

#ifndef GUCEF_CORE_CTCODECPLUGINITEM_H
#define GUCEF_CORE_CTCODECPLUGINITEM_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_CICODEC_H
#include "CICodec.h"     
#define GUCEF_CORE_CICODEC_H
#endif /* GUCEF_CORE_CICODEC_H ? */

#ifndef GUCEF_CORE_CICODECPLUGIN_H
#include "CICodecPlugin.h"
#define GUCEF_CORE_CICODECPLUGIN_H
#endif /* GUCEF_CORE_CICODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

///**
// *  Template that allows you to specify seperate codec's 
// *  originating from a single plugin without having to create new classes.
// *  This template simply delegates the Encode() and Decode() actions to
// *  the given plugin class and passes along it's typename.
// *  Because you can define your own baseclass you can make an easy to use interface
// *  without having to code the delegation aspect in a seperate class
// *
// *  Note that TCodecType should be derived from CICodec
// *
//template < class TCodecType >
//class CTCodecPluginItem : public TCodecType
//{
//    public:
//    
//    typedef TCodecType TBaseClass;
//    typedef CICodec::TDynamicBufferList TDynamicBufferList;
//
//    CTCodecPluginItem( CICodecPlugin& codecPlugin ,
//                       const CString& typeName    );
//    
//    virtual ~CTCodecPluginItem();
//
//    virtual bool Encode( const void* sourceData         ,
//                         const UInt32 sourceBuffersSize ,
//                         TDynamicBufferList& dest       ,
//                         UInt32& destBuffersUsed        );
//
//    virtual bool Decode( const void* sourceData         ,
//                         const UInt32 sourceBuffersSize ,
//                         TDynamicBufferList& dest       ,
//                         UInt32& destBuffersUsed        );
//
//    virtual CICloneable* Clone( void ) const;   
//    
//    virtual CString GetType( void ) const;
//                      
//    private:
//    
//    CTCodecPluginItem( void );
//    CTCodecPluginItem( const CTCodecPluginItem& src );
//    CTCodecPluginItem& operator=( const CTCodecPluginItem& src );
//
//    private:
//    
//    CICodecPlugin* m_codecPlugin;
//    CString m_typeName;
//};
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      UTILITIES                                                          //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//template < class TCodecType >
//CTCodecPluginItem< TCodecType >::CTCodecPluginItem( const CTCodecPluginItem& src )
//    : TCodecType( src )                  ,
//      m_codecPlugin( src.m_codecPlugin ) ,
//      m_typeName( src.m_typeName )
//{GUCEF_TRACE;
//
//    assert( NULL != &m_codecPlugin );
//    assert( m_typeName.Length() > 0 );
//}
//
///*-------------------------------------------------------------------------*/
//
//template < class TCodecType >
//CTCodecPluginItem< TCodecType >::CTCodecPluginItem( CICodecPlugin& plugin   ,
//                                                    const CString& typeName )
//    : TCodecType()             ,
//      m_codecPlugin( &plugin ) ,
//      m_typeName( typeName )
//{GUCEF_TRACE;
//
//    assert( NULL != &plugin );
//    assert( m_typeName.Length() > 0 );
//    
//    /*
//     *  Compile time test
//     *  Used to check whether 'TCodecType' is actually a decendant of CICodec
//     *  If you get an error here then check your inheritance
//     */
//    static_cast< CICodec* >( this );
//}
//
///*-------------------------------------------------------------------------*/
//
//template < class TCodecType >
//CTCodecPluginItem< TCodecType >::~CTCodecPluginItem()
//{GUCEF_TRACE;
//
//}
//
///*-------------------------------------------------------------------------*/
//
//template < class TCodecType >
//CICloneable*
//CTCodecPluginItem< TCodecType >::Clone( void ) const
//{GUCEF_TRACE;
//
//    return GUCEF_NEW CTCodecPluginItem< TCodecType >( *this );
//}
//
///*-------------------------------------------------------------------------*/
//
//template < class TCodecType >
//CString
//CTCodecPluginItem< TCodecType >::GetType( void ) const
//{GUCEF_TRACE;
//
//    return m_typeName;
//}
//
///*-------------------------------------------------------------------------*/
//
//template < class TCodecType >
//bool
//CTCodecPluginItem< TCodecType >::Encode( const void* sourceData         ,
//                                         const UInt32 sourceBuffersSize ,
//                                         TDynamicBufferList& dest       ,
//                                         UInt32& destBuffersUsed        )
//{GUCEF_TRACE;
//
//    return m_codecPlugin->Encode( sourceData        ,
//                                  sourceBuffersSize ,
//                                  dest              ,
//                                  destBuffersUsed   ,
//                                  GetFamilyName()   ,
//                                  GetType()         );
//}
//
///*-------------------------------------------------------------------------*/
//
//template < class TCodecType >
//bool
//CTCodecPluginItem< TCodecType >::Decode( const void* sourceData         ,
//                                         const UInt32 sourceBuffersSize ,
//                                         TDynamicBufferList& dest       ,
//                                         UInt32& destBuffersUsed        )
//{GUCEF_TRACE;
//
//    return m_codecPlugin->Decode( sourceData        ,
//                                  sourceBuffersSize ,
//                                  dest              ,
//                                  destBuffersUsed   ,
//                                  GetFamilyName()   ,
//                                  GetType()         );
//}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace CORE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTCODECPLUGINITEM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-12-2006 :
        - Dinand: Moved to CORE and changed into a template
- 15-12-2006 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
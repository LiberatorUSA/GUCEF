/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <malloc.h>     /* needed for malloc/realloc */
#include <string.h>     /* needed for memset */

#ifndef DVCPPSTRINGUTILS_H 
#include "dvcppstringutils.h"
#define DVCPPSTRINGUTILS_H
#endif /* DVCPPSTRINGUTILS_H ? */

#ifndef CDATANODE_H
#include "CDataNode.h"
#define CDATANODE_H
#endif /* CDATANODE_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CInputActionMap.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputActionMap::CInputActionMap( const CORE::CString& name       ,
                                  const UInt32 mousemapcount      ,
                                  const UInt32 keyboardmapcount   ,
                                  const UInt32 deviceboolmapcount ,
                                  const UInt32 devicevarmapcount  ,
                                  const UInt32 devicecount        )
        : m_mousemapcount( mousemapcount )           ,
          m_keyboardmapcount( keyboardmapcount )     ,
          m_deviceboolmapcount( 0UL )                ,
          m_devicevarmapcount( 0UL )                 ,
          m_devicecount( 0UL )                       ,
          m_name( name )                             ,
          m_mousebupmap( NULL )                      ,
          m_mousebdownmap( NULL )                    ,
          m_keyboardkupmap( NULL )                   ,
          m_keyboardkdownmap( NULL )                 ,
          m_deviceboolonmap( NULL )                  ,
          m_devicebooloffmap( NULL )                 ,
          m_devicevarchangemap( NULL )
          
{GUCEF_TRACE;
        m_mousebupmap = (UInt32*) malloc( mousemapcount * sizeof(UInt32) );
        m_mousebdownmap = (UInt32*) malloc( mousemapcount * sizeof(UInt32) );
        m_keyboardkupmap = (UInt32*) malloc( keyboardmapcount * sizeof(UInt32) );
        m_keyboardkdownmap = (UInt32*) malloc( keyboardmapcount * sizeof(UInt32) );
        
        memset( m_mousebupmap, 0, mousemapcount * sizeof(UInt32) );
        memset( m_mousebdownmap, 0, mousemapcount * sizeof(UInt32) );
        memset( m_keyboardkupmap, 0, keyboardmapcount * sizeof(UInt32) );
        memset( m_keyboardkdownmap, 0, keyboardmapcount * sizeof(UInt32) );
        
        EnsureDeviceMaxCapacity( devicecount );

        for ( UInt32 i=0; i<devicecount; ++i )
        {
                EnsureDeviceBoolStateMaxCapacity( i, deviceboolmapcount );
                EnsureDeviceVarStateMaxCapacity( i, devicevarmapcount );
        }
}

/*-------------------------------------------------------------------------*/

CInputActionMap::CInputActionMap( const CInputActionMap& src )
        : m_mousemapcount( src.m_mousemapcount )           ,
          m_keyboardmapcount( src.m_keyboardmapcount )     ,
          m_deviceboolmapcount( 0UL )                      ,
          m_devicevarmapcount( 0UL )                       ,
          m_devicecount( 0UL )                             ,
          m_mousebupmap( NULL )                            ,
          m_mousebdownmap( NULL )                          ,
          m_keyboardkupmap( NULL )                         ,
          m_keyboardkdownmap( NULL )                       ,
          m_deviceboolonmap( NULL )                        ,
          m_devicebooloffmap( NULL )                       ,
          m_devicevarchangemap( NULL )                     ,
          m_name( src.m_name )
{GUCEF_TRACE;
        m_mousebupmap = (UInt32*) malloc( m_mousemapcount * sizeof(UInt32) );
        m_mousebdownmap = (UInt32*) malloc( m_mousemapcount * sizeof(UInt32) );
        m_keyboardkupmap = (UInt32*) malloc( m_keyboardmapcount * sizeof(UInt32) );
        m_keyboardkdownmap = (UInt32*) malloc( m_keyboardmapcount * sizeof(UInt32) );
        
        memset( m_mousebupmap, 0, m_mousemapcount * sizeof(UInt32) );
        memset( m_mousebdownmap, 0, m_mousemapcount * sizeof(UInt32) );
        memset( m_keyboardkupmap, 0, m_keyboardmapcount * sizeof(UInt32) );
        memset( m_keyboardkdownmap, 0, m_keyboardmapcount * sizeof(UInt32) );                
        
        EnsureDeviceMaxCapacity( src.m_devicecount );

        for ( UInt32 i=0; i<m_devicecount; ++i )
        {
                EnsureDeviceBoolStateMaxCapacity( i, src.m_deviceboolmapcount );
                EnsureDeviceVarStateMaxCapacity( i, src.m_devicevarmapcount );
        }        
}

/*-------------------------------------------------------------------------*/

CInputActionMap::~CInputActionMap()
{GUCEF_TRACE;
        Clear();
}

/*-------------------------------------------------------------------------*/

CInputActionMap&
CInputActionMap::operator=( const CInputActionMap& src )
{GUCEF_TRACE;
        if ( &src != this )
        {       
                m_name = src.m_name;
                
                m_mousemapcount = src.m_mousemapcount;
                m_keyboardmapcount = src.m_keyboardmapcount;
                m_deviceboolmapcount = src.m_deviceboolmapcount;
                m_devicevarmapcount = src.m_devicevarmapcount;
                m_devicecount = 0;
                  
                m_mousebupmap = (UInt32*) realloc( m_mousebupmap, m_mousemapcount * sizeof(UInt32) );
                m_mousebdownmap = (UInt32*) realloc( m_mousebdownmap, m_mousemapcount * sizeof(UInt32) );
                m_keyboardkupmap = (UInt32*) realloc( m_keyboardkupmap, m_keyboardmapcount * sizeof(UInt32) );
                m_keyboardkdownmap = (UInt32*) realloc( m_keyboardkdownmap, m_keyboardmapcount * sizeof(UInt32) );
                EnsureDeviceMaxCapacity( src.m_devicecount );
                
                memcpy( m_mousebupmap, src.m_mousebupmap, m_mousemapcount * sizeof(UInt32) );
                memcpy( m_mousebdownmap, src.m_mousebdownmap, m_mousemapcount * sizeof(UInt32) );
                memcpy( m_keyboardkupmap, src.m_keyboardkupmap, m_keyboardmapcount * sizeof(UInt32) );
                memcpy( m_keyboardkdownmap, src.m_keyboardkdownmap, m_keyboardmapcount * sizeof(UInt32) );
                
                m_mouseBUpActionNames = src.m_mouseBUpActionNames;
                m_mouseBDownActionNames = src.m_mouseBDownActionNames;
                m_keyboardKUpActionNames = src.m_keyboardKUpActionNames;
                m_keyboardKDownActionNames = src.m_keyboardKDownActionNames;                
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CInputActionMap::SetMouseButtonDownMap( const UInt32 buttonindex        ,
                                        const UInt32 actionindex        ,
                                        const CORE::CString& actionName )
{GUCEF_TRACE;
        if ( buttonindex >= m_mousemapcount )
        {
                UInt32 oldmax( m_mousemapcount );
                m_mousemapcount = buttonindex+1;
                m_mousebdownmap = (UInt32*) realloc( m_mousebdownmap, m_mousemapcount * sizeof(UInt32) );
                m_mousebupmap = (UInt32*) realloc( m_mousebupmap, m_mousemapcount * sizeof(UInt32) );
                memset( m_mousebdownmap+oldmax, 0, m_mousemapcount-oldmax );
                memset( m_mousebupmap+oldmax, 0, m_mousemapcount-oldmax );
                m_mouseBUpActionNames.AddAtIndex( actionName, actionindex );
        }
        
        m_mousebdownmap[ buttonindex ] = actionindex;
}                                        

/*-------------------------------------------------------------------------*/
                                    
UInt32 
CInputActionMap::GetMouseButtonDownMap( const UInt32 buttonindex )
{GUCEF_TRACE;
        return m_mousebdownmap[ buttonindex ];
}

/*-------------------------------------------------------------------------*/                                    

void 
CInputActionMap::SetMouseButtonUpMap( const UInt32 buttonindex        ,
                                      const UInt32 actionindex        ,
                                      const CORE::CString& actionName )
{GUCEF_TRACE;
        if ( buttonindex >= m_mousemapcount )
        {
                UInt32 oldmax( m_mousemapcount );
                m_mousemapcount = buttonindex+1;
                m_mousebdownmap = (UInt32*) realloc( m_mousebdownmap, m_mousemapcount * sizeof(UInt32) );
                m_mousebupmap = (UInt32*) realloc( m_mousebupmap, m_mousemapcount * sizeof(UInt32) );
                memset( m_mousebdownmap+oldmax, 0, m_mousemapcount-oldmax );
                memset( m_mousebupmap+oldmax, 0, m_mousemapcount-oldmax );
                m_mouseBDownActionNames.AddAtIndex( actionName, actionindex );
        }

        m_mousebupmap[ buttonindex ] = actionindex;
}

/*-------------------------------------------------------------------------*/
                                    
UInt32 
CInputActionMap::GetMouseButtonUpMap( const UInt32 buttonindex )
{GUCEF_TRACE;
        return m_mousebupmap[ buttonindex ];
}

/*-------------------------------------------------------------------------*/

void 
CInputActionMap::SetKeyboardKeyDownMap( const UInt32 keyindex           ,
                                        const UInt32 actionindex        ,
                                        const CORE::CString& actionName )
{GUCEF_TRACE;
        if ( keyindex >= m_keyboardmapcount )
        {
                UInt32 oldmax( m_keyboardmapcount );
                m_keyboardmapcount = keyindex+1;
                m_keyboardkdownmap = (UInt32*) realloc( m_keyboardkdownmap, m_keyboardmapcount * sizeof(UInt32) );
                m_keyboardkupmap = (UInt32*) realloc( m_keyboardkupmap, m_keyboardmapcount * sizeof(UInt32) );
                memset( m_keyboardkdownmap+oldmax, 0, m_keyboardmapcount-oldmax );
                memset( m_keyboardkupmap+oldmax, 0, m_keyboardmapcount-oldmax );
                m_keyboardKUpActionNames.AddAtIndex( actionName, actionindex );
        }

        m_keyboardkdownmap[ keyindex ] = actionindex;
}                                        

/*-------------------------------------------------------------------------*/
                                    
UInt32 
CInputActionMap::GetKeyboardKeyDownMap( const UInt32 keyindex )
{GUCEF_TRACE;
        return m_keyboardkdownmap[ keyindex ];
}

/*-------------------------------------------------------------------------*/
        
void 
CInputActionMap::SetKeyboardKeyUpMap( const UInt32 keyindex           ,
                                      const UInt32 actionindex        ,
                                      const CORE::CString& actionName )
{GUCEF_TRACE;
        if ( keyindex >= m_keyboardmapcount )
        {
                UInt32 oldmax( m_keyboardmapcount );
                m_keyboardmapcount = keyindex+1;
                m_keyboardkdownmap = (UInt32*) realloc( m_keyboardkdownmap, m_keyboardmapcount * sizeof(UInt32) );
                m_keyboardkupmap = (UInt32*) realloc( m_keyboardkupmap, m_keyboardmapcount * sizeof(UInt32) );
                memset( m_keyboardkdownmap+oldmax, 0, m_keyboardmapcount-oldmax );
                memset( m_keyboardkupmap+oldmax, 0, m_keyboardmapcount-oldmax );
                m_keyboardKDownActionNames.AddAtIndex( actionName, actionindex );
        }
        
        m_keyboardkupmap[ keyindex ] = actionindex;
}                                      

/*-------------------------------------------------------------------------*/
                                    
UInt32 
CInputActionMap::GetKeyboardKeyUpMap( const UInt32 keyindex )
{GUCEF_TRACE;
        return m_keyboardkupmap[ keyindex ];
}

/*-------------------------------------------------------------------------*/

bool
CInputActionMap::EnsureDeviceMaxCapacity( const UInt32 devicemax )
{GUCEF_TRACE;
        if ( devicemax >= m_devicecount )
        {
                UInt32 oldmax( m_devicecount );
                m_devicecount = devicemax;
                
                m_deviceboolonmap = (UInt32**) realloc( m_deviceboolonmap, m_devicecount * sizeof(UInt32*) );
                m_devicebooloffmap = (UInt32**) realloc( m_devicebooloffmap, m_devicecount * sizeof(UInt32*) );
                m_devicevarchangemap = (UInt32**) realloc( m_devicevarchangemap, m_devicecount * sizeof(UInt32*) );
                for ( UInt32 i=oldmax; i<m_devicecount; ++i )
                {
                        m_deviceboolonmap[ i ] = (UInt32*) malloc( m_deviceboolmapcount * sizeof(UInt32) );
                        memset( m_deviceboolonmap[ i ], 0, m_deviceboolmapcount * sizeof(UInt32) );
                        m_devicebooloffmap[ i ] = (UInt32*) malloc( m_deviceboolmapcount * sizeof(UInt32) );
                        memset( m_devicebooloffmap[ i ], 0, m_deviceboolmapcount * sizeof(UInt32) );
                        m_devicevarchangemap[ i ] = (UInt32*) malloc( m_devicevarmapcount * sizeof(UInt32) );
                        memset( m_devicevarchangemap[ i ], 0, m_devicevarmapcount * sizeof(UInt32) );
                }
                return true;                
        }
        return false;
}

/*-------------------------------------------------------------------------*/

void
CInputActionMap::EnsureDeviceBoolStateMaxCapacity( const UInt32 deviceindex ,
                                                   const UInt32 statemax    )
{GUCEF_TRACE;
        if ( !EnsureDeviceMaxCapacity( deviceindex+1 ) )
        {
                if ( statemax >= m_deviceboolmapcount )
                {
                        UInt32 oldmax( m_deviceboolmapcount );
                        m_deviceboolmapcount = statemax+1;                        
                        m_deviceboolonmap[ deviceindex ] = (UInt32*) realloc( m_deviceboolonmap[ deviceindex ], m_deviceboolmapcount * sizeof(UInt32) );
                        memset( m_deviceboolonmap[ deviceindex ]+oldmax, 0, m_deviceboolmapcount * sizeof(UInt32) );
                        m_devicebooloffmap[ deviceindex ] = (UInt32*) realloc( m_devicebooloffmap[ deviceindex ], m_deviceboolmapcount * sizeof(UInt32) );
                        memset( m_devicebooloffmap[ deviceindex ]+oldmax, 0, m_deviceboolmapcount * sizeof(UInt32) );
                }                        
        }                
}

/*-------------------------------------------------------------------------*/

void
CInputActionMap::EnsureDeviceVarStateMaxCapacity( const UInt32 deviceindex ,
                                                  const UInt32 statemax    )
{GUCEF_TRACE;
        if ( !EnsureDeviceMaxCapacity( deviceindex+1 ) )
        {
                if ( statemax >= m_devicevarmapcount )
                {
                        UInt32 oldmax( m_devicevarmapcount );
                        m_devicevarmapcount = statemax+1;                        
                        m_devicevarchangemap[ deviceindex ] = (UInt32*) realloc( m_devicevarchangemap[ deviceindex ], m_devicevarmapcount * sizeof(UInt32) );
                        memset( m_devicevarchangemap[ deviceindex ]+oldmax, 0, m_devicecount * sizeof(UInt32) );
                }                        
        }                
}

/*-------------------------------------------------------------------------*/
                
void 
CInputActionMap::SetDeviceBooleanOnMap( const UInt32 deviceindex ,
                                        const UInt32 stateindex  ,
                                        const UInt32 actionindex )
{GUCEF_TRACE;
        EnsureDeviceBoolStateMaxCapacity( deviceindex, stateindex+1 );
        
        m_deviceboolonmap[ deviceindex ][ stateindex ] = actionindex;
}                                        

/*-------------------------------------------------------------------------*/
                                    
UInt32 
CInputActionMap::GetDeviceBooleanOnMap( const UInt32 deviceindex ,
                                        const UInt32 stateindex  )
{GUCEF_TRACE;
        return m_deviceboolonmap[ deviceindex ][ stateindex ];
}                                        

/*-------------------------------------------------------------------------*/
                                      
void 
CInputActionMap::SetDeviceBooleanOffMap( const UInt32 deviceindex ,
                                         const UInt32 stateindex  ,
                                         const UInt32 actionindex )
{GUCEF_TRACE;
        EnsureDeviceBoolStateMaxCapacity( deviceindex, stateindex+1 );
        
        m_devicebooloffmap[ deviceindex ][ stateindex ] = actionindex;
}                                         

/*-------------------------------------------------------------------------*/
                                    
UInt32 
CInputActionMap::GetDeviceBooleanOffMap( const UInt32 deviceindex ,
                                         const UInt32 stateindex  )
{GUCEF_TRACE;
        return m_devicebooloffmap[ deviceindex ][ stateindex ];
}                                         

/*-------------------------------------------------------------------------*/
                                       
void 
CInputActionMap::SetDeviceVariableChangedMap( const UInt32 deviceindex ,
                                              const UInt32 stateindex  ,
                                              const UInt32 actionindex )
{GUCEF_TRACE;
        EnsureDeviceVarStateMaxCapacity( deviceindex, stateindex+1 );
        
        m_deviceboolonmap[ deviceindex ][ stateindex ] = actionindex;
}                                              

/*-------------------------------------------------------------------------*/
                                          
UInt32 
CInputActionMap::GetDeviceVariableChangedMap( const UInt32 deviceid    ,
                                              const UInt32 stateindex  )
{GUCEF_TRACE;
        return m_devicevarchangemap[ deviceid ][ stateindex ];
}                                              

/*-------------------------------------------------------------------------*/

void
CInputActionMap::Reset( void )
{GUCEF_TRACE;
        memset( m_mousebupmap, 0, m_mousemapcount * sizeof(UInt32) );
        memset( m_mousebdownmap, 0, m_mousemapcount * sizeof(UInt32) );
        memset( m_keyboardkupmap, 0, m_keyboardmapcount * sizeof(UInt32) );
        memset( m_keyboardkdownmap, 0, m_keyboardmapcount * sizeof(UInt32) );
        
        for ( UInt32 i=0; i<m_deviceboolmapcount; ++i )
        {
                memset( m_deviceboolonmap[ i ], 0, m_deviceboolmapcount * sizeof(UInt32) );
                memset( m_devicebooloffmap[ i ], 0, m_deviceboolmapcount * sizeof(UInt32) );
        }
        for ( UInt32 i=0; i<m_devicevarmapcount; ++i )
        {                
                memset( m_devicevarchangemap[ i ], 0, m_devicevarmapcount * sizeof(UInt32) );
        }                
}

/*-------------------------------------------------------------------------*/

void
CInputActionMap::Clear( void )
{GUCEF_TRACE;
        free( m_mousebupmap );
        m_mousebupmap = NULL;
        free( m_mousebdownmap );
        m_mousebdownmap = NULL;
        free( m_keyboardkupmap );
        m_keyboardkupmap = NULL;
        free( m_keyboardkdownmap );
        m_keyboardkdownmap = NULL;
        
        for ( UInt32 n=0; n<m_devicecount; ++n )
        {
                free( m_deviceboolonmap[ n ] );
                free( m_devicebooloffmap[ n ] );
                free( m_devicevarchangemap[ n ] );
        }                
        free( m_deviceboolonmap );
        m_deviceboolonmap = NULL;
        free( m_devicebooloffmap );
        m_devicebooloffmap = NULL;
        free( m_devicevarchangemap );
        m_devicevarchangemap = NULL;
        
        m_mouseBUpActionNames.Clear();
        m_mouseBDownActionNames.Clear();
        m_keyboardKUpActionNames.Clear();
        m_keyboardKDownActionNames.Clear();
                
        m_mousemapcount = 0;
        m_keyboardmapcount = 0;
        m_deviceboolmapcount = 0;
        m_devicevarmapcount = 0;
        m_deviceboolmapcount = 0;
        m_devicevarmapcount = 0;
        m_devicecount = 0;        
}

/*-------------------------------------------------------------------------*/
                                            
bool 
CInputActionMap::SaveConfig( CORE::CDataNode& tree )
{GUCEF_TRACE;
        CORE::CString intstr;
        CORE::CDataNode newnode( "InputActionMap" );
        
        intstr.SetInt( m_mousemapcount );
        newnode.SetAttribute( "MouseButtonMapCount" ,
                              intstr                );

        intstr.SetInt( m_keyboardmapcount );
        newnode.SetAttribute( "KeyboardKeyMapCount" ,
                              intstr                );
                           
        intstr.SetInt( m_deviceboolmapcount );
        newnode.SetAttribute( "DeviceBoolMapCount" ,
                              intstr               );                           

        intstr.SetInt( m_devicevarmapcount );
        newnode.SetAttribute( "DeviceVarMapCount" ,
                              intstr              );

        CORE::CDataNode* listRoot = tree.AddChild( newnode );

        CORE::CString actionIndexStr( "ActionIndex" );
        CORE::CString actionNameStr( "ActionName" );
        CORE::CString buttonIndexStr( "ButtonIndex" );
        CORE::CString keyIndexStr( "KeyIndex" );

        newnode.SetName( "MouseButtonUp" );
        for ( UInt32 i=0; i<m_mousemapcount; ++i )
        {
                newnode.SetAttribute( buttonIndexStr, CORE::Int32ToString( i ) );
                newnode.SetAttribute( actionIndexStr, CORE::Int32ToString( m_mousebupmap[ i ] ) );
                newnode.SetAttribute( actionNameStr, m_mouseBUpActionNames[ i ] );
                
                listRoot->AddChild( newnode );
        }
        newnode.SetName( "MouseButtonDown" );
        for ( UInt32 i=0; i<m_mousemapcount; ++i )
        {
                newnode.SetAttribute( buttonIndexStr, CORE::Int32ToString( i ) );
                newnode.SetAttribute( actionIndexStr, CORE::Int32ToString( m_mousebdownmap[ i ] ) );
                newnode.SetAttribute( actionNameStr, m_mouseBDownActionNames[ i ] );
                
                listRoot->AddChild( newnode );
        }        
        newnode.SetName( "KeyboardKeyUp" );
        for ( UInt32 i=0; i<m_keyboardmapcount; ++i )
        {
                newnode.SetAttribute( keyIndexStr, CORE::Int32ToString( i ) );
                newnode.SetAttribute( actionIndexStr, CORE::Int32ToString( m_keyboardkupmap[ i ] ) );
                newnode.SetAttribute( actionNameStr, m_keyboardKUpActionNames[ i ] );
                
                listRoot->AddChild( newnode );
        }
        newnode.SetName( "KeyboardKeyDown" );
        for ( UInt32 i=0; i<m_keyboardmapcount; ++i )
        {
                newnode.SetAttribute( keyIndexStr, CORE::Int32ToString( i ) );
                newnode.SetAttribute( actionIndexStr, CORE::Int32ToString( m_keyboardkdownmap[ i ] ) );
                newnode.SetAttribute( actionNameStr, m_keyboardKDownActionNames[ i ] );
                
                listRoot->AddChild( newnode );
        }                
        return true;                                    
}

/*-------------------------------------------------------------------------*/
                                                               
bool
CInputActionMap::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;
        Clear();
        
        if ( treeroot.GetName() == "InputActionMap" )
        {
                CORE::CString intstr;
                const CORE::CDataNode::TNodeAtt* att;
                UInt32 mouseButtonCount( 0UL );
                UInt32 keyboardKeyCount( 0UL );
                UInt32 deviceBoolCount( 0UL );
                UInt32 deviceVarCount( 0UL );
                
                att = treeroot.GetAttribute( "name" );
                if ( att )
                {
                        m_name = att->value;                                             
                }
                                
                CORE::CString actionIndexStr( "ActionIndex" );
                CORE::CString actionNameStr( "ActionName" );
                CORE::CString buttonIndexStr( "ButtonIndex" );                
                CORE::CString mouseButtonUpStr( "MouseButtonUp" );
                CORE::CString mouseButtonDownStr( "MouseButtonDown" );
                CORE::CString keyIndexStr( "KeyIndex" );
                CORE::CString keyboardKeyUpStr( "KeyboardKeyUp" );
                CORE::CString keyboardKeyDownStr( "KeyboardKeyDown" );
                                
                UInt32 inputIndex;
                UInt32 actionIndex;
                CORE::CString actionName;
                const CORE::CDataNode *cn;
                CORE::CDataNode::const_iterator i( treeroot.ConstBegin() );
                
                while ( i != treeroot.ConstEnd() )
                {
                        cn = (*i);
                        
                        if ( cn->GetName() == mouseButtonUpStr )
                        {
                                att = cn->GetAttribute( buttonIndexStr );
                                if ( att )
                                {
                                        inputIndex = att->value.GetInt();
                                        att = cn->GetAttribute( actionIndexStr );
                                        if ( att )
                                        {
                                                actionIndex = att->value.GetInt();
                                                att = cn->GetAttribute( actionNameStr );
                                                if ( att )
                                                {
                                                        actionName = att->value;
                                                        if ( actionName.Length() > 0 )
                                                        {
                                                                SetMouseButtonUpMap( inputIndex  ,
                                                                                     actionIndex ,
                                                                                     actionName  );
                                                        }        
                                                }        
                                        }
                                }
                        }
                        else
                        if ( cn->GetName() == mouseButtonDownStr )
                        {
                                att = cn->GetAttribute( buttonIndexStr );
                                if ( att )
                                {
                                        inputIndex = att->value.GetInt();
                                        att = cn->GetAttribute( actionIndexStr );
                                        if ( att )
                                        {
                                                actionIndex = att->value.GetInt();
                                                att = cn->GetAttribute( actionNameStr );
                                                if ( att )
                                                {
                                                        actionName = att->value;
                                                        if ( actionName.Length() > 0 )
                                                        {
                                                                SetMouseButtonDownMap( inputIndex  ,
                                                                                       actionIndex ,
                                                                                       actionName  );
                                                        }        
                                                }        
                                        }
                                }
                        }
                        else
                        if ( cn->GetName() == keyboardKeyUpStr )
                        {
                                att = cn->GetAttribute( keyIndexStr );
                                if ( att )
                                {
                                        inputIndex = att->value.GetInt();
                                        att = cn->GetAttribute( actionIndexStr );
                                        if ( att )
                                        {
                                                actionIndex = att->value.GetInt();
                                                att = cn->GetAttribute( actionNameStr );
                                                if ( att )
                                                {
                                                        actionName = att->value;
                                                        if ( actionName.Length() > 0 )
                                                        {
                                                                SetKeyboardKeyUpMap( inputIndex  ,
                                                                                     actionIndex ,
                                                                                     actionName  );
                                                        }        
                                                }        
                                        }
                                }
                        }
                        else
                        if ( cn->GetName() == keyboardKeyDownStr )
                        {
                                att = cn->GetAttribute( keyIndexStr );
                                if ( att )
                                {
                                        inputIndex = att->value.GetInt();
                                        att = cn->GetAttribute( actionIndexStr );
                                        if ( att )
                                        {
                                                actionIndex = att->value.GetInt();
                                                att = cn->GetAttribute( actionNameStr );
                                                if ( att )
                                                {
                                                        actionName = att->value;
                                                        if ( actionName.Length() > 0 )
                                                        {
                                                                SetKeyboardKeyDownMap( inputIndex  ,
                                                                                       actionIndex ,
                                                                                       actionName  );
                                                        }        
                                                }        
                                        }
                                }
                        }                                                                              
                        ++i;
                }                
           
        }
        return true;
}

/*-------------------------------------------------------------------------*/

UInt32 
CInputActionMap::GetMouseButtonMaxMapCount( void ) const
{GUCEF_TRACE;
        return m_mousemapcount;
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CInputActionMap::GetKeyboardKeyMaxMapCount( void ) const
{GUCEF_TRACE;
        return m_keyboardmapcount;
}

/*-------------------------------------------------------------------------*/

UInt32 
CInputActionMap::GetDeviceMaxMapCount( void ) const
{GUCEF_TRACE;
        return m_devicecount;
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CInputActionMap::GetDeviceMaxBoolMapCount( void ) const
{GUCEF_TRACE;
        return m_deviceboolmapcount;
}
        
/*-------------------------------------------------------------------------*/

UInt32 
CInputActionMap::GetDeviceMaxVarMapCount( void ) const
{GUCEF_TRACE;
        return m_devicevarmapcount;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CInputActionMap::GetName( void ) const
{GUCEF_TRACE;
        return m_name;
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CInputActionMap::GetMouseButtonDownActionNames( void ) const
{GUCEF_TRACE;
        return m_mouseBDownActionNames;
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CInputActionMap::GetMouseButtonUpActionNames( void ) const
{GUCEF_TRACE;
        return m_mouseBUpActionNames;
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CInputActionMap::GetKeyboardKeyUpActionNames( void ) const
{GUCEF_TRACE;
        return m_keyboardKUpActionNames;
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CInputActionMap::GetKeyboardKeyDownActionNames( void ) const
{GUCEF_TRACE;
        return m_keyboardKDownActionNames;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

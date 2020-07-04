/*
 *  gucefPRODMAN: Product management module
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_PRODMAN_CPRODUCTINFO_H
#define GUCEF_PRODMAN_CPRODUCTINFO_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ESTRUCTS_H
#include "EStructs.h"
#define GUCEF_CORE_ESTRUCTS_H
#endif /* GUCEF_CORE_ESTRUCTS_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_PRODMAN_MACROS_H
#include "gucefPRODMAN_macros.h"
#define GUCEF_PRODMAN_MACROS_H
#endif /* GUCEF_PRODMAN_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PRODMAN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_PRODMAN_PUBLIC_CPP CProductInfo : public CORE::CIConfigurable
{
    public:
    
    enum EDeploymentStatus
    {
        DEPLOYMENTSTATE_INSTALLED            ,
        DEPLOYMENTSTATE_UPDATING             ,
        DEPLOYMENTSTATE_DOWNLOADING          ,
        DEPLOYMENTSTATE_AVAILABLEFORDOWNLOAD ,
        
        DEPLOYMENTSTATE_UNKNOWN
    };
    typedef enum EDeploymentStatus TDeploymentStatus;

    enum EProductType
    {
        PRODUCTTYPE_INDEPENDANT  ,
        PRODUCTTYPE_MODIFICATION ,
        PRODUCTTYPE_EXPANSION    ,
        PRODUCTTYPE_PLUGIN       ,
        
        PRODUCTTYPE_UNKNOWN
    };
    typedef enum EProductType TProductType;
    
    CProductInfo( void );
    
    CProductInfo( const CProductInfo& src );
    
    virtual ~CProductInfo();
    
    CProductInfo& operator=( const CProductInfo& src );

    void SetParentName( const CString& name );
    
    const CString& GetParentName( void ) const;
    
    void SetName( const CString& name );
    
    const CString& GetName( void ) const;
    
    void SetDescription( const CString& description );
    
    const CString& GetDescription( void ) const;
    
    void SetInfoURL( const CString& infoURL );
    
    const CString& GetInfoURL( void ) const; 
    
    void SetPatchListURL( const CString& patchListURL );
    
    const CString& GetPatchListURL( void ) const;
    
    void SetDeploymentState( const TDeploymentStatus deploymentState );
    
    const TDeploymentStatus GetDeploymentState( void ) const;
    
    void SetProductType( const TProductType productType );
    
    const TProductType GetProductType( void ) const;
    
    void SetVersion( const GUCEF::CORE::TVersion& version );
    
    const GUCEF::CORE::TVersion& GetVersion( void ) const;
    
    CString GetCombinedProductString( void ) const;
    
    bool operator<( const CProductInfo& other ) const;

    virtual bool SaveConfig( GUCEF::CORE::CDataNode& node ) const;
                                
    virtual bool LoadConfig( const GUCEF::CORE::CDataNode& node );

    virtual const CString& GetClassTypeName( void ) const;
    
    static CString ProductTypeToString( const TProductType productType );
    
    static const TProductType StringToProductType( const CString& productType );
    
    static CString DeploymentStateToString( TDeploymentStatus deploymentState );
    
    static const TDeploymentStatus StringToDeploymentState( const CString& deploymentState );
    
    private:
    
    CString m_name;
    CString m_parentName;
    CString m_description;
    CString m_infoURL;
    CString m_patchListURL;
    GUCEF::CORE::TVersion m_version;
    TDeploymentStatus m_deploymentState;
    TProductType m_productType;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PRODMAN */
}; /* namespace GUCEF*/

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PRODMAN_CPRODUCTINFO_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-02-2008 :
        - Initial implementation
          
---------------------------------------------------------------------------*/

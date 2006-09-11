/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */
 
#ifndef GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H
#define GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string>
#include <map>
#include "gucefCORE_macros.h"
#include "gucefCORE_ETypes.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_CPP
    #pragma warning( push )
#endif

#pragma warning( disable: 4251 ) // 'classname' needs to have dll-interface to be used by clients of class 'classname'
#pragma warning( disable: 4284 ) // return type for operator -> is 'const *' (ie; not a UDT or reference to a UDT).
#pragma warning( disable: 4786 ) // identifier was truncated to 'number' characters

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

class CExclusiveActiveObj;

/*-------------------------------------------------------------------------*/

class EXPORT_CPP CExclusiveActivationManager
{
    public:
    
    static CExclusiveActivationManager* Instance( void );
    
    bool SetActivateObj( const std::string& objTypeName ,
                         const std::string& objName     );
    
    bool DisableActivateObj( const std::string& objTypeName );

    CExclusiveActiveObj* GetActiveObject( const std::string& objTypeName ) const;
    
    bool SetDefaultObjForType( const std::string& objTypeName ,
                               const std::string& objName     );

    bool GetDefaultObjForType( const std::string& objTypeName ,
                               std::string& objName           );
    
    UInt32 GetRegisteredTypeCount( void ) const;    

    private:
    friend class CExclusiveActiveObj;
    
    void RegisterObj( CExclusiveActiveObj* newObj );
    
    void UnregisterObj( CExclusiveActiveObj* theObj );
    
    private:
    friend class CXBGeneralModule;
    
    static void Deinstance( void );

    private:

    CExclusiveActivationManager( void );
    CExclusiveActivationManager( const CExclusiveActivationManager& src );
    ~CExclusiveActivationManager();
    CExclusiveActivationManager& operator=( const CExclusiveActivationManager& src );
    
    private:
    typedef std::map< std::string, CExclusiveActiveObj* >   TTypedObjList;
    struct SExclusiveObjList
    {
        TTypedObjList objList;
        std::string defaultObj;
        CExclusiveActiveObj* activeObj;
    };
    typedef struct SExclusiveObjList                        TExclusiveObjList;
    typedef std::map< std::string, TExclusiveObjList >      TObjTypeList;
    
    TObjTypeList m_typeList;

    private:

    static CExclusiveActivationManager* m_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H  ? */

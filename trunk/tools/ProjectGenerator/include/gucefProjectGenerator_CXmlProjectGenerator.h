/*
 *  ProjectGenerator: Tool to generate module/project files
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#ifndef GUCEF_PROJECTGENERATOR_CXMLPROJECTGENERATOR_H
#define GUCEF_PROJECTGENERATOR_CXMLPROJECTGENERATOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PROJECTGENERATOR_CIPROJECTGENERATOR_H
#include "gucefProjectGenerator_CIProjectGenerator.h"
#define GUCEF_PROJECTGENERATOR_CIPROJECTGENERATOR_H
#endif /* GUCEF_PROJECTGENERATOR_CIPROJECTGENERATOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PROJECTGENERATOR {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CXmlProjectGenerator : public CIProjectGenerator
{
    public:
    
    CXmlProjectGenerator( void );
    
    CXmlProjectGenerator( const CXmlProjectGenerator& src );
    
    virtual ~CXmlProjectGenerator();
    
    virtual bool GenerateProject( TProjectInfo& projectInfo            ,
                                  const CORE::CString& outputDir       ,
                                  bool addGeneratorCompileTimeToOutput );
                                  
    private:
    
    CXmlProjectGenerator& operator=( const CXmlProjectGenerator& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGENERATOR */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PROJECTGENERATOR_CXMLPROJECTGENERATOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/

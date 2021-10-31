/*
 *  pluginglueAWSSDK: Library to support multiple AWS SDK based plugins that share overlap
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

#ifndef GUCEF_PLUGINGLUE_AWSSDK_CLOGGINGADAPTER_H
#define GUCEF_PLUGINGLUE_AWSSDK_CLOGGINGADAPTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PLUGINGLUE_AWSSDK_MACROS_H
#include "pluginglueAWSSDK_macros.h"
#define GUCEF_PLUGINGLUE_AWSSDK_MACROS_H
#endif /* GUCEF_PLUGINGLUE_AWSSDK_MACROS_H ? */

#ifndef GUCEF_DYNNEWOFF_H
#include "gucef_dynnewoff.h"
#define GUCEF_DYNNEWOFF_H
#endif /* GUCEF_DYNNEWOFF_H ? */

#include <aws/core/Aws.h>
#include <aws/core/utils/logging/FormattedLogSystem.h>

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PLUGINGLUE {
namespace AWSSDK {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_HIDDEN CLoggingAdapter : public Aws::Utils::Logging::LogSystemInterface
{
    public:

    CLoggingAdapter( void );

    virtual ~CLoggingAdapter();

    virtual void Flush() override;

    virtual Aws::Utils::Logging::LogLevel GetLogLevel( void ) const override;

    virtual void Log( Aws::Utils::Logging::LogLevel logLevel , 
                      const char* tag                        , 
                      const char* formatStr                  , 
                      ...                                    ) override;

    virtual void LogStream( Aws::Utils::Logging::LogLevel logLevel  , 
                            const char* tag                         , 
                            const Aws::OStringStream& messageStream ) override;

    static Aws::Utils::Logging::LogLevel MapLogLevel( CORE::TLogMsgType logType , 
                                                      CORE::Int32 logLevel      );
    
    static void MapLogLevel( Aws::Utils::Logging::LogLevel logLevel ,
                             CORE::TLogMsgType& gupLogType          , 
                             CORE::Int32& gupLogLevel               );
    
    private:
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSDK */
}; /* namespace PLUGINGLUE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLUGINGLUE_AWSSDK_CLOGGINGADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 04-05-2005 :
        - Dinand: Initial version.

---------------------------------------------------------------------------*/

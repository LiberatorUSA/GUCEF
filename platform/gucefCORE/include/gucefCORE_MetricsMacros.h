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

#ifndef GUCEF_CORE_METRICSMACROS_H
#define GUCEF_CORE_METRICSMACROS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_NO_METRICS

#ifndef GUCEF_CORE_CMETRICSCLIENTMANAGER_H
#include "gucefCORE_CMetricsClientManager.h"
#define GUCEF_CORE_CMETRICSCLIENTMANAGER_H
#endif /* GUCEF_CORE_CMETRICSCLIENTMANAGER_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#endif /* GUCEF_NO_METRICS ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  We allow all metrics calls to be removed with the define 'GUCEF_NO_METRICS'
 */
#ifndef GUCEF_NO_METRICS

/*-------------------------------------------------------------------------*/
                                     
#undef GUCEF_METRIC_INCREMENT
#define GUCEF_METRIC_INCREMENT( key, frequency ) ::GUCEF::CORE::CCoreGlobal::Instance()->GetMetricsClientManager().Increment( key, frequency );

/*-------------------------------------------------------------------------*/

#undef GUCEF_METRIC_DECREMENT
#define GUCEF_METRIC_DECREMENT( key, frequency ) ::GUCEF::CORE::CCoreGlobal::Instance()->GetMetricsClientManager().Decrement( key, frequency );

/*-------------------------------------------------------------------------*/

#undef GUCEF_METRIC_COUNT
#define GUCEF_METRIC_COUNT( key, delta, frequency ) ::GUCEF::CORE::CCoreGlobal::Instance()->GetMetricsClientManager().Count( key, delta, frequency );

/*-------------------------------------------------------------------------*/

#undef GUCEF_METRIC_GAUGE
#define GUCEF_METRIC_GAUGE( key, value, frequency ) ::GUCEF::CORE::CCoreGlobal::Instance()->GetMetricsClientManager().Gauge( key, value, frequency );

/*-------------------------------------------------------------------------*/

#undef GUCEF_METRIC_TIMING
#define GUCEF_METRIC_TIMING( key, ms, frequency ) ::GUCEF::CORE::CCoreGlobal::Instance()->GetMetricsClientManager().Timing( key, ms, frequency );

/*-------------------------------------------------------------------------*/

#else /* GUCEF_NO_METRICS ? */

#undef GUCEF_METRIC_INCREMENT
#undef GUCEF_METRIC_DECREMENT
#undef GUCEF_METRIC_COUNT
#undef GUCEF_METRIC_GAUGE
#undef GUCEF_METRIC_TIMING

#define GUCEF_METRIC_INCREMENT( key, frequency ) { }
#define GUCEF_METRIC_DECREMENT( key, frequency ) { }
#define GUCEF_METRIC_COUNT( key, delta, frequency ) { }
#define GUCEF_METRIC_GAUGE( key, value, frequency ) { }
#define GUCEF_METRIC_TIMING( logLevel, ms, logMessage ) { }

#endif /* GUCEF_NO_METRICS ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/

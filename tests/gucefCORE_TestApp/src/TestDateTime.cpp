/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
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

#include <time.h>

#ifndef GUCEF_CORE_CDATETIME_H
#include "gucefCORE_CDateTime.h"
#define GUCEF_CORE_CDATETIME_H
#endif /* GUCEF_CORE_CDATETIME_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestDateTime.h"

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static const CORE::CString testDateTimeString1 = "2020-07-10 15:01:02.300";
static const GUCEF::vector< Int32 > testDateTimeString1Result = { 2020, 7, 10, 15, 1, 2, 300 };
static const Int32 testDateTimeString1TzOffsetResult = 0;

static const CORE::CString testDateTimeString2 = "2013-07-16T19:23:51Z";
static const GUCEF::vector< Int32 > testDateTimeString2Result = { 2013, 7, 16, 19, 23, 51, 0 };
static const Int32 testDateTimeString2TzOffsetResult = 0;

static const CORE::CString testDateTimeString3 = "2020-07-10 15:01:02.300Z";
static const GUCEF::vector< Int32 > testDateTimeString3Result = { 2020, 7, 10, 15, 1, 2, 300 };
static const Int32 testDateTimeString3TzOffsetResult = 0;

static const CORE::CString testDateTimeString4 = "2020/07/10 15:01:02.300Z";
static const GUCEF::vector< Int32 > testDateTimeString4Result = { 2020, 7, 10, 15, 1, 2, 300 };
static const Int32 testDateTimeString4TzOffsetResult = 0;

static const CORE::CString testDateTimeString5 = "2020:07:10 15:01:02.300Z";
static const GUCEF::vector< Int32 > testDateTimeString5Result = { 2020, 7, 10, 15, 1, 2, 300 };
static const Int32 testDateTimeString5TzOffsetResult = 0;

static const CORE::CString testDateTimeString6 = "2020.07.10 15.01.02.300Z";
static const GUCEF::vector< Int32 > testDateTimeString6Result = { 2020, 7, 10, 15, 1, 2, 300 };
static const Int32 testDateTimeString6TzOffsetResult = 0;

static const CORE::CString testDateTimeString7 = "+20:07:10";
static const GUCEF::vector< Int32 > testDateTimeString7Result = { 0, 0, 0, 20, 7, 10, 0 };
static const Int32 testDateTimeString7TzOffsetResult = 0;

static const CORE::CString testDateTimeString8 = "-20:07:10";
static const GUCEF::vector< Int32 > testDateTimeString8Result = { 0, 0, 0, -20, -07, -10, 0 };
static const Int32 testDateTimeString8TzOffsetResult = 0;

static const CORE::CString testDateTimeString9 = "+20:07:10.300";
static const GUCEF::vector< Int32 > testDateTimeString9Result = { 0, 0, 0, 20, 7, 10, 300 };
static const Int32 testDateTimeString9TzOffsetResult = 0;

static const CORE::CString testDateTimeString10 = "2013-07-16T19:23:51+04:30";
static const GUCEF::vector< Int32 > testDateTimeString10Result = { 2013, 7, 16, 19, 23, 51, 0 };
static const Int32 testDateTimeString10TzOffsetResult = 30+(4*60);

static const CORE::CString testDateTimeString11 = "2013-07-16T19:23:51-04:30";
static const GUCEF::vector< Int32 > testDateTimeString11Result = { 2013, 7, 16, 19, 23, 51, 0 };
static const Int32 testDateTimeString11TzOffsetResult = -(30+(4*60));

static const CORE::CString testDateTimeString12 = "2020/07/10";
static const GUCEF::vector< Int32 > testDateTimeString12Result = { 2020, 7, 10, 0, 0, 0, 0 };
static const Int32 testDateTimeString12TzOffsetResult = 0;

static const CORE::CString testDateTimeString13 = "2020-07-10";
static const GUCEF::vector< Int32 > testDateTimeString13Result = { 2020, 7, 10, 0, 0, 0, 0 };
static const Int32 testDateTimeString13TzOffsetResult = 0;

static const CORE::CString testDateTimeString14 = "2020 07 10";
static const GUCEF::vector< Int32 > testDateTimeString14Result = { 2020, 7, 10, 0, 0, 0, 0 };
static const Int32 testDateTimeString14TzOffsetResult = 0;

static const CORE::CString testDateTimeString15 = "20 07 10";
static const GUCEF::vector< Int32 > testDateTimeString15Result = { 0, 0, 0, 20, 7, 10, 0 };
static const Int32 testDateTimeString15TzOffsetResult = 0;

static const CORE::CString testDateTimeString16 = "200710";
static const GUCEF::vector< Int32 > testDateTimeString16Result = { 0, 0, 0, 20, 7, 10, 0 };
static const Int32 testDateTimeString16TzOffsetResult = 0;

static const CORE::CString testDateTimeString17 = "20200710";
static const GUCEF::vector< Int32 > testDateTimeString17Result = { 2020, 7, 10, 0, 0, 0, 0 };
static const Int32 testDateTimeString17TzOffsetResult = 0;

static const CORE::CString testDateTimeString18 = "20200710060730";
static const GUCEF::vector< Int32 > testDateTimeString18Result = { 2020, 7, 10, 6, 7, 30, 0 };
static const Int32 testDateTimeString18TzOffsetResult = 0;

static const CORE::CString testDateTimeString19 = "20200710060730.600";
static const GUCEF::vector< Int32 > testDateTimeString19Result = { 2020, 7, 10, 6, 7, 30, 600 };
static const Int32 testDateTimeString19TzOffsetResult = 0;

static const CORE::CString testDateTimeString20 = "2020-7-1";
static const GUCEF::vector< Int32 > testDateTimeString20Result = { 2020, 7, 1, 0, 0, 0, 0 };
static const Int32 testDateTimeString20TzOffsetResult = 0;

static const CORE::CString testDateTimeString21 = "+20:7:1";
static const GUCEF::vector< Int32 > testDateTimeString21Result = { 0, 0, 0, 20, 7, 1, 0 };
static const Int32 testDateTimeString21TzOffsetResult = 0;

static const CORE::CString testDateTimeString22 = "2020-7-1 15:1:2.300";
static const GUCEF::vector< Int32 > testDateTimeString22Result = { 2020, 7, 1, 15, 1, 2, 300 };
static const Int32 testDateTimeString22TzOffsetResult = 0;

static const CORE::CString testDateTimeString23 = "2020-7-1 15:1:2.300+04:30";
static const GUCEF::vector< Int32 > testDateTimeString23Result = { 2020, 7, 1, 15, 1, 2, 300 };
static const Int32 testDateTimeString23TzOffsetResult = 30+(4*60);

static const CORE::CString testDateTimeString24 = "2020-7-1 15:1:2.300-4:30";
static const GUCEF::vector< Int32 > testDateTimeString24Result = { 2020, 7, 1, 15, 1, 2, 300 };
static const Int32 testDateTimeString24TzOffsetResult = -(30+(4*60));

static const CORE::CString testDateTimeString25 = "2024-265";
static const GUCEF::vector< Int32 > testDateTimeString25Result = { 2024, 9, 21, 0, 0, 0, 0 };
static const Int32 testDateTimeString25TzOffsetResult = 0;

static const CORE::CString testDateTimeString26 = "2013-07-16T19:23:51Z\0This is a test, this should not be included";
static const GUCEF::vector< Int32 > testDateTimeString26Result = { 2013, 7, 16, 19, 23, 51, 0 };
static const Int32 testDateTimeString26TzOffsetResult = 0;

static const CORE::CString testDateTimeString27 = "2013-07-16T19:23:51Z This is a test, this should not be included";
static const GUCEF::vector< Int32 > testDateTimeString27Result = { 2013, 7, 16, 19, 23, 51, 0 };
static const Int32 testDateTimeString27TzOffsetResult = 0;

static const CORE::CString testDateTimeString28 = "2013-07-16T19:23:51Z. This is a test, this should not be included";
static const GUCEF::vector< Int32 > testDateTimeString28Result = { 2013, 7, 16, 19, 23, 51, 0 };
static const Int32 testDateTimeString28TzOffsetResult = 0;

static const CORE::CString testDateTimeString29 = "20241017090031.330Z";
static const GUCEF::vector< Int32 > testDateTimeString29Result = { 2024, 10, 17, 9, 0, 31, 330 };
static const Int32 testDateTimeString29TzOffsetResult = 0;

struct StringParseTestCase
{
    const CORE::CString& dateTimeString;
    const GUCEF::vector< Int32 >& dateTimeStringResult;
    const Int32& tzOffsetResult;
};

static const GUCEF::vector< StringParseTestCase > stringParseTestCases = {
    { testDateTimeString1, testDateTimeString1Result, testDateTimeString1TzOffsetResult },
    { testDateTimeString2, testDateTimeString2Result, testDateTimeString2TzOffsetResult },
    { testDateTimeString3, testDateTimeString3Result, testDateTimeString3TzOffsetResult },
    { testDateTimeString4, testDateTimeString4Result, testDateTimeString4TzOffsetResult },
    { testDateTimeString5, testDateTimeString5Result, testDateTimeString5TzOffsetResult },
    { testDateTimeString6, testDateTimeString6Result, testDateTimeString6TzOffsetResult },
    { testDateTimeString7, testDateTimeString7Result, testDateTimeString7TzOffsetResult },
    { testDateTimeString8, testDateTimeString8Result, testDateTimeString8TzOffsetResult },
    { testDateTimeString9, testDateTimeString9Result, testDateTimeString9TzOffsetResult },
    { testDateTimeString10, testDateTimeString10Result, testDateTimeString10TzOffsetResult },
    { testDateTimeString11, testDateTimeString11Result, testDateTimeString11TzOffsetResult },
    { testDateTimeString12, testDateTimeString12Result, testDateTimeString12TzOffsetResult },
    { testDateTimeString13, testDateTimeString13Result, testDateTimeString13TzOffsetResult },
    { testDateTimeString14, testDateTimeString14Result, testDateTimeString14TzOffsetResult },
    { testDateTimeString15, testDateTimeString15Result, testDateTimeString15TzOffsetResult },
    { testDateTimeString16, testDateTimeString16Result, testDateTimeString16TzOffsetResult },
    { testDateTimeString17, testDateTimeString17Result, testDateTimeString17TzOffsetResult },
    { testDateTimeString18, testDateTimeString18Result, testDateTimeString18TzOffsetResult },
    { testDateTimeString19, testDateTimeString19Result, testDateTimeString19TzOffsetResult },
    { testDateTimeString20, testDateTimeString20Result, testDateTimeString20TzOffsetResult },
    { testDateTimeString21, testDateTimeString21Result, testDateTimeString21TzOffsetResult },
    { testDateTimeString22, testDateTimeString22Result, testDateTimeString22TzOffsetResult },
    { testDateTimeString23, testDateTimeString23Result, testDateTimeString23TzOffsetResult },
    { testDateTimeString24, testDateTimeString24Result, testDateTimeString24TzOffsetResult },
    { testDateTimeString25, testDateTimeString25Result, testDateTimeString25TzOffsetResult },
    { testDateTimeString26, testDateTimeString26Result, testDateTimeString26TzOffsetResult },
    { testDateTimeString27, testDateTimeString27Result, testDateTimeString27TzOffsetResult },
    { testDateTimeString28, testDateTimeString28Result, testDateTimeString28TzOffsetResult },
    { testDateTimeString29, testDateTimeString29Result, testDateTimeString29TzOffsetResult }

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformDateTimeStringParsingTests( void )
{
    GUCEF_TESTFW_TESTCASE( "Test: DateTime string parsing" )
        try
        {
            for ( UInt32 i=0; i<stringParseTestCases.size(); ++i )
            {
                const StringParseTestCase& testCase = stringParseTestCases[ i ];
                CORE::CDateTime dateTime;

                ASSERT_TRUE( dateTime.FromIso8601DateTimeString( testCase.dateTimeString ) );
                ASSERT_TRUE( dateTime.GetYear() == testCase.dateTimeStringResult[ 0 ] );
                ASSERT_TRUE( dateTime.GetMonth() == testCase.dateTimeStringResult[ 1 ] );
                ASSERT_TRUE( dateTime.GetDay() == testCase.dateTimeStringResult[ 2 ] );
                ASSERT_TRUE( dateTime.GetHours() == testCase.dateTimeStringResult[ 3 ] );
                ASSERT_TRUE( dateTime.GetMinutes() == testCase.dateTimeStringResult[ 4 ] );
                ASSERT_TRUE( dateTime.GetSeconds() == testCase.dateTimeStringResult[ 5 ] );
                ASSERT_TRUE( dateTime.GetMilliseconds() == testCase.dateTimeStringResult[ 6 ] );
                ASSERT_TRUE( dateTime.GetTimeZoneUTCOffsetInMins() == testCase.tzOffsetResult );
            }
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/

void
PerformDateTimeTests( void )
{
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "COMMENCING DateTime TESTS" );
    
    GUCEF_TESTFW_SUITE_SCOPE( "CDateTime" );

    PerformDateTimeStringParsingTests();

    // Regression: on Linux the time difference 'from A towards B' had its sign inverted
    GUCEF_TESTFW_TESTCASE( "Test: Time difference towards a later time is positive" )
        try
        {
            CORE::CDateTime earlier( 2030, 1, 1, 0, 0, 0, 0, 0 );
            CORE::CDateTime later( 2030, 1, 1, 0, 0, 1, 500, 0 );
            ASSERT_TRUE( 1500 == earlier.GetTimeDifferenceInMillisecondsTowards( later ) );
            ASSERT_TRUE( -1500 == later.GetTimeDifferenceInMillisecondsTowards( earlier ) );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: on Linux the day of the month was shifted by one in the conversions to and from the O/S
    GUCEF_TESTFW_TESTCASE( "Test: Conversion to and from unix time keeps the date" )
        try
        {
            // 2021-06-15 12:34:56 UTC
            const UInt64 knownTicks = 1623760496000ULL;
            CORE::CDateTime utc( 2021, 6, 15, 12, 34, 56, 0, 0 );
            ASSERT_TRUE( knownTicks == utc.ToUnixEpochBasedTicksInMillisecs() );

            CORE::CDateTime fromTicks;
            fromTicks.FromUnixEpochBasedTicksInMillisecs( knownTicks );
            ASSERT_TRUE( 2021 == fromTicks.GetYear() && 6 == fromTicks.GetMonth() && 15 == fromTicks.GetDay() );
            ASSERT_TRUE( 12 == fromTicks.GetHours() && 34 == fromTicks.GetMinutes() && 56 == fromTicks.GetSeconds() );

            CORE::CDateTime fromTimeT( (time_t) ( knownTicks / 1000 ), true );
            ASSERT_TRUE( 2021 == fromTimeT.GetYear() && 6 == fromTimeT.GetMonth() && 15 == fromTimeT.GetDay() );
            ASSERT_TRUE( 12 == fromTimeT.GetHours() && 34 == fromTimeT.GetMinutes() );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: on Linux the timezone offset was ignored when converting to UTC
    GUCEF_TESTFW_TESTCASE( "Test: A local time with a timezone offset converts to the right UTC time" )
        try
        {
            // 07:34:56 at UTC-5 is 12:34:56 UTC
            CORE::CDateTime local( 2021, 6, 15, 7, 34, 56, 0, -300 );
            ASSERT_TRUE( 1623760496000ULL == local.ToUnixEpochBasedTicksInMillisecs() );

            CORE::CDateTime utc = local.ToUTC();
            ASSERT_TRUE( 0 == utc.GetTimeZoneUTCOffsetInMins() );
            ASSERT_TRUE( 15 == utc.GetDay() && 12 == utc.GetHours() && 34 == utc.GetMinutes() );

            // the same moment in different timezones compares equal
            ASSERT_TRUE( local == CORE::CDateTime( 2021, 6, 15, 12, 34, 56, 0, 0 ) );

            // 23:30 at UTC-5 is the next day in UTC
            CORE::CDateTime lateEvening( 2021, 6, 15, 23, 30, 0, 0, -300 );
            CORE::CDateTime nextDay = lateEvening.ToUTC();
            ASSERT_TRUE( 16 == nextDay.GetDay() && 4 == nextDay.GetHours() && 30 == nextDay.GetMinutes() );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: on Linux the current date used the day of the week instead of the day of the month
    GUCEF_TESTFW_TESTCASE( "Test: The current local and UTC date-times match the C library" )
        try
        {
            time_t now = ::time( GUCEF_NULL );
            struct tm localNow = *::localtime( &now );
            struct tm utcNow = *::gmtime( &now );

            CORE::CDateTime local = CORE::CDateTime::NowLocalDateTime();
            CORE::CDateTime utc = CORE::CDateTime::NowUTCDateTime();
            CORE::CDate today = CORE::CDate::TodaysLocalDate();
            CORE::CDate todayUtc = CORE::CDate::TodaysUTCDate();

            // allow for the clock passing midnight during the test
            ASSERT_TRUE( localNow.tm_mday == local.GetDay() || ::time( GUCEF_NULL ) != now );
            ASSERT_TRUE( localNow.tm_hour == local.GetHours() || ::time( GUCEF_NULL ) != now );
            ASSERT_TRUE( utcNow.tm_mday == utc.GetDay() || ::time( GUCEF_NULL ) != now );
            ASSERT_TRUE( utcNow.tm_hour == utc.GetHours() || ::time( GUCEF_NULL ) != now );
            ASSERT_TRUE( localNow.tm_mday == today.GetDay() || ::time( GUCEF_NULL ) != now );
            ASSERT_TRUE( utcNow.tm_mday == todayUtc.GetDay() || ::time( GUCEF_NULL ) != now );

            // both describe the same moment
            Int64 differenceInMs = local.GetTimeDifferenceInMillisecondsTowards( utc );
            ASSERT_TRUE( differenceInMs > -2000 && differenceInMs < 2000 );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    CORE::CLogStreamScope::FlushLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ALL DateTime TESTS COMPLETED" );
}

/*-------------------------------------------------------------------------*/

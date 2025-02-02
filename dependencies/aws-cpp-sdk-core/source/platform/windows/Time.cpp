/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/platform/AwsTime.h>

#include <time.h>

namespace Aws
{
namespace Time
{

time_t TimeGM(struct tm* const t)
{
    return _mkgmtime(t);
}

void LocalTime(tm* t, std::time_t time)
{
    localtime_s(t, &time);
}

void GMTime(tm* t, std::time_t time)
{
    gmtime_s(t, &time);
}

} // namespace Time
} // namespace Aws

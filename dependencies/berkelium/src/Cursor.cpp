/*  Berkelium Implementation
 *  Cursor.cpp
 *
 *  Copyright (c) 2010, Ewen Cheslack-Postava
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Sirikata nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "berkelium/Cursor.hpp"

namespace Berkelium {

Cursor::Cursor() {
}

#if BERKELIUM_PLATFORM == PLATFORM_WINDOWS

Cursor::Cursor(const HCURSOR handle)
 : mHandle(handle) {
}

HCURSOR Cursor::GetCursor() const {
  return mHandle;
}

#elif BERKELIUM_PLATFORM == PLATFORM_LINUX

Cursor::Cursor(const GdkCursorEnum& _type, GdkCursorPtr _cursor)
 : mType(_type),
   mCursor(_cursor)
{
}

GdkCursorEnum Cursor::GetCursorType() const {
    return mType;
}

GdkCursorPtr Cursor::GetCustomCursor() const {
    return mCursor;
}

#elif BERKELIUM_PLATFORM == PLATFORM_MAC

Cursor::Cursor(NSCursor* _cursor)
 : mCursor(_cursor)
{
}

NSCursor* Cursor::GetCursor() const {
    return mCursor;
}

#endif


} // namespace Berkelium

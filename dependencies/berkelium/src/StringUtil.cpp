/*  Berkelium - Embedded Chromium
 *  StringUtil.cpp
 *
 *  Copyright (c) 2010, Patrick Reiter Horn
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

#include "berkelium/StringUtil.hpp"
#include "berkelium/WeakString.hpp"

#include "base/utf_string_conversions.h"

#include <cstring>

namespace Berkelium {

namespace {
template <class CharType, class StrType>
inline WeakString<CharType> newWeakString(const StrType &str) {
	CharType *outChar = new CharType[str.length()];
	std::memcpy(outChar, str.data(), sizeof(CharType) * str.length());
	return WeakString<CharType>::point_to(outChar, str.length());
}
}

WideString BERKELIUM_EXPORT UTF8ToWide(const UTF8String &in) {
	std::wstring out;
	::UTF8ToWide(in.data(), in.length(), &out);
	return newWeakString<wchar_t, std::wstring>(out);
}
UTF8String BERKELIUM_EXPORT WideToUTF8(const WideString &in) {
	std::string out;
	::WideToUTF8(in.data(), in.length(), &out);
	return newWeakString<char, std::string>(out);
}

WideString BERKELIUM_EXPORT UTF16ToWide(const UTF16String &in) {
	std::wstring out;
	::UTF16ToWide((::char16*)in.data(), in.length(), &out);
	return newWeakString<wchar_t, std::wstring>(out);
}
UTF16String BERKELIUM_EXPORT wideToUTF16(const WideString &in) {
	string16 out;
	::WideToUTF16(in.data(), in.length(), &out);
	return newWeakString<char16, string16>(out);
}

UTF8String BERKELIUM_EXPORT UTF16ToUTF8(const UTF16String &in) {
	std::string out;
	::UTF16ToUTF8((::char16*)in.data(), in.length(), &out);
	return newWeakString<char, std::string>(out);
}
UTF16String BERKELIUM_EXPORT UTF8ToUTF16(const UTF8String &in) {
	string16 out;
	::UTF8ToUTF16(in.data(), in.length(), &out);
	return newWeakString<char16, string16>(out);
}

void BERKELIUM_EXPORT stringUtil_free(WideString returnedValue) {
	delete []returnedValue.data();
}
void BERKELIUM_EXPORT stringUtil_free(UTF8String returnedValue) {
	delete []returnedValue.data();
}
void BERKELIUM_EXPORT stringUtil_free(UTF16String returnedValue) {
	delete []returnedValue.data();
}

}


/*  Replace prefix in null-terminated strings in binary files.
 *  bin_replace.cpp
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

#include <stdio.h>
#include <string>

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: bin_replace findstring replacestring\n");
        return 1;
    }
    std::string tofind(argv[1]);
    std::string toreplace(argv[2]);
    int delta = tofind.length() - toreplace.length();
    if (delta < 0 || tofind.length() < toreplace.length()) {
        fprintf(stderr, "Replace string '%s' longer than find string '%s'!\n",
                toreplace.c_str(), tofind.c_str());
        return 2;
    }
    std::string filecontents;
    while (1) {
        char buffer[2048];
        size_t length = fread(buffer, 1, 2048, stdin);
        if (ferror(stdin)) {
            perror("Failed to read from stdin: ");
            return 3;
        }
        filecontents += std::string(buffer, buffer+length);
        if (feof(stdin)) {
            break;
        }
    }
    std::string::size_type pos = 0;
    while (true) {
        pos = filecontents.find(tofind, pos);
        if (pos == std::string::npos) {
            break;
        }
        //fprintf(stderr, "Found match at position %lu\n", pos);
        std::copy(toreplace.data(), toreplace.data() + toreplace.length(),
                  filecontents.begin() + pos);

        std::string::size_type afterfind = pos + tofind.length();
        std::string::size_type afterfind_end = filecontents.find('\0', afterfind);
        if (afterfind_end != std::string::npos) {
            std::copy(filecontents.data() + afterfind, filecontents.data() + afterfind_end + 1,
                      filecontents.begin() + afterfind - delta);
        }
    }
    int ret = fwrite(filecontents.data(), filecontents.length(), 1, stdout);
    if (ret < 0) {
        perror("Failed to write to stdout: ");
        return 4;
    }
    return 0;
}

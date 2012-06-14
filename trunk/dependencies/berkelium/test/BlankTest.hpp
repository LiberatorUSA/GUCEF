/*  Berkelium Test Suite
 *  Blank.hpp
 *
 *  Copyright (c) 2011, Patrick Reiter Horn
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
#include <cxxtest/TestSuite.h>
#include <berkelium/Berkelium.hpp>
#include <berkelium/Context.hpp>
#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>

using namespace Berkelium;

class BlankTest : public CxxTest::TestSuite {
  Window *mWin;
  Context *mCtx;
public:
  BlankTest() {
  }

  void setUp( void ) {
    mCtx = Context::create();
    mWin = Window::create(mCtx);
  }
  void tearDown( void ) {
    mWin->destroy();
    mCtx->destroy();
  }

  void testListenerCallAddRemove( void ) {
    class Delegate: public WindowDelegate {
      BlankTest *testRunner;
    public:
      Delegate(BlankTest *parent) : testRunner(parent) {}
      void onLoad(Window *win) {
        Berkelium::stopRunning();
        TS_ASSERT_EQUALS(testRunner->mWin, win);
      }
    } delegate(this);

    mWin->setDelegate(&delegate);
    mWin->resize(256, 256);
    mWin->navigateTo(URLString::point_to("about:blank"));

    Berkelium::runUntilStopped();

    mWin->setDelegate(0);
  }

};

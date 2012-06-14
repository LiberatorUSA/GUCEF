/*  Berkelium Implementation
 *  Root.hpp
 *
 *  Copyright (c) 2009, Patrick Reiter Horn
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

#include "base/mac/scoped_nsautorelease_pool.h"
#include "berkelium/Platform.hpp"
#include "berkelium/Berkelium.hpp"
#include "Singleton.hpp"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/notification_service.h"
#include "base/ref_counted.h"
#include "base/message_loop.h"
#include "base/scoped_ptr.h"
#include "content/browser/browser_thread.h"
#include "DevToolsHttpProtocolHandler.hpp"

class BrowserRenderProcessHost;
class ProcessSingleton;
class URLRequestContext;
namespace ui {
    class SystemMonitor;
}
class HighResolutionTimerManager;
class HistogramSynchronizer;
class ScopedTempDir;
class StatisticsRecorder;
namespace chrome_browser_net {
class PredictorInit;
}
namespace Berkelium {

class MemoryRenderViewHostFactory;
class ErrorDelegate;
class WindowImpl;

//singleton class that contains chromium singletons. Not visible outside of Berkelium library core
class Root : public Singleton<Root> {
public:
    typedef std::vector<WindowImpl*> WindowList;
private:
    Profile* mProf;
    scoped_ptr<ui::SystemMonitor> mSysMon;
    scoped_ptr<HighResolutionTimerManager> mTimerMgr;
    scoped_ptr<chrome_browser_net::PredictorInit> mDNSPrefetch;
    URLRequestContextGetter* mDefaultRequestContext;
    scoped_ptr<MessageLoop> mMessageLoop;
    scoped_ptr<NotificationService> mNotificationService;
    scoped_ptr<ProcessSingleton> mProcessSingleton;
    scoped_ptr<BrowserThread> mUIThread;
    scoped_ptr<MemoryRenderViewHostFactory> mRenderViewHostFactory;
    base::mac::ScopedNSAutoreleasePool mAutoreleasePool;
    scoped_refptr<HistogramSynchronizer> mHistogramSynchronizer;

    scoped_ptr<ScopedTempDir> mTempProfileDir; // Used only if non-null

    ErrorDelegate* mErrorHandler;

    WindowList mWindows;
    scoped_refptr<Berkelium::DevToolsHttpProtocolHandler> devtools_http_handler_;
public:
    Root();
    ~Root();

    bool init(FileString homeDirectory, FileString subprocessDirectory, unsigned int extra_argc = 0, const char* extra_argv[] = NULL );

    bool initialized() {
        return mProf ? true : false;
    }

    void runUntilStopped();
    void stopRunning();
    void update();

    void setErrorHandler(ErrorDelegate *errorHandler) {
        mErrorHandler = errorHandler;
    }

    ErrorDelegate * getErrorHandler () const {
        return mErrorHandler;
    }

    ProcessSingleton *getProcessSingleton(){
        return mProcessSingleton.get();
    }
    inline Profile* getProfile() {
        return mProf;
    }

    static void SetUpGLibLogHandler();

    URLRequestContextGetter *getDefaultRequestContext() {
        return mDefaultRequestContext;
    }

    void addWindow(WindowImpl* w);
    void removeWindow(WindowImpl* w);
    WindowList getWindows();

    void setCookie (URLString url, WideString cookieString);
};

}

/*  Berkelium sample application
 *  ppmmain.cpp
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

#include "berkelium/Berkelium.hpp"
#include "berkelium/Window.hpp"
#include "berkelium/WindowDelegate.hpp"
#include "berkelium/Context.hpp"

#include <stdio.h>
#include <sys/types.h>
#ifdef _WIN32
#define sleep(x) Sleep(x*1000)
#include <time.h>
#include <windows.h>
#else
#include <sys/select.h>
#endif
#include <sstream>
#include <iostream>
#include <memory>

using namespace Berkelium;

class TestDelegate : public WindowDelegate {
    std::string mURL;
public:

    virtual void onAddressBarChanged(Window *win, URLString newURL) {
        std::string x = "hi";
        x+= newURL;
        mURL = newURL.get<std::string>();
        std::cout << "*** onAddressChanged to "<<newURL<<std::endl;
    }

    virtual void onStartLoading(Window *win, URLString newURL) {
        std::cout << "*** Start loading "<<newURL<<" from "<<mURL<<std::endl;
    }
    void onLoadingStateChanged(Window *win, bool isLoading) {
        std::cout << "*** Loading state changed "<<mURL<<" to "<<(isLoading?"loading":"stopped")<<std::endl;
    }
    virtual void onLoad(Window *win) {
        sleep(1);
        win->resize(1280,1024);
        win->resize(500,400);
        win->resize(600,500);
        win->resize(1024,768);
/*
        sleep(3);
        std::cout << "*** onLoad "<<mURL<<std::endl;
        if (mURL.find("yahoo") != std::string::npos) {
            return;
        }
        if (mURL.find("google") == std::string::npos) {
            win->navigateTo("http://google.com");
            return;
        }
        if (mURL.find("yahoo") == std::string::npos) {
            win->navigateTo("http://yahoo.com");
        }
*/
    }
    virtual void onLoadError(Window *win, WideString error) {
        std::cout << L"*** onLoadError "<<mURL<<": ";
        std::wcout << error<<std::endl;
    }

    virtual void onResponsive(Window *win) {
        std::cout << "*** onResponsive "<<mURL<<std::endl;
    }

    virtual void onUnresponsive(Window *win) {
        std::cout << "*** onUnresponsive "<<mURL<<std::endl;
    }

    virtual void onPaint(Window *wini, const unsigned char *bitmap_in, const Rect &bitmap_rect,
                         size_t num_copy_rects, const Rect *copy_rects,
                         int dx, int dy, const Rect &scroll_rect) {
        std::cout << "*** onPaint "<<mURL<<std::endl;
        static int call_count = 0;
        FILE *outfile;
        {
            std::ostringstream os;
			os <<
#ifdef _WIN32
				getenv("TEMP") << "\\"
#else
				"/tmp/"
#endif
				<< "chromium_render_" << time(NULL) << "_" << (call_count++) << ".ppm";
            std::string str (os.str());
            outfile = fopen(str.c_str(), "wb");
			if(outfile == NULL) {
				std::cout << "*** cant open file "<<str<<std::endl;
				return;
			}
        }
        const int width = bitmap_rect.width();
        const int height = bitmap_rect.height();

        fprintf(outfile, "P6 %d %d 255\n", width, height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                unsigned char r,g,b,a;
		b = *(bitmap_in++);
		g = *(bitmap_in++);
		r = *(bitmap_in++);
		a = *(bitmap_in++);
                fputc(r, outfile);  // Red
                //fputc(255-a, outfile);  // Alpha
                fputc(g, outfile);  // Green
                fputc(b, outfile);  // Blue
                //(pixel >> 24) & 0xff;  // Alpha
            }
        }
        fclose(outfile);
    }

    virtual void onCrashed(Window *win) {
        std::cout << "*** onCrashed "<<mURL<<std::endl;
    }

    virtual void onCreatedWindow(Window *win, Window *newWindow, const Rect &initialRect) {
        std::cout << "*** onCreatedWindow from source "<<mURL<<std::endl;
        newWindow->setDelegate(new TestDelegate);
    }

    virtual void onExternalHost(
        Window *win,
        WideString message,
        URLString origin,
        URLString target)
    {
        std::cout << "*** onChromeSend at URL "<<mURL<<" from "<<origin<<" to "<<target<<":"<<std::endl;
        std::wcout << message<<std::endl;
    }

    virtual void onPaintPluginTexture(
        Window *win,
        void* sourceGLTexture,
        const std::vector<Rect> srcRects, // relative to destRect
        const Rect &destRect) {}

////////// WIDGET FUNCTIONS //////////
    virtual void onWidgetCreated(Window *win, Widget *newWidget, int zIndex) {}
    virtual void onWidgetDestroyed(Window *win, Widget *newWidget) {}

    // Will be called before the first call to paint.
    virtual void onWidgetResize(
        Window *win,
        Widget *wid,
        int newWidth,
        int newHeight) {}

    // Never called for the main window.
    virtual void onWidgetMove(
        Window *win,
        Widget *wid,
        int newX,
        int newY) {}

    virtual void onWidgetPaint(
        Window *win,
        Widget *wid,
        const unsigned char *sourceBuffer,
        const Rect &rect,
        size_t num_copy_rects, const Rect *copy_rects,
        int dx, int dy,
        const Rect &scrollRect) {}
};

int main (int argc, char **argv) {
    printf("RUNNING MAIN!\n");
    if (!Berkelium::init(FileString::empty())) {
        std::cout << "Failed to initialize berkelium!" << std::endl;
        return 1;
    }
    std::string url;
/*
    std::auto_ptr<Window> win(Window::create());
    win->resize(800,600);
    win->setDelegate(new TestDelegate);
    url = "http://web.archive.org/web/20080724161936/http://dunnbypaul.net/js_mouse/";
    win->navigateTo(url.data(),url.length());
    win->setTransparent(true); //false);
    //win->navigateTo("http://google.com");
    std::auto_ptr<Window> win2(Window::create());
    url = "http://slashdot.org";
    win2->resize(800,600);
    win2->setDelegate(new TestDelegate);
    win2->navigateTo(url.data(),url.length());
    std::auto_ptr<Window> win3(Window::create());
    url = "http://vegastrike.sourceforge.net";
    win3->resize(800,600);
    win3->setDelegate(new TestDelegate);
    win3->navigateTo(url.data(),url.length());
*/

    Context *context = Context::create();
    std::auto_ptr<Window> win4(Window::create(context));
    delete context;
    win4->resize(800,600);
    win4->setDelegate(new TestDelegate);
    if (argc < 2) {
        url="http://xkcd.com";
    } else {
        url=argv[1];
    }
    win4->navigateTo(URLString::point_to(url));

    while(true) {
        Berkelium::update();
        {
#ifdef _WIN32
            Sleep(10);
#else
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 10000;
			select(0,NULL,NULL,NULL, &tv);
#endif
        }
    }
/*
    char *buffer = new char[WIDTH*HEIGHT*3];
    int retval=Berkelium::renderToBuffer(&buffer[0],WIDTH,HEIGHT);
    delete []buffer;
*/
    return 0;
}

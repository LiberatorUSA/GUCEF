/*  Berkelium Implementation
 *  RenderWidget.cpp
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

#include "content/browser/renderer_host/backing_store_manager.h"
#if defined(OS_LINUX)
#include "webkit/plugins/npapi/webplugin.h"
#include "webkit/plugins/npapi/gtk_plugin_container_manager.h"
#include "webkit/plugins/npapi/gtk_plugin_container.h"
#include <gtk/gtk.h>
#include <gtk/gtkwindow.h>
#endif

#if defined (OS_WIN)
#include <windows.h> // for GetTickCount()
#elif defined (OS_POSIX)
#include <sys/time.h>
#include <time.h>
#endif

#include "base/utf_string_conversions.h"

#include "berkelium/Platform.hpp"
#include "RenderWidget.hpp"
#include "WindowImpl.hpp"

#include <iostream>

namespace Berkelium {

void Widget::destroy(){
    delete this;
}

RenderWidget::RenderWidget(WindowImpl *winImpl, int id) {
    mFocused = true;
    mBacking = NULL;
    mWindow = winImpl;

    mMouseX=mMouseY=0;
    mModifiers=0;

    mId = id;
}

int RenderWidget::getId() const {
    return mId;
}

void RenderWidget::setHost(RenderWidgetHost *host) {
    mHost = host;
}

void RenderWidget::setPos(int x, int y) {
    mRect.set_x(x);
    mRect.set_y(y);
}

Rect RenderWidget::getRect() const {
    Rect ret;
    ret.mLeft = mRect.x();
    ret.mTop = mRect.y();
    ret.mWidth = mRect.width();
    ret.mHeight = mRect.height();
    return ret;
}

RenderWidget::~RenderWidget() {
    if (mBacking) {
        BackingStoreManager::RemoveBackingStore(mHost);
    }
    mWindow->onWidgetDestroyed(this);
}

void RenderWidget::InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos){
    SetSize(pos.size());
}

  // Returns the associated RenderWidgetHost.
RenderWidgetHost* RenderWidget::GetRenderWidgetHost() const{
    return mHost;
}

  // Notifies the View that it has become visible.
void RenderWidget::DidBecomeSelected(){
}

  // Notifies the View that it has been hidden.
void RenderWidget::WasHidden(){
}

  // Tells the View to size itself to the specified size.
void RenderWidget::SetSize(const gfx::Size& size){
    mRect.set_width(size.width());
    mRect.set_height(size.height());
}

  // Retrieves the native view used to contain plugins and identify the
  // renderer in IPC messages.
gfx::NativeView RenderWidget::GetNativeView(){
    return gfx::NativeView();
}

  // Actually set/take focus to/from the associated View component.
void RenderWidget::Focus(){
    mFocused = true;
}
void RenderWidget::Blur(){
    mFocused = false;
}

  // Returns true if the view is showing
bool RenderWidget::IsShowing(){
    return true;
}
  // Returns true if the View currently has the focus.
bool RenderWidget::HasFocus(){
    return mFocused;
}

  // Shows/hides the view.  These must always be called together in pairs.
  // It is not legal to call Hide() multiple times in a row.
void RenderWidget::Show(){
}
void RenderWidget::Hide(){
}

  // Retrieve the bounds of the View, in screen coordinates.
gfx::Rect RenderWidget::GetViewBounds() const{
    return mRect;
}

  // Sets the cursor to the one associated with the specified cursor_type
void RenderWidget::UpdateCursor(const WebCursor& cursor){
    mWindow->UpdateCursor(cursor);
}

  // Indicates whether the page has finished loading.
void RenderWidget::SetIsLoading(bool is_loading){
}

  // Enable or disable IME for the view.
void RenderWidget::ImeUpdateTextInputState(WebKit::WebTextInputType type, const gfx::Rect& caret_rect){
}

void RenderWidget::ImeCancelComposition() {
}

void RenderWidget::DidUpdateBackingStore(
    const gfx::Rect& scroll_rect, int scroll_dx, int scroll_dy,
    const std::vector<gfx::Rect>& copy_rects) {
}

  // Notifies the View that the renderer has ceased to exist.
void RenderWidget::RenderViewGone(base::TerminationStatus termination, int error_code){
#if defined(OS_WIN)
  CleanupCompositorWindow();
#endif
}

// Notifies the View that the renderer will be delete soon.
void RenderWidget::WillDestroyRenderWidget(RenderWidgetHost* rwh) {
}

  // Tells the View to destroy itself.
void RenderWidget::Destroy(){
    delete this;
}

  // Tells the View that the tooltip text for the current mouse position over
  // the page has changed.
void RenderWidget::SetTooltipText(const std::wstring& tooltip_text){
    // Chromium absolutely spams us with this event even when the text hasn't changed
    if (mTooltip.compare(tooltip_text) != 0) {
      mTooltip = tooltip_text;
      if (mWindow)
        mWindow->TooltipChanged(tooltip_text);
    }
}

  // Notifies the View that the renderer text selection has changed.
void RenderWidget::SelectionChanged(const std::string& text) {
}

  // Tells the View whether the context menu is showing. This is used on Linux
  // to suppress updates to webkit focus for the duration of the show.
void RenderWidget::ShowingContextMenu(bool showing) {
}

  // Allocate a backing store for this view
BackingStore* RenderWidget::AllocBackingStore(const gfx::Size& size) {
    SetSize(size);

    if (!mBacking) {
        mBacking = BackingStoreManager::GetBackingStore(mHost, size); // will ignore paints on linux!
    }
    return mBacking;
}

void RenderWidget::InitAsFullscreen() {
}

#if defined(OS_WIN)

static BOOL CALLBACK AddChildWindowToVector(HWND hwnd, LPARAM lparam) {
  std::vector<HWND>* vector = reinterpret_cast<std::vector<HWND>*>(lparam);
  vector->push_back(hwnd);
  return TRUE;
}

void RenderWidget::CleanupCompositorWindow() {
  if (!compositor_host_window_)
    return;

  std::vector<HWND> all_child_windows;
  ::EnumChildWindows(compositor_host_window_, AddChildWindowToVector,
    reinterpret_cast<LPARAM>(&all_child_windows));
  if (all_child_windows.size()) {
    DCHECK(all_child_windows.size() == 1);
    ::ShowWindow(all_child_windows[0], SW_HIDE);
    ::SetParent(all_child_windows[0], NULL);
  }
  compositor_host_window_ = NULL;
}

void RenderWidget::WillWmDestroy() {
  CleanupCompositorWindow();
}

// Looks through the children windows of the CompositorHostWindow. If the
// compositor child window is found, its size is checked against the host
// window's size. If the child is smaller in either dimensions, we fill
// the host window with white to avoid unseemly cracks.
static void PaintCompositorHostWindow(HWND hWnd) {
  PAINTSTRUCT paint;
  BeginPaint(hWnd, &paint);

  std::vector<HWND> child_windows;
  EnumChildWindows(hWnd, AddChildWindowToVector,
      reinterpret_cast<LPARAM>(&child_windows));

  if (child_windows.size()) {
    HWND child = child_windows[0];

    RECT host_rect, child_rect;
    GetClientRect(hWnd, &host_rect);
    if (GetClientRect(child, &child_rect)) {
      if (child_rect.right < host_rect.right ||
         child_rect.bottom != host_rect.bottom) {
          FillRect(paint.hdc, &host_rect,
              static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
      }
    }
  }
  EndPaint(hWnd, &paint);
}

// WndProc for the compositor host window. We use this instead of Default so
// we can drop WM_PAINT and WM_ERASEBKGD messages on the floor.
static LRESULT CALLBACK CompositorHostWindowProc(HWND hWnd, UINT message,
                                                 WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    return 0;
  case WM_PAINT:
    PaintCompositorHostWindow(hWnd);
    return 0;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}

void RenderWidget::ShowCompositorHostWindow(bool show) {
  // When we first create the compositor, we will get a show request from
  // the renderer before we have gotten the create request from the GPU. In this
  // case, simply ignore the show request.
  if (compositor_host_window_ == NULL)
    return;

  if (show) {
    UINT flags = SWP_NOSENDCHANGING | SWP_NOCOPYBITS | SWP_NOZORDER |
      SWP_NOACTIVATE | SWP_DEFERERASE | SWP_SHOWWINDOW;
    gfx::Rect rect = GetViewBounds();
    ::SetWindowPos(compositor_host_window_, NULL, 0, 0,
        rect.width(), rect.height(),
        flags);

    // Get all the child windows of this view, including the compositor window.
	/*
    std::vector<HWND> all_child_windows;
    ::EnumChildWindows(m_hWnd, AddChildWindowToVector,
        reinterpret_cast<LPARAM>(&all_child_windows));

    // Build a list of just the plugin window handles
    std::vector<HWND> plugin_windows;
    bool compositor_host_window_found = false;
    for (size_t i = 0; i < all_child_windows.size(); ++i) {
      if (all_child_windows[i] != compositor_host_window_)
        plugin_windows.push_back(all_child_windows[i]);
      else
        compositor_host_window_found = true;
    }
    DCHECK(compositor_host_window_found);

    // Set all the plugin windows to be "after" the compositor window.
    // When the compositor window is created, gets placed above plugins.
    for (size_t i = 0; i < plugin_windows.size(); ++i) {
      HWND next;
      if (i + 1 < plugin_windows.size())
        next = plugin_windows[i+1];
      else
        next = compositor_host_window_;
      ::SetWindowPos(plugin_windows[i], next, 0, 0, 0, 0,
          SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
	*/
  } else {
    ::ShowWindow(compositor_host_window_, SW_HIDE);
  }
}

gfx::PluginWindowHandle RenderWidget::AcquireCompositingSurface() {
  // If the window has been created, don't recreate it a second time
  if (compositor_host_window_)
    return compositor_host_window_;

  static ATOM window_class = 0;
  if (!window_class) {
    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = 0;
    wcex.lpfnWndProc    = CompositorHostWindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = GetModuleHandle(NULL);
    wcex.hIcon          = 0;
    wcex.hCursor        = 0;
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = L"CompositorHostWindowClass";
    wcex.hIconSm        = 0;
    window_class = RegisterClassEx(&wcex);
    DCHECK(window_class);
  }

  int width = mRect.width();
  int height = mRect.height();

  compositor_host_window_ = CreateWindowEx(
    WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
    MAKEINTATOM(window_class), 0,
    WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_DISABLED,
    0, 0, width, height, 0, 0, GetModuleHandle(NULL), 0);
  DCHECK(compositor_host_window_);

  return static_cast<gfx::PluginWindowHandle>(compositor_host_window_);
}

void RenderWidget::ReleaseCompositingSurface(gfx::PluginWindowHandle) {
  ShowCompositorHostWindow(false);
}

#else

gfx::PluginWindowHandle RenderWidget::AcquireCompositingSurface() {
    return gfx::PluginWindowHandle();
}

void RenderWidget::ReleaseCompositingSurface(gfx::PluginWindowHandle) {
}

#endif

#if defined(OS_MACOSX)
  // Display a native control popup menu for WebKit.
void RenderWidget::ShowPopupWithItems(gfx::Rect bounds,
                                  int item_height,
                                  double item_font,
                                  int selected_item,
                                  const std::vector<WebMenuItem>& items,
                                  bool x){
}

  // Get the view's position on the screen.
gfx::Rect RenderWidget::GetWindowRect(){
    return gfx::Rect(0, 0, mRect.width(), mRect.height());
}

  // Get the view's window's position on the screen.
gfx::Rect RenderWidget::GetRootWindowRect(){
    return mRect;
}

  // Set the view's active state (i.e., tint state of controls).
void RenderWidget::SetActive(bool active){
}

void RenderWidget::SetWindowVisibility(bool) {
}

void RenderWidget::WindowFrameChanged() {
}

  // Methods associated with GPU-accelerated plug-in instances.
gfx::PluginWindowHandle RenderWidget::AllocateFakePluginWindowHandle(bool opaque, bool root) {
  return plugin_container_manager_.AllocateFakePluginWindowHandle(opaque, root);
}
void RenderWidget::DestroyFakePluginWindowHandle(gfx::PluginWindowHandle window) {
  plugin_container_manager_.DestroyFakePluginWindowHandle(window);
}
void RenderWidget::AcceleratedSurfaceSetIOSurface(
    gfx::PluginWindowHandle window,
    int32 width, int32 height,
    uint64_t io_surface_id) {
  plugin_container_manager_.SetSizeAndIOSurface(window,
                                                width,
                                                height,
                                                io_surface_id);
}
void RenderWidget::AcceleratedSurfaceSetTransportDIB(
    gfx::PluginWindowHandle window,
    int32 width, int32 height, base::SharedMemoryHandle transport_dib) {
  plugin_container_manager_.SetSizeAndTransportDIB(window,
                                                   width,
                                                   height,
                                                   transport_dib);
}
void RenderWidget::AcceleratedSurfaceBuffersSwapped(
      gfx::PluginWindowHandle window,
      uint64 surface_id,
      int renderer_id,
      int32 route_id,
      int gpu_host_id,
      uint64 swap_buffers_count) {
  // FIXME: What do we do here.
  //[cocoa_view_ drawAcceleratedPluginLayer];
}

void RenderWidget::SetTakesFocusOnlyOnMouseDown(bool) {
}
void RenderWidget::SetPluginImeEnabled(bool, int) {
}
void RenderWidget::StartPluginIme() {
}
bool RenderWidget::PostProcessEventForPluginIme(const NativeWebKeyboardEvent&) {
  return false;
}
void RenderWidget::GpuRenderingStateDidChange() {
}
void RenderWidget::PluginFocusChanged(bool, int) {
}
gfx::Rect RenderWidget::GetViewCocoaBounds() const {
  return mRect;
}

#endif

#if defined(OS_LINUX)

static std::map<gfx::PluginWindowHandle, GtkWidget*> activeWidgets;

void RealizeCallback(GtkWidget* widget, void* user_data) {
    gfx::PluginWindowHandle id = (gfx::PluginWindowHandle)user_data;
    if (id)
        gtk_socket_add_id(GTK_SOCKET(widget), id);
}


void RenderWidget::CreatePluginContainer(gfx::PluginWindowHandle id){
    std::cerr<<"CREATED PLUGIN CONTAINER: "<<id<<std::endl;
    assert(activeWidgets.find(id) == activeWidgets.end());

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    activeWidgets.insert(
        std::pair<gfx::PluginWindowHandle, GtkWidget*>(id, window));

    const GURL &current = mWindow->getCurrentURL();
    std::string pluginTitle = "[Plugin from "+current.spec()+"]";
    gtk_window_set_title (GTK_WINDOW(window), pluginTitle.c_str());

    GtkWidget *widget = webkit::npapi::gtk_plugin_container_new();

    // The Realize callback is responsible for adding the plug into the socket.
    // The reason is 2-fold:
    // - the plug can't be added until the socket is realized, but this may not
    // happen until the socket is attached to a top-level window, which isn't the
    // case for background tabs.
    // - when dragging tabs, the socket gets unrealized, which breaks the XEMBED
    // connection. We need to make it again when the tab is reattached, and the
    // socket gets realized again.
    //
    // Note, the RealizeCallback relies on the plugin_window_to_widget_map_ to
    // have the mapping.
    g_signal_connect(G_OBJECT(widget), "realize",
                     G_CALLBACK(RealizeCallback), (void*)id);

    // Don't destroy the widget when the plug is removed.
    g_signal_connect(G_OBJECT(widget), "plug-removed",
                     G_CALLBACK(gtk_true), NULL);

    gtk_container_add(GTK_CONTAINER(window), widget);
    gtk_widget_show(widget);

}
void RenderWidget::DestroyPluginContainer(gfx::PluginWindowHandle id){
    std::cerr<<"DESTROYED PLUGIN CONTAINER: "<<id<<std::endl;
    std::map<gfx::PluginWindowHandle, GtkWidget*>::iterator iter;
    iter = activeWidgets.find(id);
    if (iter != activeWidgets.end()) {
        GtkWidget * w = iter->second;
        gtk_widget_destroy(w);
        activeWidgets.erase(iter);
    }
}

void RenderWidget::AcceleratedCompositingActivated(bool activated) {
    // FIXME: Support for accelerated compositing.
}
#endif

  // Moves all plugin windows as described in the given list.
void RenderWidget::MovePluginWindows(
    const std::vector<webkit::npapi::WebPluginGeometry>& moves)
{
#ifdef OS_LINUX
    for (size_t i = 0; i < moves.size(); i++) {
        std::map<gfx::PluginWindowHandle, GtkWidget*>::iterator iter;
        iter = activeWidgets.find(moves[i].window);
        if (iter != activeWidgets.end()) {
            gtk_window_resize(
                GTK_WINDOW (iter->second),
                moves[i].window_rect.width(),
                moves[i].window_rect.height());
            if (moves[i].visible) {
                gtk_widget_show (iter->second);
            } else {
                gtk_widget_hide (iter->second);
            }
        }
    }
#endif
}

void RenderWidget::SetVisuallyDeemphasized(const SkColor *color, bool deemphasized) {
}

// Returns true if the native view, |native_view|, is contained within in the
// widget associated with this RenderWidgetHostView.
bool RenderWidget::ContainsNativeView(gfx::NativeView native_view) const {
    return false;
}

void RenderWidget::focus() {
    if (mHost) {
        mHost->Focus();
	}
}
void RenderWidget::unfocus() {
    if (mHost) {
        mHost->Blur();
	}
}

bool RenderWidget::hasFocus() const {
    return mFocused;
}

template<class T>
void zeroWebEvent(T &event, int modifiers, WebKit::WebInputEvent::Type t) {
    memset(&event,0,sizeof(T));
    event.type=t;
    event.size=sizeof(T);
    event.modifiers=modifiers;
#ifdef _WIN32
    event.timeStampSeconds=GetTickCount()/1000.0;
#else
    timeval tv;
    gettimeofday(&tv,NULL);
    event.timeStampSeconds=((double)tv.tv_usec)/1000000.0;
    event.timeStampSeconds+=tv.tv_sec;
#endif
}

void RenderWidget::mouseMoved(int xPos, int yPos) {
    WebKit::WebMouseEvent event;
    zeroWebEvent(event, mModifiers, WebKit::WebInputEvent::MouseMove);
	event.x = xPos;
	event.y = yPos;
	event.globalX = xPos+mRect.x();
	event.globalY = yPos+mRect.y();
    mMouseX=xPos;
    mMouseY=yPos;

	event.button = (WebKit::WebMouseEvent::Button)mButton;
    event.clickCount = (mButton != (int32)WebKit::WebMouseEvent::ButtonNone);

	if (GetRenderWidgetHost()) {
		GetRenderWidgetHost()->ForwardMouseEvent(event);
	}
}

void RenderWidget::mouseWheel(int scrollX, int scrollY) {
	WebKit::WebMouseWheelEvent event;
	zeroWebEvent(event, mModifiers, WebKit::WebInputEvent::MouseWheel);
	event.x = mMouseX;
	event.y = mMouseY;
	event.windowX = mMouseX; // PRHFIXME: Window vs Global position?
	event.windowY = mMouseY;
	event.globalX = mMouseX+mRect.x();
	event.globalY = mMouseY+mRect.y();
	event.deltaX = scrollX; // PRHFIXME: want x and y scroll.
	event.deltaY = scrollY;
	event.wheelTicksX = scrollX; // PRHFIXME: want x and y scroll.
	event.wheelTicksY = scrollY;
	event.scrollByPage = false;

	event.button = (WebKit::WebMouseEvent::Button)mButton;
    event.clickCount = (mButton != (int32)WebKit::WebMouseEvent::ButtonNone);

	if (GetRenderWidgetHost()) {
		GetRenderWidgetHost()->ForwardWheelEvent(event);
	}
}

void RenderWidget::mouseButton(unsigned int mouseButton, bool down, int clickCount) {
    unsigned int buttonChangeMask=0;
    switch(mouseButton) {
      case 0:
        buttonChangeMask = WebKit::WebInputEvent::LeftButtonDown;
        break;
      case 1:
        buttonChangeMask = WebKit::WebInputEvent::MiddleButtonDown;
        break;
      case 2:
        buttonChangeMask = WebKit::WebInputEvent::RightButtonDown;
        break;
    }
    if (down) {
        mModifiers|=buttonChangeMask;
    }else {
        mModifiers&=(~buttonChangeMask);
    }
    WebKit::WebMouseEvent event;
    zeroWebEvent(event, mModifiers, down?WebKit::WebInputEvent::MouseDown:WebKit::WebInputEvent::MouseUp);
    switch(mouseButton) {
      case 0:
        event.button = WebKit::WebMouseEvent::ButtonLeft;
        break;
      case 1:
        event.button = WebKit::WebMouseEvent::ButtonMiddle;
        break;
      case 2:
        event.button = WebKit::WebMouseEvent::ButtonRight;
        break;
    }
    if (down){
        event.clickCount=clickCount;
        this->mButton = event.button;
    } else {
        this->mButton = WebKit::WebMouseEvent::ButtonNone;
    }
	event.x = mMouseX;
	event.y = mMouseY;
	event.windowX = mMouseX;
	event.windowY = mMouseY;
	event.globalX = mMouseX+mRect.x();
	event.globalY = mMouseY+mRect.y();
	if (GetRenderWidgetHost()) {
		GetRenderWidgetHost()->ForwardMouseEvent(event);
	}
}

void RenderWidget::keyEvent(bool pressed, int modifiers, int vk_code, int scancode){
	NativeWebKeyboardEvent event;
	zeroWebEvent(event, mModifiers, pressed?WebKit::WebInputEvent::RawKeyDown:WebKit::WebInputEvent::KeyUp);
	event.windowsKeyCode = vk_code;
	event.nativeKeyCode = scancode;
	event.text[0]=0;
	event.unmodifiedText[0]=0;
	event.isSystemKey = (modifiers & Berkelium::SYSTEM_KEY)?true:false;

	event.modifiers=0;
	if (modifiers & Berkelium::ALT_MOD)
		event.modifiers |= WebKit::WebInputEvent::AltKey;
	if (modifiers & Berkelium::CONTROL_MOD)
		event.modifiers |= WebKit::WebInputEvent::ControlKey;
	if (modifiers & Berkelium::SHIFT_MOD)
		event.modifiers |= WebKit::WebInputEvent::ShiftKey;
	if (modifiers & Berkelium::META_MOD)
		event.modifiers |= WebKit::WebInputEvent::MetaKey;
	if (modifiers & Berkelium::KEYPAD_KEY)
		event.modifiers |= WebKit::WebInputEvent::IsKeyPad;
	if (modifiers & Berkelium::AUTOREPEAT_KEY)
		event.modifiers |= WebKit::WebInputEvent::IsAutoRepeat;

	event.setKeyIdentifierFromWindowsKeyCode();

	if (GetRenderWidgetHost()) {
		GetRenderWidgetHost()->ForwardKeyboardEvent(event);
	}
	// keep track of persistent modifiers.
    unsigned int test=(WebKit::WebInputEvent::LeftButtonDown|WebKit::WebInputEvent::MiddleButtonDown|WebKit::WebInputEvent::RightButtonDown);
	mModifiers = ((mModifiers&test) |  (event.modifiers& (Berkelium::SHIFT_MOD|Berkelium::CONTROL_MOD|Berkelium::ALT_MOD|Berkelium::META_MOD)));
}


void RenderWidget::textEvent(const wchar_t * text, size_t textLength) {
    textEvent(WideString::point_to(text, textLength));
}

void RenderWidget::textEvent(WideString wideText) {
	// generate one of these events for each lengthCap chunks.
	// 1 less because we need to null terminate.
    if (!GetRenderWidgetHost()) {
		return;
	}

    string16 text16;
    if (!WideToUTF16(wideText.data(), wideText.length(), &text16)) {
        // error in conversion.
        return;
    }

    // assert(WebKit::WebKeyboardEvent::textLengthCap > 2);

	NativeWebKeyboardEvent event;
	zeroWebEvent(event,mModifiers,WebKit::WebInputEvent::Char);
	event.isSystemKey = false;
	event.windowsKeyCode = 0;
	event.nativeKeyCode = 0;
	event.keyIdentifier[0]=0;
	size_t i;
	for (i = 0; i < text16.length(); i++) {
        event.text[0] = event.unmodifiedText[0] = text16[i];
        if (event.text[0] >= 0xD800 && event.text[0] < 0xDC00 && i < text16.length() - 1) {
            // Surrogate pairs are the only case with two utf-16 chars.
            event.text[1] = event.unmodifiedText[1] = text16[i + 1];
            event.text[2] = event.unmodifiedText[1] = 0;
            i++;
        } else {
            // Otherwise, only send one at a time.
            event.text[1] = event.unmodifiedText[1] = 0;
        }
        GetRenderWidgetHost()->ForwardKeyboardEvent(event);
	}
}

}

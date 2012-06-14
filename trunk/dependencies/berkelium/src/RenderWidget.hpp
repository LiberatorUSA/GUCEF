/*  Berkelium Implementation
 *  RenderWidget.hpp
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

#ifndef _BERKELIUM_RENDERWIDGET_HPP_
#define _BERKELIUM_RENDERWIDGET_HPP_

#include "berkelium/Widget.hpp"
#include "content/browser/renderer_host/render_widget_host.h"
#include "content/browser/renderer_host/render_widget_host_view.h"
#if defined(OS_MACOSX)
#include "content/browser/renderer_host/accelerated_surface_container_manager_mac.h"#
#endif
#include "ui/gfx/rect.h"
//see chrome/browser/renderer_host/test/test_render_view_host.h for a stub impl.

struct WebMenuItem;

namespace Berkelium {
class WindowImpl;


class RenderWidget : public RenderWidgetHostView, public Widget {

public:
    RenderWidget(WindowImpl *winImpl, int id);
    ~RenderWidget();

    int getId() const;

    void focus();
    void unfocus();
    bool hasFocus() const;

    void mouseMoved(int xPos, int yPos);
    void mouseButton(uint32 buttonID, bool down, int clickCount = 1);
    void mouseWheel(int xScroll, int yScroll);
    void textEvent(const wchar_t * evt, size_t evtLength);
    void keyEvent(bool pressed, int mods, int vk_code, int scancode);

    void setHost(RenderWidgetHost *host);
    void setPos(int x, int y);

    Rect getRect() const;

    void textEvent(WideString text);

public: /******* RenderWidgetHostView *******/

  // Perform all the initialization steps necessary for this object to represent
  // a popup (such as a <select> dropdown), then shows the popup at |pos|.
  virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos);

  // Returns the associated RenderWidgetHost.
  virtual RenderWidgetHost* GetRenderWidgetHost() const;

  // Notifies the View that it has become visible.
  virtual void DidBecomeSelected();

  // Notifies the View that it has been hidden.
  virtual void WasHidden();

  // Tells the View to size itself to the specified size.
  virtual void SetSize(const gfx::Size& size);

  // Retrieves the native view used to contain plugins and identify the
  // renderer in IPC messages.
  virtual gfx::NativeView GetNativeView();

  // Moves all plugin windows as described in the given list.
  virtual void MovePluginWindows(
      const std::vector<webkit::npapi::WebPluginGeometry>& moves);

  // Actually set/take focus to/from the associated View component.
  virtual void Focus();
  virtual void Blur();

  virtual bool IsShowing();

  // Returns true if the View currently has the focus.
  virtual bool HasFocus();

  // Shows/hides the view.  These must always be called together in pairs.
  // It is not legal to call Hide() multiple times in a row.
  virtual void Show();
  virtual void Hide();

  // Retrieve the bounds of the View, in screen coordinates.
  virtual gfx::Rect GetViewBounds() const;

  // Sets the cursor to the one associated with the specified cursor_type
  virtual void UpdateCursor(const WebCursor& cursor);

  // Indicates whether the page has finished loading.
  virtual void SetIsLoading(bool is_loading);
  // Updates the state of the input method attached to the view.
  virtual void ImeUpdateTextInputState(WebKit::WebTextInputType type,
                                       const gfx::Rect& caret_rect);

  // Cancel the ongoing composition of the input method attached to the view.
  virtual void ImeCancelComposition();

  // Informs the view that a portion of the widget's backing store was painted.
  // The view should ensure this gets copied to the screen.
  //
  // If the scroll_rect is non-empty, then a portion of the widget's backing
  // store was scrolled by dx pixels horizontally and dy pixels vertically.
  // The exposed rect from the scroll operation is included in copy_rects.
  //
  // There are subtle performance implications here.  The RenderWidget gets sent
  // a paint ack after this returns, so if the view only ever invalidates in
  // response to this, then on Windows, where WM_PAINT has lower priority than
  // events which can cause renderer resizes/paint rect updates, e.g.
  // drag-resizing can starve painting; this function thus provides the view its
  // main chance to ensure it stays painted and not just invalidated.  On the
  // other hand, if this always blindly paints, then if we're already in the
  // midst of a paint on the callstack, we can double-paint unnecessarily.
  // (Worse, we might recursively call RenderWidgetHost::GetBackingStore().)
  // Thus implementers should generally paint as much of |rect| as possible
  // synchronously with as little overpainting as possible.
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect, int scroll_dx, int scroll_dy,
      const std::vector<gfx::Rect>& copy_rects);

  // Notifies the View that the renderer has ceased to exist.
    virtual void RenderViewGone(base::TerminationStatus termination, int error_code);

  // Notifies the View that the renderer will be delete soon.
  virtual void WillDestroyRenderWidget(RenderWidgetHost* rwh);

  // Tells the View to destroy itself.
  virtual void Destroy();

  // Tells the View that the tooltip text for the current mouse position over
  // the page has changed.
  virtual void SetTooltipText(const std::wstring& tooltip_text);

  // Notifies the View that the renderer text selection has changed.
  virtual void SelectionChanged(const std::string& text);

  // Tells the View whether the context menu is showing. This is used on Linux
  // to suppress updates to webkit focus for the duration of the show.
  virtual void ShowingContextMenu(bool showing);

  // Allocate a backing store for this view
  virtual BackingStore* AllocBackingStore(const gfx::Size& size);

  virtual void InitAsFullscreen();

#if defined(OS_WIN)
  void CleanupCompositorWindow();
  void WillWmDestroy();
  void ShowCompositorHostWindow(bool);
#endif

  gfx::PluginWindowHandle AcquireCompositingSurface();
  void ReleaseCompositingSurface(gfx::PluginWindowHandle);

#if defined(OS_MACOSX)

  // Display a native control popup menu for WebKit.
  virtual void ShowPopupWithItems(gfx::Rect bounds,
                                  int item_height,
                                  double item_font_size,
                                  int selected_item,
                                  const std::vector<WebMenuItem>& items,
                                  bool right_aligned);

  // Get the view's position on the screen.
  virtual gfx::Rect GetWindowRect();

  // Get the view's window's position on the screen.
  virtual gfx::Rect GetRootWindowRect();

  // Set the view's active state (i.e., tint state of controls).
  virtual void SetActive(bool active);

  // App hidden or minimized/unminimized
  virtual void SetWindowVisibility(bool);

  virtual void WindowFrameChanged();

  // Methods associated with GPU-accelerated plug-in instances.
  virtual gfx::PluginWindowHandle AllocateFakePluginWindowHandle(bool, bool);
  virtual void DestroyFakePluginWindowHandle(gfx::PluginWindowHandle window);
  virtual void AcceleratedSurfaceSetIOSurface(gfx::PluginWindowHandle window,
                                              int32 width, int32 height, uint64_t io_surface_id);
  virtual void AcceleratedSurfaceSetTransportDIB(gfx::PluginWindowHandle window,
                                              int32 width, int32 height, base::SharedMemoryHandle transport_dib);
  virtual void AcceleratedSurfaceBuffersSwapped(gfx::PluginWindowHandle window,
      uint64 surface_id,
      int renderer_id,
      int32 route_id,
      int gpu_host_id,
      uint64 swap_buffers_count);
  virtual void SetTakesFocusOnlyOnMouseDown(bool);
  virtual void SetPluginImeEnabled(bool, int);
  virtual void StartPluginIme();
  virtual bool PostProcessEventForPluginIme(const NativeWebKeyboardEvent&);
  virtual void GpuRenderingStateDidChange();
  virtual void PluginFocusChanged(bool, int);
  virtual gfx::Rect GetViewCocoaBounds() const;

 #endif

#if defined(OS_LINUX)
  virtual void CreatePluginContainer(gfx::PluginWindowHandle id);
  virtual void DestroyPluginContainer(gfx::PluginWindowHandle id);
  virtual void AcceleratedCompositingActivated(bool activated);
#endif

  // Toggles visual muting of the render view area. This is on when a
  // constrained window is showing.
  virtual void SetVisuallyDeemphasized(const SkColor *color, bool deemphasized);

  // Returns true if the native view, |native_view|, is contained within in the
  // widget associated with this RenderWidgetHostView.
  virtual bool ContainsNativeView(gfx::NativeView native_view) const;

private:
    uint32 mModifiers;
    int32 mButton;
    int32 mMouseX;
    int32 mMouseY;

    RenderWidgetHost *mHost;
    bool mFocused;
    BackingStore* mBacking;
    int mId;
    std::wstring mTooltip;

    WindowImpl* mWindow;

    gfx::Rect mRect;

#if defined(OS_WIN)
    // When we are doing accelerated compositing
    HWND compositor_host_window_;
#endif

#if defined(OS_MACOSX)
  // Helper class for managing instances of accelerated plug-ins.
  AcceleratedSurfaceContainerManagerMac plugin_container_manager_;
#endif
};

}

#endif

/*  Berkelium Implementation
 *  WindowImpl.hpp
 *
 *  Copyright (c) 2009, Daniel Reiter Horn
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

#ifndef _BERKELIUM_WINDOWIMPL_HPP_
#define _BERKELIUM_WINDOWIMPL_HPP_
#include "berkelium/Widget.hpp"
#include "berkelium/Window.hpp"
#include "NavigationController.hpp"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "content/browser/renderer_host/render_widget_host.h"
#include "content/browser/renderer_host/render_view_host.h"
#include "content/browser/renderer_host/render_view_host_delegate.h"
#include "chrome/browser/history/history.h"
#include "chrome/common/render_messages.h"
class RenderProcessHost;
class Profile;
class SelectFileDialog;

namespace Berkelium {
class WindowView;
class RenderWidget;
class MemoryRenderViewHost;
struct Rect;
class NavigationController;
class ContextImpl;

class WindowImpl :
        public Window,
        public RenderViewHostDelegate,
        public RenderViewHostDelegate::View
{

    void init(SiteInstance *, int routingId);
    NavigationEntry* CreateNavigationEntry(
        const GURL&url,
        const GURL&referrer,
        PageTransition::Type transition);

    WindowImpl(const Context*otherContext, int routingId);

	// Updates history with the specified navigation. This is called by
	// OnMsgNavigate to update history state.
	void UpdateHistoryForNavigation(
	  scoped_refptr<history::HistoryAddPageArgs> add_page_args);
    bool UpdateTitleForEntry(NavigationEntry* ent, const std::wstring&title);

	// Returns the history::HistoryAddPageArgs to use for adding a page to
	// history.
	scoped_refptr<history::HistoryAddPageArgs> CreateHistoryAddPageArgs(
		const GURL& virtual_url,
		const NavigationController::LoadCommittedDetails& details,
		const ViewHostMsg_FrameNavigate_Params& params);

public:
//    WindowImpl();
    WindowImpl(const Context*otherContext);
    virtual ~WindowImpl();

    const GURL &getCurrentURL() {
        return mCurrentURL;
    }

    NavigationController* navigationController() {
        return mController;
    }

    SiteInstance *GetSiteInstance();
    bool is_crashed() const { return is_crashed_; }
    void SetIsCrashed(bool state);
    void ShowRepostFormWarningDialog();
    void UpdateMaxPageID(int32 page_id);
    int32 GetMaxPageID();

    bool NavigateToPendingEntry(NavigationController::ReloadType reload);

    // Changes the IsLoading state and notifies delegate as needed
    // |details| is used to provide details on the load that just finished
    // (but can be null if not applicable). Can be overridden.
    void SetIsLoading(bool is_loading);
    void TooltipChanged(const std::wstring& tooltipText);

    void executeJavascript(WideString javascript);
    void insertCSS (WideString css, WideString id);
    bool navigateTo(URLString url);

    Profile *profile() const;
    RenderProcessHost *process() const;
    RenderWidgetHostView *view() const;
    RenderViewHost *host() const;

    virtual Widget* getWidget() const;

    virtual void setTransparent(bool istrans);

    virtual int getId() const;

    virtual void focus();
    virtual void unfocus();
    virtual void mouseMoved(int xPos, int yPos);
    virtual void mouseButton(unsigned int buttonID, bool down, int clickCount);
    virtual void mouseWheel(int xScroll, int yScroll);
    virtual void textEvent(const wchar_t *evt, size_t evtLength);
    virtual void keyEvent(bool pressed, int mods, int vk_code, int scancode);

    virtual void adjustZoom (int mode);

    virtual void refresh();
    virtual void stop();
    virtual void cut();
    virtual void copy();
    virtual void paste();
    virtual void undo();
    virtual void redo();
    virtual void del();
    virtual void selectAll();

    virtual void filesSelected(FileString *files);

    virtual void goBack();
    virtual void goForward();

    virtual bool canGoBack() const;
    virtual bool canGoForward() const;

    void SetContainerBounds(const gfx::Rect &rc);
    void resize(int width, int height);

    void onPaint(Widget *wid,
                 const unsigned char *sourceBuffer,
                 const Rect &sourceBufferRect,
                 size_t numCopyRects, const Rect *copyRects,
                 int dx, int dy, const Rect &scrollRect);
    void onWidgetDestroyed(Widget *wid);

    // Called from MemoryRenderViewHost, since RenderViewHost does nothing here?!
    void OnAddMessageToConsole(
        const std::wstring& message,
        int32 line_no,
        const std::wstring& source_id);

    void UpdateCursor(const WebCursor& cursor);

	// Called by NavigationController.
	void NavigationEntryCommitted(NavigationController::LoadCommittedDetails* details);

    void synchronousScriptReturn(void *handle, const Script::Variant &returnValue);
    void bind(WideString lvalue, const Script::Variant &rvalue);
    void addBindOnStartLoading(WideString, const Script::Variant&);
    void addEvalOnStartLoading(WideString);
    void clearStartLoading();

    void evalInitialJavascript();

    bool javascriptCall(IPC::Message *sync, URLString msg, const std::wstring &args);

protected:
    ContextImpl *getContextImpl() const;

    bool CreateRenderViewForRenderManager(
        RenderViewHost* render_view_host,
        bool remote_view_exists);

protected: /******* RenderViewHostDelegate *******/

    virtual RenderViewHostDelegate::View* GetViewDelegate();
    virtual RendererPreferences GetRendererPrefs(Profile*) const;
    virtual WebPreferences GetWebkitPrefs();

    virtual void RendererUnresponsive(RenderViewHost* render_view_host,
                                      bool is_during_unload);
    virtual void RendererResponsive(RenderViewHost* render_view_host);
    // crashed
    virtual void RenderViewGone(RenderViewHost* render_view_host, base::TerminationStatus status, int error_code);
    // "un-crashed"
    virtual void RenderViewReady(RenderViewHost* render_view_host);

    void ProcessExternalHostMessage(const std::string& message,
                                    const std::string& origin,
                                    const std::string& target);
    virtual bool IsReservedAccelerator(const NativeWebKeyboardEvent&) {return false;}
    virtual void DidDisplayInsecureContent(){}
    virtual void DidRunInsecureContent(const std::string&){}
    virtual void DidStartLoading();
    virtual void DidStopLoading();

    virtual void DidNavigate(RenderViewHost* render_view_host,
                             const ViewHostMsg_FrameNavigate_Params& params);
    virtual void DocumentAvailableInMainFrame(RenderViewHost* render_view_host);
    virtual void UpdateState(RenderViewHost* render_view_host,
                             int32 page_id,
                             const std::string& state);
    virtual void UpdateTitle(RenderViewHost* render_view_host,
                             int32 page_id,
                             const std::wstring& title);
  virtual void Close(RenderViewHost* render_view_host);
  //virtual void RequestMove(const gfx::Rect& new_bounds);
  virtual void RequestOpenURL(const GURL& url, const GURL& referrer,
                              WindowOpenDisposition disposition);
  virtual void DomOperationResponse(const std::string& json_string,
                                    int automation_id);
  virtual void RunJavaScriptMessage(const std::wstring& message,
                                    const std::wstring& default_prompt,
                                    const GURL& frame_url,
                                    const int flags,
                                    IPC::Message* reply_msg,
                                    bool* did_suppress_message);
  virtual void RunFileChooser(const ViewHostMsg_RunFileChooser_Params&params);

  virtual bool OnMessageReceived(const IPC::Message& message);
	virtual const GURL& GetURL() const;

    virtual int GetBrowserWindowID() const;
    ViewType::Type GetRenderViewType()const;

    virtual void UpdateInspectorSetting(const std::string&, const std::string&);
    virtual void ClearInspectorSettings();
    virtual void OnUserGesture();
    virtual void WorkerCrashed();
    virtual void onResizeRequested(gfx::Rect rect);

private: /******* Formerly RenderViewHostDelegate::Resource *******/
    void OnDidStartProvisionalLoadForFrame(
        int64 frame_id,
        bool is_main_frame,
        const GURL& url);

    void OnDidRedirectProvisionalLoad(int32 page_id,
                                            const GURL& source_url,
                                            const GURL& target_url);

    void OnDidRedirectResource(const ResourceRedirectDetails& details);

    void OnDidFailProvisionalLoadWithError(
        int64 frame_id,
        bool is_main_frame,
        int error_code,
        const GURL& url,
        bool showing_repost_interstitial);

    void OnDocumentLoadedInFrame(int64 frame_id);
    void OnDidFinishLoad(int64 frame_id);

protected: /******* RenderViewHostDelegate::View *******/
    virtual void CreateNewWindow(int route_id,
                                 const ViewHostMsg_CreateWindow_Params&params);

    virtual void CreateNewWidget(int route_id, WebKit::WebPopupType popup_type);
    virtual void ShowCreatedWindow(int route_id,
                                   WindowOpenDisposition disposition,
                                   const gfx::Rect& initial_pos,
                                   bool user_gesture);
    virtual void ShowCreatedWidget(int route_id,
                                   const gfx::Rect& initial_pos);
    virtual void ShowContextMenu(const ContextMenuParams& params);
    virtual void StartDragging(const WebDropData& drop_data,
                               WebKit::WebDragOperationsMask allowed_ops,
                               const SkBitmap& image,
                               const gfx::Point& image_offset);
    virtual void UpdateDragCursor(WebKit::WebDragOperation operation);
    virtual void GotFocus();
    virtual void TakeFocus(bool reverse);
    virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);
    virtual void HandleMouseEvent();
    virtual void HandleMouseLeave();
    virtual void CreateNewFullscreenWidget(int route_id);
    virtual void ShowCreatedFullscreenWidget(int route_id);
    virtual void Activate();
    virtual void Deactivate();
    virtual void HandleMouseMove();
    virtual void HandleMouseDown();
    virtual void UpdatePreferredWidth(int pref_width);
    virtual void UpdatePreferredSize(const gfx::Size&);
    virtual bool PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                         bool* is_keyboard_shortcut);
    virtual void ShowPopupMenu(const gfx::Rect&, int, double, int, const std::vector<WebMenuItem>&, bool);
    virtual void LostCapture();
    virtual void HandleMouseUp();
    virtual void HandleMouseActivate();

private: /******* Formerly RenderViewHostDelegate::BrowserIntegration *******/
    void OnGoToEntryAtOffset(int offset);
    void OnGetHistoryListCount(int* back_list_count,
                                     int* forward_list_count);
    void OnMissingPluginStatus(int status);
    void OnCrashedPlugin(const FilePath& plugin_path);
    void OnPageContents(const GURL& url,
                                int32 page_id,
                                const string16& contents,
                                const std::string& language,
                                bool page_translatable);
    void OnPageTranslated(int32 page_id,
                                  const std::string& original_lang,
                                  const std::string& translated_lang,
                                  TranslateErrors::Type error_type);
    void OnDisabledOutdatedPlugin(const string16&, const GURL&);

private:

    GURL mCurrentURL;
    int zIndex;
    int mId;

    int mMouseX;
    int mMouseY;
    bool isLoading;
    gfx::Rect mRect;

    NavigationController *mController;
    scoped_refptr<SelectFileDialog> mSelectFileDialog;

    std::map<int, WindowImpl*> mNewlyCreatedWindows;
    std::map<int, RenderWidget*> mNewlyCreatedWidgets;

    std::set<std::string> mPermittedNames;
	std::wstring mBindingJavascript;
	std::wstring mUniqueId;

    bool received_page_title_;
    bool is_loading_;
    bool is_crashed_;

	bool mIsReentrant;

    // Manages creation and swapping of render views.
    RenderViewHost *mRenderViewHost;

};

}

#endif

/*  Berkelium Implementation
 *  WindowImpl.cpp
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
#include "berkelium/Platform.hpp"
#include "ContextImpl.hpp"
#include "RenderWidget.hpp"
#include "WindowImpl.hpp"
#include "MemoryRenderViewHost.hpp"
#include "Root.hpp"
#include "berkelium/WindowDelegate.hpp"
#include "berkelium/Cursor.hpp"
#include "berkelium/Context.hpp"
#include "berkelium/Rect.hpp"
#include "berkelium/ScriptVariant.hpp"
#include "ScriptUtilImpl.hpp"

#include "base/file_util.h"
#include "base/file_version_info.h"
#include "base/utf_string_conversions.h"
#include "base/values.h"
#include "net/base/net_util.h"
#include "content/browser/in_process_webkit/dom_storage_context.h"
#include "content/browser/in_process_webkit/webkit_context.h"
#include "content/browser/renderer_host/render_view_host.h"
#include "content/browser/site_instance.h"
#include "chrome/browser/renderer_preferences_util.h"
#include "content/browser/tab_contents/tab_contents.h"
#include "content/browser/renderer_host/render_widget_host_view.h"
#include "content/browser/renderer_host/render_view_host_factory.h"
#include "chrome/browser/browser_url_handler.h"
#include "chrome/common/native_web_keyboard_event.h"
#include "content/browser/renderer_host/render_process_host.h"
#include "chrome/common/bindings_policy.h"
#include "webkit/glue/context_menu.h"
#include "chrome/common/render_messages.h"
#include "chrome/common/render_messages_params.h"
#include "ui/base/message_box_flags.h"
#include <iostream>

#if BERKELIUM_PLATFORM == PLATFORM_LINUX
#include <gdk/gdkcursor.h>
#endif

namespace Berkelium {
//WindowImpl temp;

static const char letters[] = "abcdef0123456789";

void WindowImpl::init(SiteInstance*site, int routing_id) {
    mId = routing_id;
    received_page_title_=false;
    is_crashed_=false;
    mIsReentrant = false;
    mUniqueId = std::wstring();
    for (int i = 0; i < 32; i++) {
        if (i == 8 || i == 12 || i == 16 || i == 20) {
            mUniqueId.push_back('-');
        }
        mUniqueId.push_back(letters[rand() % (sizeof(letters)-1)]);
    }

    mRenderViewHost = RenderViewHostFactory::Create(
        site,
        this,
        routing_id,
        mController->session_storage_namespace_);
    host()->AllowBindings(
        BindingsPolicy::EXTERNAL_HOST);

    // For remote debugging
    Root::getSingleton().addWindow(this);
}

WindowImpl::WindowImpl(const Context*otherContext):
        Window(otherContext)
{
    mController = new NavigationController(this, profile(), otherContext->getImpl()->sessionStorageNamespace());
    mMouseX = 0;
    mMouseY = 0;
    isLoading = false;
    mCurrentURL = GURL("about:blank");
    zIndex = 0;
    init(mContext->getImpl()->getSiteInstance(), MSG_ROUTING_NONE);
    CreateRenderViewForRenderManager(host(), false);
}
WindowImpl::WindowImpl(const Context*otherContext, int routing_id):
        Window(otherContext)
{
    mController = new NavigationController(this, profile(), otherContext->getImpl()->sessionStorageNamespace());
    mMouseX = 0;
    mMouseY = 0;
    mCurrentURL = GURL("about:blank");
    zIndex = 0;
    init(mContext->getImpl()->getSiteInstance(), routing_id);
    CreateRenderViewForRenderManager(host(), true);
}
WindowImpl::~WindowImpl() {
    // For remote debugging
    Root::getSingleton().removeWindow(this);

    RenderViewHost* render_view_host = mRenderViewHost;
    mRenderViewHost = NULL;
    render_view_host->Shutdown();
    delete mController;
}

RenderProcessHost *WindowImpl::process() const {
    return host()->process();
}
RenderWidgetHostView *WindowImpl::view() const {
    if (!host()) {
        return NULL;
    }
    return host()->view();
}
RenderViewHost *WindowImpl::host() const {
    return mRenderViewHost;
}

SiteInstance *WindowImpl::GetSiteInstance() {
    return mContext->getImpl()->getSiteInstance();
}

void WindowImpl::SetIsCrashed(bool state) {
  if (state == is_crashed_)
    return;

  is_crashed_ = state;
}

void WindowImpl::SetIsLoading(bool is_loading) {
    isLoading = is_loading;
    host()->SetIsLoading(is_loading);
}
int WindowImpl::GetBrowserWindowID() const {
    return mController->window_id().id();
}

ViewType::Type WindowImpl::GetRenderViewType()const {
    return ViewType::TAB_CONTENTS;
}

void WindowImpl::UpdateInspectorSetting(const std::string&, const std::string&) {
}

void WindowImpl::ClearInspectorSettings() {
}

Widget *WindowImpl::getWidget() const {
    return static_cast<RenderWidget*>(view());
}


void WindowImpl::bind(WideString lvalue, const Script::Variant &rvalue) {
    std::string jsonStr;
    if (host() && Berkelium::Script::toJSON(rvalue, &jsonStr)) {
        host()->ExecuteJavascriptInWebFrame(
            string16(),
            WideToUTF16(lvalue.get<std::wstring>() + L" = " + UTF8ToWide(jsonStr) + L";\n"));
    }
}

void WindowImpl::addBindOnStartLoading(WideString lvalue, const Script::Variant &rvalue) {
    std::string jsonStr;
    if (Berkelium::Script::toJSON(rvalue, &jsonStr)) {
        mBindingJavascript += lvalue;
        mBindingJavascript += L" = ";
        mBindingJavascript += UTF8ToWide(jsonStr);
        mBindingJavascript += L";\n";
    }
}

void WindowImpl::addEvalOnStartLoading(WideString script) {
    mBindingJavascript += script;
    mBindingJavascript += L"\n";
}

void WindowImpl::clearStartLoading() {
    mBindingJavascript = L"";
}

void WindowImpl::evalInitialJavascript() {
    const char *berkeliumFunc =
        "if(!window.Berkelium){(function(bkID){"
        "  var bkCallbacks = {};"
        "  function syncAsyncCall(name, args, issync){"
        "    var msg={'callee': ''+name};"
        "    msg.arguments = [];"
        "    for (var i = 0; i < args.length; i++) {"
        "      msg.arguments[i] = args[i];"
        "    }"
        "    var msgStr = JSON.stringify(msg);"
        "    var id = 'urn:uuid:' + bkID;"
        "    if (!issync) {"
        "      window.externalHost.postMessage(msgStr, id);"
        "    } else {"
        "      return JSON.parse(prompt(msgStr, id));"
        "    }"
        "  }"
        "  window.Berkelium = function(operation, name, args){"
        "    switch (operation) {"
        "    case 'asyncCall':"
        "      return syncAsyncCall(name, args, false);"
        "    case 'syncCall':"
        "      return syncAsyncCall(name, args, true);"
        "    case 'callback':"
        "      bkCallbacks[name].apply(this, args);"
        "      break;"
        "    case 'setCallback':"
        "      bkCallbacks[name] = args;"
        "      break;"
        "    case 'unsetCallback':"
        "      delete bkCallbacks[name];"
        "      break;"
        "    }"
        "  };"
        "})('";
    std::wstring wideScript = UTF8ToWide(berkeliumFunc);
    wideScript += mUniqueId + L"');}\n";
    wideScript += mBindingJavascript;
    host()->ExecuteJavascriptInWebFrame(string16(), WideToUTF16(wideScript));
}

bool WindowImpl::javascriptCall(IPC::Message* reply_msg, URLString url, const std::wstring &args) {
    if (!mDelegate) {
        return false;
    }
    WideString thisFunc = WideString::point_to(L"WindowImpl::javascriptCall");
    scoped_ptr<Value> val (Script::fromJSON(WideToUTF8(args)));
    if (!val.get() || !val->IsType(Value::TYPE_DICTIONARY)) {
        mDelegate->onConsoleMessage(this, WideString::point_to(L"Argument is not a dictionary."),
                                    thisFunc, 1);
        return false;
    }
    DictionaryValue *dict = static_cast<DictionaryValue*>(val.get());
    std::string funcNameUTF8;
    if (!dict->GetString("callee", &funcNameUTF8)) {
        mDelegate->onConsoleMessage(this, WideString::point_to(L"callee is not a string."),
                                    thisFunc, 2);
        return false;
    }
    std::wstring funcName = UTF8ToWide(funcNameUTF8);
    ListValue *listValue = NULL;
    if (!dict->GetList("arguments", &listValue)) {
        mDelegate->onConsoleMessage(this, WideString::point_to(L"arguments is not an array."),
                                    thisFunc, 3);
        return false;
    }
    scoped_array<Script::Variant> list (new Script::Variant[listValue->GetSize()]);
    for (size_t i = 0; i < listValue->GetSize(); i++) {
        Value *element;
        if (!listValue->Get(i, &element)) {
            mDelegate->onConsoleMessage(this, WideString::point_to(L"a list element does not exist."),
                                        thisFunc, 4);
            return false;
        }
        if (!valueToVariant(element, list[i])) {
            mDelegate->onConsoleMessage(this, WideString::point_to(L"Unable to convert value to variant."),
                                        thisFunc, 5);
            return false;
        }
    }
    mDelegate->onJavascriptCallback(
        this, reply_msg, url,
        WideString::point_to(funcName),
        list.get(), listValue->GetSize());
    return true;
}

void WindowImpl::synchronousScriptReturn(void* reply_msg, const Script::Variant &result) {
    std::string jsonStr = "null";
    if (host() && reply_msg) {
        bool success = Berkelium::Script::toJSON(result, &jsonStr);
        host()->JavaScriptMessageBoxClosed(
            static_cast<IPC::Message*>(reply_msg),
            success,
            UTF8ToWide(jsonStr));
    }
}

void WindowImpl::setTransparent(bool istrans) {
    SkBitmap bg;
    int bitmap = 0;
    if (istrans) {
        bg.setConfig(SkBitmap::kA1_Config, 1, 1);
        bg.setPixels(&bitmap);
    }
    if (host()) {
        host()->Send(new ViewMsg_SetBackground(host()->routing_id(),bg));
    }
}

void WindowImpl::focus() {
    FrontToBackIter iter = frontIter();
    if (iter != frontEnd()) {
        (*iter)->focus();
        ++iter;
    }
    while (iter != frontEnd()) {
        (*iter)->unfocus();
        ++iter;
    }
    if (getWidget()) {
        getWidget()->focus();
    }
}
void WindowImpl::unfocus() {
    FrontToBackIter iter = frontIter();
    while (iter != frontEnd()) {
        (*iter)->unfocus();
        ++iter;
    }
}

void WindowImpl::mouseMoved(int xPos, int yPos) {
    int oldX = mMouseX, oldY = mMouseY;
    mMouseX = xPos;
    mMouseY = yPos;
    bool notifiedOld = false, notifiedNew = false;

    for (FrontToBackIter iter = frontIter(); iter != frontEnd(); ++iter) {
        Rect r = (*iter)->getRect();
        if (!notifiedOld && r.contains(oldX, oldY)) {
            notifiedOld = true;
            (*iter)->mouseMoved(xPos - r.left(), yPos - r.top());
        } else if (!notifiedNew && r.contains(xPos, yPos)) {
            notifiedNew = true;
            (*iter)->mouseMoved(xPos - r.left(), yPos - r.top());
        }

        if (notifiedOld && notifiedNew)
            break;
    }
}
void WindowImpl::mouseButton(unsigned int buttonID, bool down, int clickCount) {
    Widget *wid = getWidgetAtPoint(mMouseX, mMouseY, true);
    if (wid) {
        (wid)->mouseButton(buttonID, down, clickCount);
    }
}
void WindowImpl::mouseWheel(int xScroll, int yScroll) {
    Widget *wid = getWidgetAtPoint(mMouseX, mMouseY, true);
    if (wid) {
        wid->mouseWheel(xScroll, yScroll);
    }
}

void WindowImpl::textEvent(const wchar_t* evt, size_t evtLength) {
    FrontToBackIter iter = frontIter();
    if (iter != frontEnd()) {
        (*iter)->textEvent(evt,evtLength);
    }
}
void WindowImpl::keyEvent(bool pressed, int mods, int vk_code, int scancode) {
    FrontToBackIter iter = frontIter();
    if (iter != frontEnd()) {
        (*iter)->keyEvent(pressed, mods, vk_code, scancode);
    }
}



void WindowImpl::resize(int width, int height) {
    SetContainerBounds(gfx::Rect(0, 0, width, height));
}

void WindowImpl::cut() {
    if (host()) host()->Cut();
}
void WindowImpl::copy() {
    if (host()) host()->Copy();
}
void WindowImpl::paste() {
    if (host()) host()->Paste();
}
void WindowImpl::undo() {
    if (host()) host()->Undo();
}
void WindowImpl::redo() {
    if (host()) host()->Redo();
}
void WindowImpl::del() {
    if (host()) host()->Delete();
}
void WindowImpl::selectAll() {
    if (host()) host()->SelectAll();
}

void WindowImpl::refresh() {
    mController->Reload(true);
    // mController->ReloadIgnoringCache(true)
}

void WindowImpl::stop() {
  if (host()) {
    host()->Stop();
  }
}

void WindowImpl::adjustZoom(int mode) {
  if (host()) {
    host()->Zoom((PageZoom::Function)mode);
  }
}

void WindowImpl::goBack() {
  mController->GoBack();
}

void WindowImpl::goForward() {
  mController->GoForward();
}

bool WindowImpl::canGoBack() const {
  return mController->CanGoBack();
}

bool WindowImpl::canGoForward() const {
  return mController->CanGoForward();
}

void WindowImpl::SetContainerBounds (const gfx::Rect &rc) {
    mRect = rc;
    RenderWidgetHostView* myview = view();
    if (myview) {
        myview->SetSize(gfx::Size(mRect.width(), mRect.height()));
    }
    RenderViewHost* myhost = host();
    if (myhost) {
        static_cast<MemoryRenderViewHost*>(myhost)->Memory_WasResized();
    }
}

void WindowImpl::executeJavascript(WideString javascript) {
    if (host() && !mIsReentrant) {
        mIsReentrant = true;
        host()->ExecuteJavascriptInWebFrame(string16(), WideToUTF16(javascript.get<std::wstring>()));
        mIsReentrant = false;
    }
}

void WindowImpl::insertCSS(WideString css, WideString id) {
    if (host()) {
        std::string cssUtf8, idUtf8;
        WideToUTF8(css.data(), css.length(), &cssUtf8);
        WideToUTF8(id.data(), id.length(), &idUtf8);
        host()->InsertCSSInWebFrame(std::wstring(), cssUtf8, idUtf8);
    }
}

bool WindowImpl::navigateTo(URLString url) {
    this->mCurrentURL = GURL(url.get<std::string>());
    mController->LoadURL(this->mCurrentURL, GURL(), PageTransition::TYPED);
    return true;
}

void WindowImpl::TooltipChanged(const std::wstring& tooltipText) {
   if (mDelegate)
       mDelegate->onTooltipChanged(this, WideString::point_to(tooltipText));
}

void WindowImpl::UpdateMaxPageID(int32 page_id) {
  // Ensure both the SiteInstance and RenderProcessHost update their max page
  // IDs in sync. Only TabContents will also have site instances, except during
  // testing.
  if (GetSiteInstance())
    GetSiteInstance()->UpdateMaxPageID(page_id);
  process()->UpdateMaxPageID(page_id);
}

int32 WindowImpl::GetMaxPageID() {
  if (GetSiteInstance())
    return GetSiteInstance()->max_page_id();
  return 0;
}



Profile* WindowImpl::profile() const{
    //return Root::getSingleton().getProfile();
    return getContextImpl()->profile();
}

ContextImpl *WindowImpl::getContextImpl() const {
    return static_cast<ContextImpl*>(getContext());
}

void WindowImpl::onPaint(Widget *wid,
                         const unsigned char *sourceBuffer,
                         const Rect &sourceBufferRect,
                         size_t numCopyRects,
                         const Rect *copyRects,
                         int dx, int dy, const Rect &scrollRect) {
    if (mDelegate) {
        if (wid) {
            mDelegate->onWidgetPaint(
                this, wid,
                sourceBuffer, sourceBufferRect,
                numCopyRects, copyRects,
                dx, dy, scrollRect);
        } else {
            mDelegate->onPaint(
                this,
                sourceBuffer, sourceBufferRect,
                numCopyRects, copyRects,
                dx, dy, scrollRect);
        }
    }
}

void WindowImpl::onWidgetDestroyed(Widget *wid) {
    if (wid != getWidget()) {
        if (mDelegate) {
            mDelegate->onWidgetDestroyed(this, wid);
        }
    }
    removeWidget(wid);
}

/******* RenderViewHostManager::Delegate *******/

bool WindowImpl::CreateRenderViewForRenderManager(
    RenderViewHost* render_view_host,
    bool remote_view_exists) {

  RenderWidget *rwh_view = new RenderWidget(this, mId);
  rwh_view->setHost(render_view_host);

  // If we're creating a renderview for a window that was opened by the
  //  renderer process remotely, calling CreateRenderView will crash the
  //  renderer by attempting to create a second copy of the window
  if (!remote_view_exists) {
      if (!render_view_host->CreateRenderView(string16()))
      return false;
  }

  // Now that the RenderView has been created, we need to tell it its size.
  rwh_view->SetSize(gfx::Size(mRect.width(), mRect.height()));
  render_view_host->set_view(rwh_view);

  appendWidget(rwh_view);

//  UpdateMaxPageIDIfNecessary(render_view_host->site_instance(),
//                             render_view_host);
  return true;

}

bool WindowImpl::OnMessageReceived(const IPC::Message& message) {
    bool handled = true;
    bool message_is_ok = true;
    IPC_BEGIN_MESSAGE_MAP_EX(TabContents, message, message_is_ok)
      IPC_MESSAGE_HANDLER(ViewHostMsg_DidStartProvisionalLoadForFrame,
                          OnDidStartProvisionalLoadForFrame)
      IPC_MESSAGE_HANDLER(ViewHostMsg_DidRedirectProvisionalLoad,
                          OnDidRedirectProvisionalLoad)
      IPC_MESSAGE_HANDLER(ViewHostMsg_DidFailProvisionalLoadWithError,
                          OnDidFailProvisionalLoadWithError)
      IPC_MESSAGE_HANDLER(ViewHostMsg_DocumentLoadedInFrame,
                          OnDocumentLoadedInFrame)
      IPC_MESSAGE_HANDLER(ViewHostMsg_DidFinishLoad, OnDidFinishLoad)
      IPC_MESSAGE_HANDLER(ViewHostMsg_GoToEntryAtOffset, OnGoToEntryAtOffset)
      IPC_MESSAGE_HANDLER(ViewHostMsg_MissingPluginStatus, OnMissingPluginStatus)
      IPC_MESSAGE_HANDLER(ViewHostMsg_CrashedPlugin, OnCrashedPlugin)
//      IPC_MESSAGE_HANDLER(ViewHostMsg_BlockedOutdatedPlugin,
//                          OnBlockedOutdatedPlugin)
      IPC_MESSAGE_HANDLER(ViewHostMsg_PageContents, OnPageContents)
      IPC_MESSAGE_HANDLER(ViewHostMsg_PageTranslated, OnPageTranslated)
      IPC_MESSAGE_HANDLER(ViewHostMsg_RequestMove, onResizeRequested)
      IPC_MESSAGE_HANDLER(ViewHostMsg_RunFileChooser, RunFileChooser)
      IPC_MESSAGE_UNHANDLED(handled = false)
    IPC_END_MESSAGE_MAP_EX()

    if (!message_is_ok) {
      process()->ReceivedBadMessage();
    }

    return handled;
}

const GURL& WindowImpl::GetURL() const {
	return mCurrentURL;
}


void WindowImpl::DidStartLoading() {
    if (isLoading)
        return;

    SetIsLoading(true);

    if (mDelegate) {
        mDelegate->onLoadingStateChanged(this, true);
    }
}

void WindowImpl::DocumentAvailableInMainFrame( RenderViewHost *pRVH )
{
	evalInitialJavascript();
	RenderViewHostDelegate::DocumentAvailableInMainFrame( pRVH );
}

void WindowImpl::DidStopLoading() {
    SetIsLoading(false);

    if (mDelegate) {
        mDelegate->onLoadingStateChanged(this, false);
    }
}

void WindowImpl::OnAddMessageToConsole(
        const std::wstring& message,
        int32 line_no,
        const std::wstring& source_id)
{
    if (mDelegate) {
        mDelegate->onConsoleMessage(this, WideString::point_to(message),
                                    WideString::point_to(source_id), line_no);
    }
}

void WindowImpl::UpdateCursor(const WebCursor& cursor) {
#if BERKELIUM_PLATFORM == PLATFORM_WINDOWS
    // Normally you'd pass a handle to chrome.dll (or for us, berkelium.dll) to GetCursor
    //  so it could pull out cursor resources, but we don't link resources into berkelium.dll.
    // Also: augh, why is GetCursor not const on windows?
    WebCursor& cursor_ = const_cast<WebCursor&>(cursor);
    HCURSOR cursorHandle = cursor_.GetCursor(0);
    Cursor new_cursor(cursorHandle);
#elif BERKELIUM_PLATFORM == PLATFORM_LINUX
    WebCursor cursorCopy = cursor;
    // Returns an int to avoid including Gdk headers--so we have to cast.
    GdkCursorType cursorType = (GdkCursorType)cursorCopy.GetCursorType();
    GdkCursor* cursorPtr = cursorType == GDK_CURSOR_IS_PIXMAP ? cursorCopy.GetCustomCursor() : NULL;
    Cursor new_cursor(cursorType, cursorPtr);
#elif BERKELIUM_PLATFORM == PLATFORM_MAC
    Cursor new_cursor( cursor.GetCursor() );
#else
    Cursor new_cursor;
#endif
    if (mDelegate) mDelegate->onCursorUpdated(this, new_cursor);
}

/******* RenderViewHostDelegate *******/

RenderViewHostDelegate::View* WindowImpl::GetViewDelegate() {
    return this;
}

RendererPreferences WindowImpl::GetRendererPrefs(Profile*) const {
    RendererPreferences ret;
    renderer_preferences_util::UpdateFromSystemSettings(
        &ret, profile());

    // We may want to support this eventually...
    // But until we properly support top-level requests, this causes certain
    // popup windows (such as descriptions of youtube videos) to redirect
    // infinitely, or else never load with a provisional load error
    //     net::ERR_ABORTED (-3)
    // Enable the following line at your own risk
    //ret.browser_handles_top_level_requests = true;
    return ret;
}

WebPreferences WindowImpl::GetWebkitPrefs() {
    WebPreferences web_prefs;
    web_prefs.experimental_webgl_enabled = true;
    web_prefs.allow_file_access_from_file_urls = true;
    web_prefs.uses_universal_detector = true;
    return web_prefs;
}

void WindowImpl::UpdateHistoryForNavigation(
	scoped_refptr<history::HistoryAddPageArgs> add_page_args)
{
  if (profile()->IsOffTheRecord())
    return;

  // Add to history service.
  HistoryService* hs = profile()->GetHistoryService(Profile::IMPLICIT_ACCESS);
  if (hs)
    hs->AddPage(*add_page_args);
}

ViewMsg_Navigate_Params::NavigationType GetNavigationType(
    Profile* profile, const NavigationEntry& entry,
    NavigationController::ReloadType reload_type) {
  switch (reload_type) {
    case NavigationController::RELOAD:
      return ViewMsg_Navigate_Params::RELOAD;
    case NavigationController::RELOAD_IGNORING_CACHE:
      return ViewMsg_Navigate_Params::RELOAD_IGNORING_CACHE;
    case NavigationController::NO_RELOAD:
      break;  // Fall through to rest of function.
  }

  if (entry.restore_type() == NavigationEntry::RESTORE_LAST_SESSION &&
      profile->DidLastSessionExitCleanly())
    return ViewMsg_Navigate_Params::RESTORE;

  return ViewMsg_Navigate_Params::NORMAL;
}


static void MakeNavigateParams(const NavigationEntry& entry,
                        const NavigationController& controller,
                        NavigationController::ReloadType reload_type,
                        ViewMsg_Navigate_Params* params) {
  params->page_id = entry.page_id();
  params->pending_history_list_offset = controller.GetIndexOfEntry(&entry);
  params->current_history_list_offset = controller.last_committed_entry_index();
  params->current_history_list_length = controller.entry_count();
  params->url = entry.url();
  params->referrer = entry.referrer();
  params->transition = entry.transition_type();
  params->state = entry.content_state();
  params->navigation_type =
      GetNavigationType(controller.profile(), entry, reload_type);
  params->request_time = base::Time::Now();
}


bool WindowImpl::NavigateToPendingEntry(NavigationController::ReloadType reload_type) {
  const NavigationEntry& entry = *(mController->pending_entry());
  RenderViewHost* dest_render_view_host = host();

// FIXME(patrick): computing preferred size is a requested feature. We should expose this
/*
  if (delegate_ && delegate_->ShouldEnablePreferredSizeNotifications()) {
    dest_render_view_host->EnablePreferredSizeChangedMode(
        kPreferredSizeWidth | kPreferredSizeHeightThisIsSlow);
  }
*/

  // Tell DevTools agent that it is attached prior to the navigation.
/*
  DevToolsManager* devtools_manager = DevToolsManager::GetInstance();
  if (devtools_manager) {  // NULL in unit tests.
    devtools_manager->OnNavigatingToPendingEntry(render_view_host(),
                                                 dest_render_view_host,
                                                 entry.url());
  }
*/

  // Navigate in the desired RenderViewHost.
  ViewMsg_Navigate_Params navigate_params;
  MakeNavigateParams(entry, *mController, reload_type, &navigate_params);
  dest_render_view_host->Navigate(navigate_params);

  if (entry.page_id() == -1) {
    // HACK!!  This code suppresses javascript: URLs from being added to
    // session history, which is what we want to do for javascript: URLs that
    // do not generate content.  What we really need is a message from the
    // renderer telling us that a new page was not created.  The same message
    // could be used for mailto: URLs and the like.
    if (entry.url().SchemeIs("javascript"))
      return false;
  }

  return true;
}

bool WindowImpl::UpdateTitleForEntry(NavigationEntry* entry,
                                     const std::wstring& title) {
  // For file URLs without a title, use the pathname instead. In the case of a
  // synthesized title, we don't want the update to count toward the "one set
  // per page of the title to history."
  string16 final_title;
  bool explicit_set;
  if (entry->url().SchemeIsFile() && title.empty()) {
    final_title = UTF8ToUTF16(entry->url().ExtractFileName());
    explicit_set = false;  // Don't count synthetic titles toward the set limit.
  } else {
    TrimWhitespace(WideToUTF16Hack(title), TRIM_ALL, &final_title);
    explicit_set = true;
  }

  if (final_title == entry->title()) {
    if (mDelegate) {
      std::wstring wtitle(UTF16ToWideHack(final_title));
      mDelegate->onTitleChanged(this, WideString::point_to(wtitle));
    }

    return false;  // Nothing changed, don't bother.
  }

  entry->set_title(final_title);

  // Update the history system for this page.
  if (!profile()->IsOffTheRecord() && !received_page_title_) {
    HistoryService* hs =
        profile()->GetHistoryService(Profile::IMPLICIT_ACCESS);
    if (hs)
      hs->SetPageTitle(entry->virtual_url(), final_title);

    // Don't allow the title to be saved again for explicitly set ones.
    received_page_title_ = explicit_set;
  }

  if (mDelegate) {
    std::wstring wtitle(UTF16ToWideHack(final_title));
    mDelegate->onTitleChanged(this, WideString::point_to(wtitle));
  }

  // Lastly, set the title for the view.
  //view()->SetPageTitle(final_title);

  return true;
}



void WindowImpl::RendererUnresponsive(RenderViewHost* rvh,
                                  bool is_during_unload) {
  if (rvh != host()) {
    return;
  }
  if (mDelegate) mDelegate->onUnresponsive(this);
}
void WindowImpl::RendererResponsive(RenderViewHost* rvh) {
  if (rvh != host()) {
    return;
  }
  if (mDelegate) mDelegate->onResponsive(this);
}

void WindowImpl::RenderViewReady(RenderViewHost* rvh) {
  if (rvh != host()) {
    // Don't notify the world, since this came from a renderer in the
    // background.
    return;
  }

  bool was_crashed = is_crashed();
  SetIsCrashed(false);

  // Restore the focus to the tab (otherwise the focus will be on the top
  // window).
  if (was_crashed)
      view()->Focus();
}

void WindowImpl::RenderViewGone(RenderViewHost* rvh, base::TerminationStatus status, int error_code) {
  // Ask the print preview if this renderer was valuable.
  if (rvh != host()) {
    // The pending page's RenderViewHost is gone.
    return;
  }

  SetIsLoading(false);
  SetIsCrashed(true);

  // Tell the view that we've crashed so it can prepare the sad tab page.
  //view()->OnTabCrashed();
  if (mDelegate) mDelegate->onCrashed(this);
}

void WindowImpl::OnUserGesture(){
}
void WindowImpl::OnGoToEntryAtOffset(int offset) {
    std::cout << "GOING TO ENTRY AT OFFSET "<<offset<<std::endl;
    mController->GoToOffset(offset);
}
void WindowImpl::OnGetHistoryListCount(int* back_list_count,
                                     int* forward_list_count){
  int current_index = mController->last_committed_entry_index();
  *back_list_count = current_index;
  *forward_list_count = mController->entry_count() - current_index - 1;
}
void WindowImpl::OnMissingPluginStatus(int status){
}
void WindowImpl::OnCrashedPlugin(const FilePath& plugin_path) {
    DCHECK(!plugin_path.value().empty());

    std::wstring plugin_name = plugin_path.ToWStringHack();
#if defined(OS_WIN) || defined(OS_MACOSX)
    scoped_ptr<FileVersionInfo> version_info(
        FileVersionInfo::CreateFileVersionInfo(plugin_path));
    if (version_info.get()) {
        const string16& product_name = version_info->product_name();
        if (!product_name.empty()) {
            plugin_name = UTF16ToWide(product_name);
#if defined(OS_MACOSX)
            // Many plugins on the Mac have .plugin in the actual name, which looks
            // terrible, so look for that and strip it off if present.
            const std::wstring plugin_extension(L".plugin");
            if (EndsWith(plugin_name, plugin_extension, true))
                plugin_name.erase(plugin_name.length() - plugin_extension.length());
#endif  // OS_MACOSX
        }
    }
#endif
    if (mDelegate) {
        mDelegate->onCrashedPlugin(this, WideString::point_to(plugin_name));
    }
}
void WindowImpl::WorkerCrashed(){
    if (mDelegate) {
        mDelegate->onCrashedWorker(this);
    }
}

void WindowImpl::onResizeRequested(gfx::Rect rect) {
    if (mDelegate) {
        mDelegate->onResizeRequested(this, rect.x(), rect.y(), rect.width(), rect.height());
    }
}

void WindowImpl::OnPageContents(
    const GURL& url,
    int32 page_id,
    const string16& contents,
    const std::string& language,
    bool page_translatable) {
}

void WindowImpl::OnPageTranslated(
    int32 page_id,
    const std::string& original_lang,
    const std::string& translated_lang,
    TranslateErrors::Type error_type) {
}
void WindowImpl::OnDisabledOutdatedPlugin(const string16&, const GURL&) {
}

void WindowImpl::ProcessExternalHostMessage(const std::string& message,
                                            const std::string& origin,
                                            const std::string& target)
{
    if (target == "urn:uuid:" + WideToUTF8(mUniqueId)) {
        javascriptCall(NULL, URLString::point_to(origin), UTF8ToWide(message));
        return;
    }

    if (mDelegate) {
        std::wstring wide_message(UTF8ToWide(message));
        mDelegate->onExternalHost(this,
                                  WideString::point_to(wide_message),
                                  URLString::point_to(origin),
                                  URLString::point_to(target));
    }
}


void WindowImpl::DidNavigate(RenderViewHost* rvh,
                              const ViewHostMsg_FrameNavigate_Params& params) {
  int extra_invalidate_flags = 0;

  // Update the site of the SiteInstance if it doesn't have one yet.
  if (!GetSiteInstance()->has_site())
    GetSiteInstance()->SetSite(params.url);

  // Need to update MIME type here because it's referred to in
  // UpdateNavigationCommands() called by RendererDidNavigate() to
  // determine whether or not to enable the encoding menu.
  // It's updated only for the main frame. For a subframe,
  // RenderView::UpdateURL does not set params.contents_mime_type.
  // (see http://code.google.com/p/chromium/issues/detail?id=2929 )
  // TODO(jungshik): Add a test for the encoding menu to avoid
  // regressing it again.

  /*if (PageTransition::IsMainFrame(params.transition))
        contents_mime_type_ = params.contents_mime_type;
  */

  NavigationController::LoadCommittedDetails details;
  bool did_navigate = mController->RendererDidNavigate(
      params, extra_invalidate_flags, &details);

  // Update history. Note that this needs to happen after the entry is complete,
  // which WillNavigate[Main,Sub]Frame will do before this function is called.
  if (params.should_update_history) {
    // Most of the time, the displayURL matches the loaded URL, but for about:
    // URLs, we use a data: URL as the real value.  We actually want to save
    // the about: URL to the history db and keep the data: URL hidden. This is
    // what the TabContents' URL getter does.
    scoped_refptr<history::HistoryAddPageArgs> add_page_args(
        CreateHistoryAddPageArgs(GetURL(), details, params));
	UpdateHistoryForNavigation(add_page_args);
  }

  if (!did_navigate)
    return;  // No navigation happened.

  // DO NOT ADD MORE STUFF TO THIS FUNCTION! Your component should either listen
  // for the appropriate notification (best) or you can add it to
  // DidNavigateMainFramePostCommit / DidNavigateAnyFramePostCommit (only if
  // necessary, please).

  // Run post-commit tasks.
  if (details.is_main_frame) {
// FIXME(pathorn): Look into what TabContents uses these functions for.
      //DidNavigateMainFramePostCommit(details, params);
  }
  //DidNavigateAnyFramePostCommit(details, params);
}


scoped_refptr<history::HistoryAddPageArgs>
WindowImpl::CreateHistoryAddPageArgs(
    const GURL& virtual_url,
    const NavigationController::LoadCommittedDetails& details,
    const ViewHostMsg_FrameNavigate_Params& params) {
  scoped_refptr<history::HistoryAddPageArgs> add_page_args(
      new history::HistoryAddPageArgs(
          params.url, base::Time::Now(), this, params.page_id, params.referrer,
          params.redirects, params.transition, history::SOURCE_BROWSED,
          details.did_replace_entry));
  if (PageTransition::IsMainFrame(params.transition) &&
      virtual_url != params.url) {
    // Hack on the "virtual" URL so that it will appear in history. For some
    // types of URLs, we will display a magic URL that is different from where
    // the page is actually navigated. We want the user to see in history what
    // they saw in the URL bar, so we add the virtual URL as a redirect.  This
    // only applies to the main frame, as the virtual URL doesn't apply to
    // sub-frames.
    add_page_args->url = virtual_url;
    if (!add_page_args->redirects.empty())
      add_page_args->redirects.back() = virtual_url;
  }
  return add_page_args;
}


void WindowImpl::UpdateState(RenderViewHost* rvh,
                              int32 page_id,
                              const std::string& state) {
  DCHECK(rvh == host());

  // We must be prepared to handle state updates for any page, these occur
  // when the user is scrolling and entering form data, as well as when we're
  // leaving a page, in which case our state may have already been moved to
  // the next page. The navigation controller will look up the appropriate
  // NavigationEntry and update it when it is notified via the delegate.

  int entry_index = mController->GetEntryIndexWithPageID(
      GetSiteInstance(), page_id);
  if (entry_index < 0)
    return;
  NavigationEntry* entry = mController->GetEntryAtIndex(entry_index);

  if (state == entry->content_state())
    return;  // Nothing to update.
  entry->set_content_state(state);
  mController->NotifyEntryChanged(entry, entry_index);
}

void WindowImpl::NavigationEntryCommitted(NavigationController::LoadCommittedDetails* details) {
	GURL url = details->entry->url();
	const std::string&spec=url.spec();
	if (mDelegate) {
		mDelegate->onAddressBarChanged(this, URLString::point_to(spec));
	}
}

void WindowImpl::UpdateTitle(RenderViewHost* rvh,
                              int32 page_id, const std::wstring& title) {
  // If we have a title, that's a pretty good indication that we've started
  // getting useful data.
  //SetNotWaitingForResponse();

  DCHECK(rvh == host());
  NavigationEntry* entry = mController->GetEntryWithPageID(GetSiteInstance(),
                                                            page_id);
  if (!entry || !UpdateTitleForEntry(entry, title))
    return;
}

void WindowImpl::ShowRepostFormWarningDialog() {
}

void WindowImpl::RunFileChooser(const ViewHostMsg_RunFileChooser_Params&params) {
  if (mDelegate) {
      std::wstring title = UTF16ToWideHack(params.title);
      const FilePath::StringType &filepath = params.default_file_name.value();
      int mode;
      switch (params.mode) {
        case ViewHostMsg_RunFileChooser_Params::OpenMultiple:
          mode = FileOpenMultiple;
          break;
        case ViewHostMsg_RunFileChooser_Params::OpenFolder:
          mode = FileOpenFolder;
          break;
        case ViewHostMsg_RunFileChooser_Params::Save:
          mode = FileSaveAs;
          break;
        default: //case ViewHostMsg_RunFileChooser_Params::Open or unknown
          mode = FileOpen;
          break;
      }
      mDelegate->onRunFileChooser(this, mode,
                                  WideString::point_to(title),
                                  FileString::point_to(filepath));
  }
}

void WindowImpl::filesSelected(FileString *listOfFiles) {
    std::vector<FilePath> files;
    // Empty list means cancelled (default)
    if (listOfFiles) {
        for (size_t i = 0; listOfFiles[i].length(); i++) {
            files.push_back(FilePath(listOfFiles[i].get<FilePath::StringType>()));
        }
    }
    host()->FilesSelectedInChooser(files);
/* Don't have access to a top level window, since this could be run in a
   windowless environment. Perhaps we need a function to return a native
   window handle or NULL if it does not want to allow file choosers.
*/
/*
  if (!mSelectFileDialog.get())
    mSelectFileDialog = SelectFileDialog::Create(this);
  SelectFileDialog::Type dialog_type =
    multiple_files ? SelectFileDialog::SELECT_OPEN_MULTI_FILE :
                     SelectFileDialog::SELECT_OPEN_FILE;
  mSelectFileDialog->SelectFile(dialog_type, title, default_file,
                                  NULL, 0, FILE_PATH_LITERAL(""),
                                  view()->GetTopLevelNativeWindow(), NULL);
*/
}

void WindowImpl::RequestOpenURL(const GURL& url, const GURL& referrer,
                                 WindowOpenDisposition disposition) {
  bool isNewWindow = false;
  bool cancelDefault = false;

  switch (disposition) {
    case NEW_FOREGROUND_TAB:
    case NEW_BACKGROUND_TAB:
    case NEW_POPUP:
    case NEW_WINDOW:
      isNewWindow = true;
      break;
    default:
      break;
  }

  if (mDelegate) {
    std::string urlstring (url.spec());
    std::string referrerstring (referrer.spec());

    mDelegate->onNavigationRequested(
      this,
      URLString::point_to(urlstring),
      URLString::point_to(referrerstring),
      isNewWindow, cancelDefault
    );
  }

  if (!cancelDefault)
    mController->LoadURL(url, referrer, PageTransition::LINK);
}

void WindowImpl::DomOperationResponse(const std::string& json_string,
                                       int automation_id) {
}

void WindowImpl::RunJavaScriptMessage(
    const std::wstring& message,
    const std::wstring& default_prompt,
    const GURL& frame_url,
    const int flags,
    IPC::Message* reply_msg,
    bool* did_suppress_message)
{
    bool success = false;
    std::wstring promptstr;

    if (default_prompt == L"urn:uuid:" + mUniqueId
            && flags == ui::MessageBoxFlags::kIsJavascriptPrompt) {
        GURL origin = frame_url.GetOrigin();
        const std::string &urlspec = origin.spec();
        if (!javascriptCall(reply_msg, URLString::point_to(urlspec), message)) {
            synchronousScriptReturn(reply_msg, Script::Variant());
        }
        return;
    }

    if (mDelegate) {
        std::string frame_url_str (frame_url.spec());
        WideString prompt = WideString::empty();
		int bkflags = 0;
		switch (flags) {
        case ui::MessageBoxFlags::kIsJavascriptConfirm:
			bkflags = JavascriptConfirm;
			break;
        case ui::MessageBoxFlags::kIsJavascriptPrompt:
			bkflags = JavascriptPrompt;
			break;
		default:
			bkflags = JavascriptAlert;
			break;
		}
        mDelegate->onScriptAlert(
            this, WideString::point_to(message),
            WideString::point_to(default_prompt),
            URLString::point_to(frame_url_str),
            bkflags, success, prompt
        );
        prompt.get(promptstr);
        mDelegate->freeLastScriptAlert(prompt);
    }
    if (host()) {
        host()->JavaScriptMessageBoxClosed(reply_msg, success, promptstr);
    }
}

void WindowImpl::Close(RenderViewHost* rvh) {
}


/******* RenderViewHostDelegate::Resource *******/

void WindowImpl::OnDidStartProvisionalLoadForFrame(
        int64 frame_id,
        bool is_main_frame,
        const GURL& url) {
    if (!is_main_frame) {
        return;
    }

    if (mDelegate) {
		bool cancelDefault = false;
        const std::string &urlstring = url.spec();
        const std::string &referrerstring = this->mCurrentURL.spec();

        // Note: cancelDefault is currently broken. Don't depend on this feature.
        if (urlstring != referrerstring) {
            mDelegate->onNavigationRequested(
              this,
              URLString::point_to(urlstring),
              URLString::point_to(referrerstring),
              false, cancelDefault
            );
        }
        if (cancelDefault) {
            // FIXME: Hack for browser_handles_top_level_requests disabled.
            //mRenderViewHost->Stop();
            //this->goBack();
            //return;
        }
    }

    this->mCurrentURL = url;

    if (mDelegate) {
        const std::string&spec=url.spec();
        mDelegate->onStartLoading(this, URLString::point_to(spec));
        mDelegate->onAddressBarChanged(this, URLString::point_to(spec));
    }
}

void WindowImpl::OnDidRedirectProvisionalLoad(
    int32 page_id,
    const GURL& source_url,
    const GURL& target_url)
{
    // should use page_id to lookup in history
    NavigationEntry *entry;
    if (page_id == -1)
        entry = mController->pending_entry();
    else
        entry = mController->GetEntryWithPageID(GetSiteInstance(), page_id);
    if (!entry || entry->url() != source_url)
        return;
    entry->set_url(target_url);
}

void WindowImpl::OnDidRedirectResource(const ResourceRedirectDetails& details) {
    // Only accessor function:
    // details->new_url();
}

void WindowImpl::OnDidFailProvisionalLoadWithError(
        int64 frame_id,
        bool is_main_frame,
        int error_code,
        const GURL& url,
        bool showing_repost_interstitial) {
    if (mDelegate) {
      std::string urlstring (url.spec());
      mDelegate->onProvisionalLoadError(this, URLString::point_to(urlstring),
                                        error_code, is_main_frame);
    }
}

void WindowImpl::OnDocumentLoadedInFrame(int64 frame_id) {
/*
    if (mDelegate) {
        mDelegate->onLoad(this);
    }
*/
}


void WindowImpl::OnDidFinishLoad(int64 frame_id) {
    if (mDelegate) {
        mDelegate->onLoad(this);
    }
}



/******* RenderViewHostDelegate::View *******/
void WindowImpl::CreateNewWindow(int route_id, const ViewHostMsg_CreateWindow_Params&params) {
    // A window shown in popup or tab.
    //WINDOW_CONTAINER_TYPE_NORMAL = 0,
    // A window run as a hidden "background" page.
    //WINDOW_CONTAINER_TYPE_BACKGROUND,
    // A window run as a hidden "background" page that wishes to be started
    // upon browser launch and run beyond the lifetime of the pages that
    // reference it.
    //WINDOW_CONTAINER_TYPE_PERSISTENT,

    // std::cout<<"Created window "<<route_id<<std::endl;
    Context * context = getContext();
    WindowImpl * newWindow = new WindowImpl(context, route_id);

    mNewlyCreatedWindows.insert(
        std::pair<int, WindowImpl*>(route_id, newWindow));
}

void WindowImpl::CreateNewWidget(int route_id, WebKit::WebPopupType popup_type) {
    //WebPopupTypeNone, // Not a popup.
    //WebPopupTypeSelect, // A HTML select (combo-box) popup.
    //WebPopupTypeSuggestion, // An autofill/autocomplete popup.

    // std::cout<<"Created widget "<<route_id<<std::endl;
    RenderWidget* wid = new RenderWidget(this, route_id);
    wid->setHost(new MemoryRenderWidgetHost(this, wid, process(), route_id));
    //wid->set_activatable(activatable); // ???????
    mNewlyCreatedWidgets.insert(
        std::pair<int, RenderWidget*>(route_id, wid));
}
void WindowImpl::ShowCreatedWindow(int route_id,
                                   WindowOpenDisposition disposition,
                                   const gfx::Rect& initial_pos,
                                   bool user_gesture) {
    // std::cout<<"Show Created window "<<route_id<<std::endl;
    std::map<int, WindowImpl*>::iterator iter = mNewlyCreatedWindows.find(route_id);
    assert(iter != mNewlyCreatedWindows.end());
    WindowImpl *newwin = iter->second;
    mNewlyCreatedWindows.erase(iter);
    newwin->host()->Init();

    if (initial_pos.width() != 0 && initial_pos.height() != 0) {
        newwin->resize(initial_pos.width(), initial_pos.height());
    }

    if (!newwin->process()->HasConnection()) {
        // The view has gone away or the renderer crashed. Nothing to do.
        // Fixme: memory leak?
        std::cout<<"Show window process fail "<<route_id<<std::endl;
        return;
    }

    if (mDelegate) {
        Rect rect;
        rect.setFromRect(initial_pos);
        mDelegate->onCreatedWindow(this, newwin, rect);
    }
}
void WindowImpl::ShowCreatedWidget(int route_id,
                                   const gfx::Rect& initial_pos) {
    // std::cout<<"Show Created widget "<<route_id<<std::endl;
    std::map<int, RenderWidget*>::iterator iter = mNewlyCreatedWidgets.find(route_id);
    assert(iter != mNewlyCreatedWidgets.end());
    RenderWidget *wid = iter->second;
    appendWidget(wid);
    mNewlyCreatedWidgets.erase(iter);

    if (!wid->GetRenderWidgetHost()->process()->HasConnection()) {
        // The view has gone away or the renderer crashed. Nothing to do.
        // Fixme: memory leak?
        std::cout<<"Show widget process fail "<<route_id<<std::endl;
        return;
    }

    int thisZ = ++zIndex;

    wid->InitAsPopup(view(), initial_pos);
    wid->GetRenderWidgetHost()->Init();

    wid->SetSize(gfx::Size(initial_pos.width(), initial_pos.height()));
    wid->setPos(initial_pos.x(), initial_pos.y());
    if (mDelegate) {
        mDelegate->onWidgetCreated(this, wid, thisZ);
        mDelegate->onWidgetResize(this, wid,
                                  initial_pos.width(), initial_pos.height());
        mDelegate->onWidgetMove(this, wid,
                                initial_pos.x(), initial_pos.y());
    }

}
void WindowImpl::ShowContextMenu(const ContextMenuParams& params) {
    if (!mDelegate)
      return;

    ContextMenuEventArgs args;
    memset(&args, 0, sizeof(args));

    std::string linkUrl (params.link_url.spec());
    std::string srcUrl (params.src_url.spec());
    std::string pageUrl (params.page_url.spec());
    std::string frameUrl (params.frame_url.spec());

    args.linkUrl = URLString::point_to(linkUrl);
    args.srcUrl = URLString::point_to(srcUrl);
    args.pageUrl = URLString::point_to(pageUrl);
    args.frameUrl = URLString::point_to(frameUrl);
    std::wstring selection_text_storage (UTF16ToWide(params.selection_text));
    args.selectedText = WideString::point_to(selection_text_storage);

    args.mediaType = ContextMenuEventArgs::MediaTypeNone;
    switch (params.media_type) {
    case WebKit::WebContextMenuData::MediaTypeNone:
        args.mediaType = ContextMenuEventArgs::MediaTypeNone;
        break;
    case WebKit::WebContextMenuData::MediaTypeImage:
        args.mediaType = ContextMenuEventArgs::MediaTypeImage;
        break;
    case WebKit::WebContextMenuData::MediaTypeAudio:
        args.mediaType = ContextMenuEventArgs::MediaTypeAudio;
        break;
    case WebKit::WebContextMenuData::MediaTypeVideo:
        args.mediaType = ContextMenuEventArgs::MediaTypeVideo;
        break;
    default:
        break;
    }
    args.mouseX = params.x;
    args.mouseY = params.y;
    args.isEditable = params.is_editable;
    args.editFlags = ContextMenuEventArgs::CanDoNone;
    if (params.edit_flags & WebKit::WebContextMenuData::CanUndo) {
        args.editFlags |= ContextMenuEventArgs::CanUndo;
    }
    if (params.edit_flags & WebKit::WebContextMenuData::CanRedo) {
        args.editFlags |= ContextMenuEventArgs::CanRedo;
    }
    if (params.edit_flags & WebKit::WebContextMenuData::CanCut) {
        args.editFlags |= ContextMenuEventArgs::CanCut;
    }
    if (params.edit_flags & WebKit::WebContextMenuData::CanCopy) {
        args.editFlags |= ContextMenuEventArgs::CanCopy;
    }
    if (params.edit_flags & WebKit::WebContextMenuData::CanPaste) {
        args.editFlags |= ContextMenuEventArgs::CanPaste;
    }
    if (params.edit_flags & WebKit::WebContextMenuData::CanDelete) {
        args.editFlags |= ContextMenuEventArgs::CanDelete;
    }
    if (params.edit_flags & WebKit::WebContextMenuData::CanSelectAll) {
        args.editFlags |= ContextMenuEventArgs::CanSelectAll;
    }

    mDelegate->onShowContextMenu(this, args);
}
void WindowImpl::StartDragging(const WebDropData& drop_data,
                               WebKit::WebDragOperationsMask allowed_ops,
                               const SkBitmap& image,
                               const gfx::Point& image_offset) {
    // TODO: Add dragging event
    host()->DragSourceSystemDragEnded();
}
void WindowImpl::UpdateDragCursor(WebKit::WebDragOperation operation) {
    // TODO: Add dragging event
}
void WindowImpl::GotFocus() {
    // Useless: just calls this when we hand it an input event.
}
void WindowImpl::TakeFocus(bool reverse) {
}
void WindowImpl::HandleKeyboardEvent(const NativeWebKeyboardEvent& event) {
    // Useless: just calls this when we hand it an input event.
}
void WindowImpl::HandleMouseEvent() {
    // Useless: just calls this when we hand it an input event.
}
void WindowImpl::HandleMouseLeave() {
    // Useless: just calls this when we hand it an input event.
}

void WindowImpl::ShowPopupMenu(const gfx::Rect&, int, double, int, const std::vector<WebMenuItem>&, bool) {
    // FIXME(patrick): We need to expose this to the public API!
}

void WindowImpl::LostCapture() {
}

void WindowImpl::HandleMouseUp() {
}

void WindowImpl::HandleMouseActivate() {
}

void WindowImpl::CreateNewFullscreenWidget(int route_id) {
    CreateNewWidget(route_id, WebKit::WebPopupTypeNone);
}
void WindowImpl::ShowCreatedFullscreenWidget(int route_id) {
    ShowCreatedFullscreenWidget(route_id);
}
void WindowImpl::Activate() {
}
void WindowImpl::Deactivate() {
}
void WindowImpl::HandleMouseMove() {
}
void WindowImpl::HandleMouseDown() {
}
void WindowImpl::UpdatePreferredWidth(int pref_width) {
}

void WindowImpl::UpdatePreferredSize(const gfx::Size&) {
}

// Callback to give the browser a chance to handle the specified keyboard
// event before sending it to the renderer.
// Returns true if the |event| was handled. Otherwise, if the |event| would
// be handled in HandleKeyboardEvent() method as a normal keyboard shortcut,
// |*is_keyboard_shortcut| should be set to true.
bool WindowImpl::PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event, bool* is_keyboard_shortcut) {
    // FIXME(patrick): Is keyboard shortcut might be useful to ask application...
    return false;
}

int WindowImpl::getId() const {
  return mId;
}

}

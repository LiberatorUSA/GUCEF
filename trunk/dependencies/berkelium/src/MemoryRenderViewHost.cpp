/*  Berkelium Implementation
 *  MemoryRenderViewHost.cpp
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

#include "berkelium/Platform.hpp"
#include "WindowImpl.hpp"
#include "berkelium/Window.hpp"
#include "RenderWidget.hpp"
#include "MemoryRenderViewHost.hpp"
#include <stdio.h>

#include "content/browser/renderer_host/render_widget_host_view.h"
#include "content/browser/renderer_host/render_process_host.h"
#include "chrome/common/notification_service.h"
#include "chrome/common/render_messages.h"
#include "chrome/common/render_messages_params.h"

namespace Berkelium {

///////// MemoryRenderViewHost /////////

MemoryRenderViewHost::MemoryRenderViewHost(
        SiteInstance* instance,
        RenderViewHostDelegate* delegate,
        int routing_id,
        SessionStorageNamespace *ssn_id) // session_storage_namespace_id
    : MemoryRenderHostImpl<RenderViewHost>(instance, delegate, routing_id, ssn_id) {

    mWindow = static_cast<WindowImpl*>(delegate);
    mWidget = NULL;
}

MemoryRenderViewHost::~MemoryRenderViewHost() {
}

bool MemoryRenderViewHost::OnMessageReceived(const IPC::Message& msg) {
  bool msg_is_ok = true;
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_EX(MemoryRenderViewHost, msg, msg_is_ok)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateRect, Memory_OnMsgUpdateRect)
    IPC_MESSAGE_HANDLER(ViewHostMsg_AddMessageToConsole, Memory_OnAddMessageToConsole)
    IPC_MESSAGE_UNHANDLED(handled = RenderViewHost::OnMessageReceived(msg))
  IPC_END_MESSAGE_MAP_EX()
      ;

  if (!msg_is_ok) {
    // The message had a handler, but its de-serialization failed.
    // Kill the renderer.
    process()->ReceivedBadMessage();
  }
  return handled;
}

void MemoryRenderViewHost::Memory_OnAddMessageToConsole(const std::wstring& message,
                                           int32 line_no,
                                           const std::wstring& source_id) {
    mWindow->OnAddMessageToConsole(message, line_no, source_id);
}


///////// MemoryRenderWidgetHost /////////

MemoryRenderWidgetHost::MemoryRenderWidgetHost(
        RenderViewHostDelegate *delegate,
        RenderWidgetHostView *wid,
        RenderProcessHost* host,
        int routing_id)
    : MemoryRenderHostImpl<RenderWidgetHost>(host, routing_id) {

    mWindow = static_cast<WindowImpl*>(delegate);
    mWidget = static_cast<RenderWidget*>(wid);

    set_view(wid);
}

MemoryRenderWidgetHost::~MemoryRenderWidgetHost() {
}

bool MemoryRenderWidgetHost::OnMessageReceived(const IPC::Message& msg) {
  bool msg_is_ok = true;
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_EX(MemoryRenderWidgetHost, msg, msg_is_ok)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateRect, Memory_OnMsgUpdateRect)
    IPC_MESSAGE_UNHANDLED(handled = this->RenderWidgetHost::OnMessageReceived(msg))
  IPC_END_MESSAGE_MAP_EX()
      ;

  if (!msg_is_ok) {
    // The message had a handler, but its de-serialization failed.
    // Kill the renderer.
    process()->ReceivedBadMessage();
  }
  return handled;
}




///////// MemoryRenderHostImpl common functions /////////

void MemoryRenderWidgetHost::Memory_OnMsgUpdateRect(const ViewHostMsg_UpdateRect_Params&params){
    this->MemoryRenderHostImpl<RenderWidgetHost>::Memory_OnMsgUpdateRect(params);
}

template <class T> void MemoryRenderHostImpl<T>::init() {
    mResizeAckPending=true;
    mWidget=NULL;
    
}
template <class T> void MemoryRenderHostImpl<T>::Memory_WasResized() {
    if (this->mResizeAckPending || !this->process()->HasConnection() || !this->view() || !this->renderer_initialized_) {
        return;
    }
    
    gfx::Rect view_bounds = this->view()->GetViewBounds();
    gfx::Size new_size(view_bounds.width(), view_bounds.height());
    
    // Avoid asking the RenderWidget to resize to its current size, since it
    // won't send us a PaintRect message in that case.
    if (new_size == current_size_ || current_size_== gfx::Size())
        return;
    
    if (mInFlightSize != gfx::Size())
        return;
    
    // We don't expect to receive an ACK when the requested size is empty.
    if (!new_size.IsEmpty())
        mResizeAckPending = true;
    
    if (!this->process()->Send(new ViewMsg_Resize(this->routing_id(), new_size,
                                                  this->view()->reserved_contents_rect())))
        mResizeAckPending = false;
    else
        mInFlightSize = new_size;
}
template <class T> void MemoryRenderHostImpl<T>::Memory_OnMsgUpdateRect(
    const ViewHostMsg_UpdateRect_Params&params)
{
  current_size_ = params.view_size;

    DCHECK(!params.bitmap_rect.IsEmpty());
    DCHECK(!params.view_size.IsEmpty());

    bool is_resize_ack =
        ViewHostMsg_UpdateRect_Flags::is_resize_ack(params.flags);

    // resize_ack_pending_ needs to be cleared before we call DidPaintRect, since
    // that will end up reaching GetBackingStore.
    if (is_resize_ack) {
        assert(mResizeAckPending);
        mResizeAckPending=false;
        mInFlightSize.SetSize(0,0);
        //PRIV//DCHECK(resize_ack_pending_);
        //PRIV//resize_ack_pending_ = false;
        //PRIV//in_flight_size_.SetSize(0, 0);
    }


  DCHECK(!params.bitmap_rect.IsEmpty());
  DCHECK(!params.view_size.IsEmpty());

  const size_t size = params.bitmap_rect.height() *
                      params.bitmap_rect.width() * 4;
  TransportDIB* dib = this->process()->GetTransportDIB(params.bitmap);
  if (dib) {
    if (dib->size() < size) {
      DLOG(WARNING) << "Transport DIB too small for given rectangle";
      this->process()->ReceivedBadMessage();
    } else {
      // Paint the backing store. This will update it with the renderer-supplied
      // bits. The view will read out of the backing store later to actually
      // draw to the screen.
      Memory_PaintBackingStoreRect(dib, params.bitmap_rect,
        params.copy_rects, params.view_size,
        params.dx, params.dy, params.scroll_rect);
      // FIXME: Difference between scroll_rect + params.view_size and clip_rect
    }
  }

  // ACK early so we can prefetch the next PaintRect if there is a next one.
  // This must be done AFTER we're done painting with the bitmap supplied by the
  // renderer. This ACK is a signal to the renderer that the backing store can
  // be re-used, so the bitmap may be invalid after this call.
  this->process()->Send(new ViewMsg_UpdateRect_ACK(this->routing_id()));

  // Now paint the view. Watch out: it might be destroyed already.
  if (this->view()) {
    this->view()->MovePluginWindows(params.plugin_window_moves);
    //PRIV//view_being_painted_ = true;
    this->view()->DidUpdateBackingStore(params.scroll_rect,
                                        params.dx, params.dy,
                                        params.copy_rects);
    //PRIV//view_being_painted_ = false;
  }

  // If we got a resize ack, then perhaps we have another resize to send?
  if (is_resize_ack && this->view()) {
    gfx::Rect view_bounds = this->view()->GetViewBounds();
    if (current_size_.width() != view_bounds.width() ||
        current_size_.height() != view_bounds.height()) {
      Memory_WasResized();
    }
  }

}

template <class T> void MemoryRenderHostImpl<T>::Memory_PaintBackingStoreRect(
    TransportDIB* bitmap,
    const gfx::Rect& bitmap_rect,
    const std::vector<gfx::Rect>& copy_rects,
    const gfx::Size& view_size,
    int dx, int dy,
    const gfx::Rect& clip_rect)
{
    Rect updateRect;
    updateRect.setFromRect(bitmap_rect);

    Rect clipRect;
    clipRect.setFromRect(clip_rect);

    size_t numCopyRects = copy_rects.size();
    Rect * copyRects = new Rect [numCopyRects];

    for (size_t i = 0; i < numCopyRects; ++i) {
        copyRects[i].setFromRect(copy_rects[i]);
    }

    mWindow->onPaint(
        mWidget,
        static_cast<const unsigned char *>(bitmap->memory()),
        updateRect,
        numCopyRects,
        copyRects,
        dx,
        dy,
        clipRect);

}

/*
gfx::Rect MemoryRenderViewHost::RootWindowResizerRectSize()const{
    return GetRootWindowResizerRect();
}

RenderProcessHost* MemoryRenderViewHost::process() {
    return RenderViewHost::process();
}

int MemoryRenderViewHost::routing_id()const{
    return RenderViewHost::routing_id();
}

RenderProcessHost* MemoryRenderWidgetHost::process() {
    return RenderWidgetHost::process();
}

int MemoryRenderWidgetHost::routing_id()const{
    return RenderWidgetHost::routing_id();
}

*/
///////// MemoryRenderViewHostFactory /////////

MemoryRenderViewHostFactory::MemoryRenderViewHostFactory() {
    RenderViewHostFactory::RegisterFactory(this);
}

MemoryRenderViewHostFactory::~MemoryRenderViewHostFactory() {
    RenderViewHostFactory::UnregisterFactory();
}

RenderViewHost* MemoryRenderViewHostFactory::CreateRenderViewHost(
    SiteInstance* instance,
    RenderViewHostDelegate* delegate,
    int routing_id,
    SessionStorageNamespace *ssn_id)
{
    return new MemoryRenderViewHost(instance, delegate,
                                    routing_id, ssn_id);
}


template class MemoryRenderHostImpl<RenderWidgetHost>;
template class MemoryRenderHostImpl<RenderViewHost>;

}


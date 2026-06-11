#include "core/foreign_toplevels.hpp"

#include <algorithm>
#include <wayland-client.h>

#include "ext-foreign-toplevel-list-v1-client-protocol.h"

namespace hs::core {

ExtForeignToplevels::~ExtForeignToplevels() { shutdown(); }

void ExtForeignToplevels::bind(ext_foreign_toplevel_list_v1* list, wl_display* display)
{
  if (!list || list_ == list) return;
  list_ = list;
  display_ = display;

  static const ext_foreign_toplevel_list_v1_listener kListListener = {
      .toplevel = on_toplevel,
      .finished = on_finished,
  };
  ext_foreign_toplevel_list_v1_add_listener(list_, &kListListener, this);
}

void ExtForeignToplevels::shutdown()
{
  if (display_ && wl_display_get_error(display_)) {
    display_ = nullptr;
    for (auto& tl : toplevels_) tl.handle = nullptr;
    toplevels_.clear();
    list_ = nullptr;
    initialSyncDone_ = false;
    return;
  }
  for (auto& tl : toplevels_) {
    if (tl.handle) {
      ext_foreign_toplevel_handle_v1_destroy(tl.handle);
      tl.handle = nullptr;
    }
  }
  toplevels_.clear();
  if (list_) {
    ext_foreign_toplevel_list_v1_destroy(list_);
    list_ = nullptr;
  }
  display_ = nullptr;
  initialSyncDone_ = false;
}

void ExtForeignToplevels::on_toplevel(void* data, ext_foreign_toplevel_list_v1*,
                                       ext_foreign_toplevel_handle_v1* handle)
{
  auto& self = *static_cast<ExtForeignToplevels*>(data);
  Toplevel tl;
  tl.handle = handle;
  self.toplevels_.push_back(std::move(tl));

  static const ext_foreign_toplevel_handle_v1_listener kHandleListener = {
      .closed = on_closed,
      .done = on_done,
      .title = on_title,
      .app_id = on_app_id,
      .identifier = on_identifier,
  };
  ext_foreign_toplevel_handle_v1_add_listener(handle, &kHandleListener, &self);
}

void ExtForeignToplevels::on_finished(void* data, ext_foreign_toplevel_list_v1* list)
{
  auto& self = *static_cast<ExtForeignToplevels*>(data);
  (void)list;
  if (self.display_ && !self.initialSyncDone_) {
    (void)wl_display_roundtrip(self.display_);
    self.initialSyncDone_ = true;
  }
}

void ExtForeignToplevels::on_closed(void* data, ext_foreign_toplevel_handle_v1* handle)
{
  auto& self = *static_cast<ExtForeignToplevels*>(data);
  auto it = std::remove_if(self.toplevels_.begin(), self.toplevels_.end(),
                            [handle](const Toplevel& t) { return t.handle == handle; });
  if (it != self.toplevels_.end()) {
    self.toplevels_.erase(it, self.toplevels_.end());
  }
  if (handle) {
    if (self.display_ && wl_display_get_error(self.display_)) return;
    ext_foreign_toplevel_handle_v1_destroy(handle);
  }
}

void ExtForeignToplevels::on_done(void*, ext_foreign_toplevel_handle_v1*) {}

void ExtForeignToplevels::on_title(void* data, ext_foreign_toplevel_handle_v1* handle, const char* title)
{
  auto& self = *static_cast<ExtForeignToplevels*>(data);
  for (auto& tl : self.toplevels_) {
    if (tl.handle == handle) { tl.title = title ? title : ""; return; }
  }
}

void ExtForeignToplevels::on_app_id(void* data, ext_foreign_toplevel_handle_v1* handle, const char* app_id)
{
  auto& self = *static_cast<ExtForeignToplevels*>(data);
  for (auto& tl : self.toplevels_) {
    if (tl.handle == handle) { tl.appId = app_id ? app_id : ""; return; }
  }
}

void ExtForeignToplevels::on_identifier(void* data, ext_foreign_toplevel_handle_v1* handle, const char* identifier)
{
  auto& self = *static_cast<ExtForeignToplevels*>(data);
  for (auto& tl : self.toplevels_) {
    if (tl.handle == handle) { tl.identifier = identifier ? identifier : ""; return; }
  }
}

}

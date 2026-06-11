#include "core/icon_cache.hpp"

namespace hs::icons {

IconCache::~IconCache() {
  for (auto& [_, entry] : cache_) {
    if (entry && entry->surface) {
      cairo_surface_destroy(entry->surface);
    }
  }
}

const IconEntry* IconCache::app_icon(const std::string& app_id)
{
  auto it = cache_.find(app_id);
  if (it != cache_.end()) {
    return it->second.get();
  }
  return nullptr;
}

}

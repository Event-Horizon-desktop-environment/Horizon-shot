#pragma once

#include <cairo/cairo.h>
#include <string>
#include <unordered_map>
#include <memory>

namespace hs::icons {

struct IconEntry {
  cairo_surface_t* surface = nullptr;
  int width = 0;
  int height = 0;
  size_t bytes = 0;
};

class IconCache {
public:
  IconCache() = default;
  ~IconCache();

  const IconEntry* app_icon(const std::string& app_id);

private:
  std::unordered_map<std::string, std::unique_ptr<IconEntry>> cache_;
};

}

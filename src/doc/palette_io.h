// Aseprite Document Library
// Copyright (c) 2001-2015 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include <iosfwd>
#include <memory>

namespace doc {

  class Palette;

  void write_palette(std::ostream& os, const Palette& palette);
  std::shared_ptr<Palette> read_palette(std::istream& is);

} // namespace doc

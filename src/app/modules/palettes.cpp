// Aseprite
// Copyright (C) 2001-2015  David Capello
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/modules/palettes.h"

#include "app/app.h"
#include "app/file/palette_file.h"
#include "app/resource_finder.h"
#include "base/fs.h"
#include "base/path.h"
#include "doc/image.h"
#include "doc/palette.h"
#include "doc/sprite.h"

#include <cstring>

namespace app {

// The default color palette.
  static std::shared_ptr<Palette> ase_default_palette;

// Palette in current sprite frame.
  static std::shared_ptr<Palette> ase_current_palette;

int init_module_palette()
{
  ase_default_palette = Palette::create(256);
  ase_current_palette = Palette::create(256);
  return 0;
}

void exit_module_palette()
{
}

void load_default_palette(const std::string& userDefined)
{
  std::shared_ptr<Palette> pal;

  // Load specific palette file defined by the user in the command line.
  std::string palFile = userDefined;
  if (!palFile.empty())
    pal = load_palette(palFile.c_str());
  // Load default palette file
  else {
    std::string defaultPalName = get_preset_palette_filename(
      get_default_palette_preset_name(), ".ase");

    // If there is no palette in command line, we use the default one.
    palFile = defaultPalName;
    if (base::is_file(palFile)) {
      pal = load_palette(palFile.c_str());
    }
    else {
      // Migrate old default.gpl to default.ase format
      palFile = get_preset_palette_filename(
        get_default_palette_preset_name(), ".gpl");

      if (base::is_file(palFile)) {
        pal = load_palette(palFile.c_str());

        // Remove duplicate black entries at the end (as old palettes
        // contains 256 colors)
        if (pal && pal->size() == 256) {
          doc::color_t black = rgba(0, 0, 0, 255);

          // Get the last non-black entry
          int i = 0;
          for (i=pal->size()-1; i>0; --i) {
            if (pal->getEntry(i) != black)
              break;
          }

          if (i < pal->size()-1) {
            // Check if there is a black entry in the first entries.
            bool hasBlack = false;
            for (int j=0; j<i; ++j) {
              if (pal->getEntry(j) == black) {
                hasBlack = true;
                break;
              }
            }
            if (!hasBlack)
              ++i;                // Leave one black entry

            // Resize the palette
            if (i < pal->size()-1)
              pal->resize(i+1);
          }
        }

        // We could remove the old .gpl file (palFile), but as the
        // user could be using multiple versions of Aseprite, it's a
        // good idea to keep both formats (.gpl for Aseprite <=
        // v1.1-beta5, and .ase for future versions).
      }
      // If the default palette file doesn't exist, we copy db32.gpl
      // as the default one (default.ase).
      else {
        ResourceFinder rf;
        rf.includeDataDir("palettes/db32.gpl");
        if (rf.findFirst()) {
          pal = load_palette(rf.filename().c_str());
        }
      }

      // Save default.ase file
      if (pal) {
        palFile = defaultPalName;
        save_palette(palFile.c_str(), *pal, 0);
      }
    }
  }

  if (pal)
    set_default_palette(pal.get());

  set_current_palette(nullptr, true);
}

Palette* get_current_palette()
{
  return ase_current_palette.get();
}

Palette* get_default_palette()
{
  return ase_default_palette.get();
}

void set_default_palette(const Palette* palette)
{
  palette->copyColorsTo(*ase_default_palette);
}

// Changes the current system palette and triggers the
// App::PaletteChange signal.
//
// If "_palette" is nullptr the default palette is set.
bool set_current_palette(const Palette *_palette, bool forced)
{
  const Palette* palette = (_palette ? _palette: ase_default_palette.get());
  bool ret = false;

  // Have changes
  if (forced || palette->countDiff(*ase_current_palette, NULL, NULL) > 0) {
    // Copy current palette
    palette->copyColorsTo(*ase_current_palette);

    // Call slots in signals
    App::instance()->PaletteChange();

    ret = true;
  }

  return ret;
}

std::string get_preset_palette_filename(const std::string& preset,
                                        const std::string& dot_extension)
{
  ResourceFinder rf;
  rf.includeUserDir(base::join_path("palettes", ".").c_str());
  std::string palettesDir = rf.getFirstOrCreateDefault();

  if (!base::is_directory(palettesDir))
    base::make_directory(palettesDir);

  return base::join_path(palettesDir, preset + dot_extension);
}

std::string get_default_palette_preset_name()
{
  return "default";
}

} // namespace app

// Aseprite
// Copyright (C) 2001-2016  David Capello
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/app.h"
#include "app/app_options.h"
#include "app/console.h"
#include "app/resource_finder.h"
#include "app/send_crash.h"
#include "base/exception.h"
#include "base/memory.h"
#include "base/memory_dump.h"
#include "base/system_console.h"
#include "she/error.h"
#include "she/scoped_handle.h"
#include "she/system.h"

#include <clocale>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef EMSCRIPTEN
#define LIFETIME static
#else
#define LIFETIME
#endif

namespace {

  // Memory leak detector wrapper
  class MemLeak {
  public:
#ifdef MEMLEAK
    MemLeak() { base_memleak_init(); }
    ~MemLeak() { base_memleak_exit(); }
#else
    MemLeak() { }
#endif
  };

}

// Aseprite entry point. (Called from she library.)
int app_main(int argc, char* argv[])
{
  // Initialize the locale. Aseprite isn't ready to handle numeric
  // fields with other locales (e.g. we expect strings like "10.32" be
  // used in std::strtod(), not something like "10,32").
  std::setlocale(LC_ALL, "en-US");
  ASSERT(std::strtod("10.32", nullptr) == 10.32);

  // Initialize the random seed.
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

#ifdef _WIN32
  ::CoInitialize(NULL);
#endif

  try {
    static app::AppOptions options(argc, const_cast<const char**>(argv));
    LIFETIME auto system = std::unique_ptr<she::System>(she::create_system());
    return system->run([]{
      try {
	base::MemoryDump memoryDump; //
	MemLeak memleak;
	base::SystemConsole systemConsole;
	app::App app;

	// Change the name of the memory dump file
	{
	  std::string filename = app::memory_dump_filename();
	  if (!filename.empty())
	    memoryDump.setFileName(filename);
	}

	app.initialize(options);

	if (options.startShell())
	  systemConsole.prepareShell();

	app.run();
	return 0;
      } catch (std::exception& e) {
	std::cerr << e.what() << '\n';
	she::error_message(e.what());
	return 1;
      }
    });
  }
  catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    she::error_message(e.what());
    return 1;
  }
}

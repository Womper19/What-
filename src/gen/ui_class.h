// Aseprite Code Generator
// Copyright (c) 2014 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include <string>
#include "tinyxml2.h"

void gen_ui_class(tinyxml2::XMLDocument* doc, const std::string& inputFn, const std::string& widgetId);

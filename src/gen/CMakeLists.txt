# Aseprite Code Generator
# Copyright (C) 2014, 2015  David Capello

add_executable(gen
  gen.cpp
  pref_types.cpp
  skin_class.cpp
  ui_class.cpp)

if(MSVC)
  # Fix problem compiling gen from a Visual Studio solution
  set_target_properties(gen
    PROPERTIES LINK_FLAGS -ENTRY:"mainCRTStartup")
endif()

target_link_libraries(gen
  base-lib
  ${TINYXML2_LIBRARY})

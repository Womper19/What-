# Aseprite Network Library
# Copyright (C) 2001-2015 David Capello

add_library(net-lib
  http_request.cpp
  http_response.cpp)

target_link_libraries(net-lib
  ${CURL_LIBRARIES})

# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\foodappproject_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\foodappproject_autogen.dir\\ParseCache.txt"
  "foodappproject_autogen"
  )
endif()

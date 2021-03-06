cmake_minimum_required(VERSION 3.0)
project(ExampleComponentLibrary)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_DEBUG_POSTFIX _d)
if(WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

set(targetname examplecomponentlibrary)

file(GLOB_RECURSE component_files *.h *.hpp *.cci)

add_library(${targetname} SHARED exampleComponentLib.cpp ${component_files})
target_include_directories(${targetname} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_link_libraries(${targetname} hopsancore)

install(TARGETS ${targetname}
  RUNTIME DESTINATION componentLibraries/exampleComponentLib
  LIBRARY DESTINATION componentLibraries/exampleComponentLib
)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/ DESTINATION componentLibraries/exampleComponentLib
  FILES_MATCHING
  PATTERN "*.cpp"
  PATTERN "*.h *"
  PATTERN "*.hpp"
  PATTERN "*.cci"
  PATTERN "*.xml"
  PATTERN "*.nb"
  PATTERN "*.py"
  PATTERN "*.bat"
  PATTERN "*.sh"
  PATTERN "*.csv"
  PATTERN "*.txt"
  PATTERN "*.md"
  PATTERN "*.hmf"
  PATTERN "LICENSE")

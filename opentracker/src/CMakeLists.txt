FILE(GLOB_RECURSE SOURCES
  common/*.cxx
  core/*.cxx
  input/*.cxx
  misc/*.cxx
  network/*.cxx
  otpy/*.cxx
  #otqt/*.cxx
  tool/*.cxx
  ubitrack/*.cxx
)

# a few files are making troubles and are excluded for the moment:
LIST(REMOVE_ITEM SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/network/CORBAModule.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/network/CORBASink.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/network/CORBAUtils.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/tool/ToolAppWin.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/tool/ToolIOModule.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/tool/tool.cxx
)

SET(SOURCES ${SOURCES} ../extras/intersense/isense.c)

ADD_SUBDIRECTORY(standalones)
INCLUDE_DIRECTORIES(../extras/intersense)
ADD_LIBRARY(opentracker SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(opentracker ${ACE_LIBRARIES} ${TINY_XML_MOD_LIBRARY} ${CURSES_LIBRARIES} xerces-c)
INSTALL(TARGETS opentracker DESTINATION ${CMAKE_INSTALL_LIBDIR})
SET_TARGET_PROPERTIES(opentracker PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})

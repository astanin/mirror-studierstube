# - Try to find the opentracker library
# Once done this will define
#
#  OPENTRACKER_FOUND - system has opentracker
#  OPENTRACKER_INCLUDE_DIR - the opentracker include directory
#  OPENTRACKER_LIBRARIES - Link these to use opentracker
#  OPENTRACKER_LINK_DIRECTORIES - link directories, useful for rpath
#  OPENTRACKER_DEFINITIONS - Compiler switches required for using opentracker
#

if (OPENTRACKER_INCLUDE_DIR AND OPENTRACKER_LIBRARIES)

  # in cache already
  SET(OPENTRACKER_FOUND TRUE)

else (OPENTRACKER_INCLUDE_DIR AND OPENTRACKER_LIBRARIES)

  if (WIN32)
    
    # check whether the OPENTRACKERDIR environment variable is set and points to a 
    # valid windows OPENTRACKER installation
    FIND_PATH(
	OPENTRACKER_DIR include/OpenTracker/OpenTracker.h
	PATHS $ENV{OPENTRACKERROOT}/ "C:/Program Files/OpenTracker/"
      DOC "The main directory of the OPENTRACKER library, containing the subfolders include and lib" )
    
    if (OPENTRACKER_DIR)
      SET(OPENTRACKER_INCLUDE_DIR ${OPENTRACKER_DIR}/include CACHE PATH "OPENTRACKER include directory")
      SET(OPENTRACKER_LINK_DIRECTORIES ${OPENTRACKER_DIR}/lib CACHE PATH "OPENTRACKER link directory")
      SET(OPENTRACKER_LIBRARIES OPENTRACKER CACHE STRING "OPENTRACKER library name")
      SET(OPENTRACKER_FOUND TRUE)
    else (OPENTRACKER_DIR)
      SET (OPENTRACKER_FOUND FALSE)
    endif (OPENTRACKER_DIR)

  else (WIN32)

    FIND_PATH(OPENTRACKER_INCLUDE_DIR OpenTracker/OpenTracker.h
      /usr/include)

    FIND_LIBRARY(OPENTRACKER_LIBRARIES NAMES opentracker PATHS
      /usr/lib
      /usr/lib64)
    
    if(OPENTRACKER_INCLUDE_DIR AND OPENTRACKER_LIBRARIES)
      set(OPENTRACKER_FOUND TRUE)
    endif(OPENTRACKER_INCLUDE_DIR AND OPENTRACKER_LIBRARIES)

  endif (WIN32)
endif (OPENTRACKER_INCLUDE_DIR AND OPENTRACKER_LIBRARIES)
  
if (OPENTRACKER_FOUND)
  set (OPENTRACKER_DEFINITIONS -DUSE_OT_2_0)
  if (NOT OpenTracker_FIND_QUIETLY)
    message(STATUS "Found OPENTRACKER: ${OPENTRACKER_LIBRARIES}")
  endif (NOT OpenTracker_FIND_QUIETLY)
else (OPENTRACKER_FOUND)
  if (OpenTracker_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find OPENTRACKER")
  endif (OpenTracker_FIND_REQUIRED)
endif (OPENTRACKER_FOUND)

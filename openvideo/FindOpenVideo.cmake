# - Try to find the openvideo library
# Once done this will define
#
#  OPENVIDEO_FOUND - system has OPENVIDEO
#  OPENVIDEO_INCLUDE_DIR - the OPENVIDEO include directory
#  OPENVIDEO_LIBRARIES - Link these to use OPENVIDEO
#  OPENVIDEO_LINK_DIRECTORIES - link directories, useful for rpath
#

if (OPENVIDEO_INCLUDE_DIR AND OPENVIDEO_LIBRARIES)

  # in cache already
  SET(OPENVIDEO_FOUND TRUE)

else (OPENVIDEO_INCLUDE_DIR AND OPENVIDEO_LIBRARIES)

  if (WIN32)
    
    # check whether the OPENVIDEODIR environment variable is set and points to a 
    # valid windows OPENVIDEO installation
    FIND_PATH(
	OPENVIDEO_DIR include/openvideo/configOV.template.h
	PATHS $ENV{OPENVIDEOROOT}/ "C:/Program Files/openvideo/"
      DOC "The main directory of the OPENVIDEO library, containing the subfolders include and lib" )
    
    if (OPENVIDEO_DIR)
      SET(OPENVIDEO_INCLUDE_DIR ${OPENVIDEO_DIR}/include CACHE PATH "OPENVIDEO include directory")
      SET(OPENVIDEO_LINK_DIRECTORIES ${OPENVIDEO_DIR}/lib CACHE PATH "OPENVIDEO link directory")
      SET(OPENVIDEO_LIBRARIES OPENVIDEO CACHE STRING "OPENVIDEO library name")
      SET(OPENVIDEO_FOUND TRUE)
    else (OPENVIDEO_DIR)
      SET (OPENVIDEO_FOUND FALSE)
    endif (OPENVIDEO_DIR)

  else (WIN32)

    FIND_PATH(OPENVIDEO_INCLUDE_DIR openvideo/configOV.template.h
      /usr/include)

    FIND_LIBRARY(OPENVIDEO_LIBRARIES NAMES openvideo PATHS
      /usr/lib
      /usr/lib64)
    
    if(OPENVIDEO_INCLUDE_DIR AND OPENVIDEO_LIBRARIES)
      set(OPENVIDEO_FOUND TRUE)
    endif(OPENVIDEO_INCLUDE_DIR AND OPENVIDEO_LIBRARIES)

  endif (WIN32)
  
  if (OPENVIDEO_FOUND)
    if (NOT OpenVideo_FIND_QUIETLY)
      message(STATUS "Found OPENVIDEO: ${OPENVIDEO_LIBRARIES}")
    endif (NOT OpenVideo_FIND_QUIETLY)
  else (OPENVIDEO_FOUND)
    if (OpenVideo_FIND_REQUIRED)
      message(FATAL_ERROR "Could NOT find OPENVIDEO")
    endif (OpenVideo_FIND_REQUIRED)
  endif (OPENVIDEO_FOUND)

  
endif (OPENVIDEO_INCLUDE_DIR AND OPENVIDEO_LIBRARIES)

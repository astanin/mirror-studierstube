FILE(GLOB stbstarlight_SOURCES *.cpp *.cxx)
INCLUDE_DIRECTORIES(../../../include)

# Coin:
INCLUDE_DIRECTORIES(${COIN_INCLUDE_DIR})
ADD_DEFINITIONS(${COIN_DEFINITIONS})
LINK_DIRECTORIES(${COIN_LINK_DIRECTORIES})

# ACE:
INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIR})
ADD_DEFINITIONS(${ACE_DEFINITIONS})
LINK_DIRECTORIES(${ACE_LINK_DIRECTORIES})

ADD_LIBRARY(stbstarlight SHARED ${stbstarlight_SOURCES})
TARGET_LINK_LIBRARIES(stbstarlight ${COIN_LIBRARIES} ${ACE_LIBRARIES})

SET_TARGET_PROPERTIES(stbstarlight PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})
INSTALL(TARGETS stbstarlight DESTINATION ${CMAKE_INSTALL_LIBDIR})

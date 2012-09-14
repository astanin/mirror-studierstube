PROJECT(studierstube)

INCLUDE(UsePkgConfig)

FIND_PACKAGE(OpenTracker)
FIND_PACKAGE(OpenVideo)
FIND_PACKAGE(ACE REQUIRED)
FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(Coin REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(SoQt REQUIRED)
#FIND_PACKAGE(Xerces REQUIRED)

FIND_PACKAGE(TinyXMLMod)
IF(NOT TINY_XML_MOD_FOUND)
  FIND_PACKAGE(TinyXML)
  IF(NOT TINY_XML_FOUND)
    MESSAGE(FATAL_ERROR "neither TinyXML nor TinyXML_Mod could be found")
  ENDIF(NOT TINY_XML_FOUND)
ELSE(NOT TINY_XML_MOD_FOUND)
  ADD_DEFINITIONS(-DUSE_TINYXMLMOD)
  SET(TINY_XML_INCLUDE_DIR ${TINY_XML_MOD_INCLUDE_DIR})
  SET(TINY_XML_LIBRARY_DIR ${TINY_XML_MOD_LIBRARY_DIR})
  SET(TINY_XML_LIBRARY ${TINY_XML_MOD_LIBRARY})
ENDIF(NOT TINY_XML_MOD_FOUND)

SET(CPACK_GENERATOR "TBZ2")
SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_PACKAGE_VERSION_MAJOR "4")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR})
SET(CPACK_PACKAGE_NAME "studierstube")

# RPM variables:
IF(NOT CMAKE_INSTALL_BINDIR)
SET(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF(NOT CMAKE_INSTALL_BINDIR)
IF(NOT CMAKE_INSTALL_LIBDIR)
SET(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF(NOT CMAKE_INSTALL_LIBDIR)
IF(NOT CMAKE_INSTALL_DATADIR)
SET(CMAKE_INSTALL_DATADIR ${CMAKE_INSTALL_PREFIX}/share)
ENDIF(NOT CMAKE_INSTALL_DATADIR)
IF(NOT CMAKE_INSTALL_DOCDIR)
SET(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATADIR}/doc/packages)
ENDIF(NOT CMAKE_INSTALL_DOCDIR)
IF(NOT CMAKE_INSTALL_MANDIR)
SET(CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_DATADIR}/man)
ENDIF(NOT CMAKE_INSTALL_MANDIR)

# configure stb4.pc:
SET(STB4_PREFIX ${CMAKE_INSTALL_PREFIX})
SET(STB4_LIBDIR ${CMAKE_INSTALL_LIBDIR})
SET(STB4_PROJECT_NAME ${CPACK_PACKAGE_NAME})
SET(STB4_PROJECT_VERSION ${CPACK_PACKAGE_VERSION})
SET(STB4_PROJECT_DESCRIPTION "Studierstube - The Augmented Reality Environment")
SET(STB4_PROJECT_LIBNAME ${CPACK_PACKAGE_NAME})
SET(STB4_PROJECT_DEFINES "")
CONFIGURE_FILE(stb4.pc.in stb4.pc @ONLY)
INSTALL(FILES stb4.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

CONFIGURE_FILE(studierstube.spec.in studierstube.spec)

# testing using CTest/Dart:
INCLUDE(CTest)

ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(doc)

ADD_DEFINITIONS(${OPENTRACKER_DEFINITIONS})
INCLUDE_DIRECTORIES(${OPENTRACKER_INCLUDE_DIR} ${OPENVIDEO_INCLUDE_DIR})

INSTALL(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN ".svn" EXCLUDE)

# packaging:

SET(CPACK_IGNORE_FILES
  ".depend"
  "/#.*#$"
  "/debian"
  "/doc/doc/"
  "/files.stb"
  "/log"
  "/rpm.stb"
  "/trunk/build/"
  "CMakeCache\\\\.txt"
  "CMakeFiles"
  "CPack.*Config\\\\.cmake"
  "CVS"
  "Makefile"
  "SCons*"
  "\\\\.#"
  "\\\\.a$"
  "\\\\.aux$"
  "\\\\.bz2$"
  "\\\\.dsp$"
  "\\\\.dvi$"
  "\\\\.eps$"
  "\\\\.gz$"
  "\\\\.iv$"
  "\\\\.log$"
  "\\\\.o$"
  "\\\\.out"
  "\\\\.pgm$"
  "\\\\.pro$"
  "\\\\.ps$"
  "\\\\.save"
  "\\\\.so"
  "\\\\.svn"
  "\\\\.swp$"
  "\\\\.tbz$"
  "\\\\.tex$"
  "\\\\.tgz$"
  "\\\\.vcproj$"
  "^/box.cpp"
  "_CPack_Packages"
  "a.out"
  "cgparams$"
  "cmake_install.cmake"
  "cvs_import"
  "install.sh"
  "install_manifest.txt"
  "makedist.sh"
  "moc_"
  "progress.make"
  "~$"
)

SET(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
INCLUDE(CPack)
ADD_CUSTOM_TARGET(dist COMMAND make clean && make package_source)
ADD_CUSTOM_TARGET(clean-all COMMAND find \\\( -name CMakeFiles -or -name CMakeCache.txt -or -name Makefile -or -name DartTestfile.txt -or -name 'CPack*.cmake' -or -name _CPack_Packages -or -name '*~' \\\) -prune -exec rm -rf {} '\;')

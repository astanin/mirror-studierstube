PROJECT(opentracker)

INCLUDE(UsePkgConfig)

#FIND_PACKAGE(OpenGL REQUIRED)
#FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(ACE REQUIRED)
FIND_PACKAGE(TinyXMLMod REQUIRED)
#FIND_PACKAGE(AVIFile64 REQUIRED)

IF(UNIX)
  FIND_PACKAGE(Curses)
  ADD_DEFINITIONS(-DLINUX)
ENDIF(UNIX)

SET(CPACK_GENERATOR "TBZ2")
SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_PACKAGE_VERSION_MAJOR "2")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
SET(CPACK_PACKAGE_NAME "opentracker")

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

# configure opentracker.pc:
SET(OPENTRACKER_PREFIX ${CMAKE_INSTALL_PREFIX})
SET(OPENTRACKER_LIBDIR ${CMAKE_INSTALL_LIBDIR})
SET(OPENTRACKER_PROJECT_NAME ${CPACK_PACKAGE_NAME})
SET(OPENTRACKER_PROJECT_VERSION ${CPACK_PACKAGE_VERSION})
SET(OPENTRACKER_PROJECT_DESCRIPTION "An open architecture for reconfigurable tracking based on XML")
SET(OPENTRACKER_PROJECT_LIBNAME ${CPACK_PACKAGE_NAME})
SET(OPENTRACKER_PROJECT_DEFINES "")
CONFIGURE_FILE(opentracker.pc.in opentracker.pc @ONLY)
INSTALL(FILES opentracker.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

CONFIGURE_FILE(redhat/opentracker.spec.in redhat/opentracker.spec)

INCLUDE_DIRECTORIES(include ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(share)

INSTALL(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN ".svn" EXCLUDE)

IF(UNIX)
  INSTALL(FILES FindOpenTracker.cmake DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/Modules)
ENDIF(UNIX)

IF(WIN32)
  INSTALL(FILES FindOpenTracker.cmake DESTINATION $ENV{CMAKE_MODULE_PATH})
ENDIF(WIN32)

# packaging:

SET(CPACK_IGNORE_FILES
  "/#.*#$"
  "/log"
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
  "\\\\.depend"
  "\\\\.deps*"
  "\\\\.dll$"
  "\\\\.dsp$"
  "\\\\.dvi$"
  "\\\\.eps$"
  "\\\\.gz$"
  "\\\\.iv$"
  "\\\\.la$"
  "\\\\.lib$"
  "\\\\.libs$"
  "\\\\.lo$"
  "\\\\.loT$"
  "\\\\.log$"
  "\\\\.o$"
  "\\\\.out"
  "\\\\.pgm$"
  "\\\\.pro$"
  "\\\\.ps$"
  "\\\\.save"
  "\\\\.scon.*"
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
  "autom4te.cache"
  "bin"
  "cfg*"
  "cgparams$"
  "cmake_install.cmake"
  "cvs_import"
  "install.sh"
  "install_manifest.txt"
  "makedist.sh"
  "moc_"
  "pkgconfig"
  "progress.make"
  "~$"
)

SET(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
INCLUDE(CPack)
ADD_CUSTOM_TARGET(dist COMMAND make clean && make package_source)
ADD_CUSTOM_TARGET(clean-all COMMAND find \\\( -name CMakeFiles -or -name CMakeCache.txt -or -name Makefile -or -name '*.cmake' -or -name _CPack_Packages -or -name '*~' \\\) -prune -exec rm -rf {} '\;')

LINK_LIBRARIES(opentracker)

ADD_EXECUTABLE(middleware middleware.cxx)
ADD_EXECUTABLE(opentracker-bin main.cxx)
#ADD_EXECUTABLE(opentracker2c ???)
ADD_EXECUTABLE(opentrackerdemand main_ondemand.cxx)
ADD_EXECUTABLE(opentrackerubitrack main_ubitrack.cxx)

INSTALL(TARGETS middleware opentracker-bin opentrackerdemand opentrackerubitrack DESTINATION ${CMAKE_INSTALL_BINDIR})

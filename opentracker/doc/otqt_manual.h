/**
 * @page page_otqt_manual OTQt Manual
 *
 * @section sec_otqt_index Index
 *
 * - @ref sec_otqt_introduction
 * - @ref sec_otqt_implementation
 *   - @ref sec_otqt_impl_sinks
 *   - @ref sec_otqt_impl_modules
 *   - @ref sec_otqt_impl_otqt_main_class
 *   - @ref sec_otqt_impl_calibration_mode
 *   - @ref sec_otqt_impl_operation_mode
 *   - @ref sec_otqt_impl_config_proc
 * - @ref sec_otqt_installation
 *   - @ref sec_otqt_install_general
 *   - @ref sec_otqt_install_unix
 *   - @ref sec_otqt_install_windows_vs
 * - @ref sec_otqt_configuration
 *   - @ref sec_otqt_config_overview
 *   - @ref sec_otqt_config_details
 */

/**
 * @page page_otqt_manual OTQt Manual
 *
 * @section sec_otqt_introduction Introduction
 *
 * The idea for OTQt emerged from an usability problem within an existing hybrid AR
 * environment, which consists of a 3D display as well as a conventional 2D desktop
 * screen, notably a notebook. Further, the 3D scene displayed on a stereoscopic
 * projection wall is operated by some optically tracked input device similar to a pen,
 * while the 2D desktop screen is operated with a conventional desktop mouse. Thus, 3D and
 * 2D displays employ different input devices. An AR application, which takes advantage of
 * both displays faces the necessity of frequent changes of the interaction
 * device. Putting away the pen-device in order to grab the desktop mouse and vice-versa
 * causes fatigue on the application user.
 *
 * A possible solution is the construction of a single interaction device for 2D and 3D
 * display. The approach presented here is the simulation of the desktop mouse with
 * tracked input devices, thus the operation of the 2D desktop application with the 3D
 * interaction device.
 *
 * OTQt links the tracking data-flow library OpenTracker with Trolltech's Qt for desktop
 * applications. The OTQt module receives tracking data from OpenTracker, converts it into
 * different kinds of Qt mouse events and posts them to the target application via
 * standard Qt API methods.
 *
 * OTQt benefits from both foundation libraries: designed as an module extension of
 * OpenTracker, it inherits its ease of configuration describing complex tracking setups
 * with intuitive XML documents. The preprocessing of tracking data can be done exploiting
 * the rich set of reusable operations contained in OpenTracker. On the other side, the
 * Trolltech Qt API provides powerful static methods for posting mouse events and
 * retrieving required UI data. The integration of OTQt into an existing Qt project does
 * not require writing additional classes or mechanisms in the target
 * application. Applications implementing OTQt can be ported to any operating system for
 * which Qt and OpenTracker are available. Thus, using the Qt library as mouse event feed
 * was preferred over platform-specific low-level feeds such as the X window system.
 *
 * Back to @ref sec_otqt_index
 */

/**
 * @page page_otqt_manual OTQt Manual
 *
 * @section sec_otqt_implementation Implementation
 *
 * @subsection sec_otqt_impl_sinks Input Devices and Sinks
 *
 * For any subtask (mouse move, button event generation) OTQt allows the use of different
 * input devices providing tracking data.  OTQt requires another tracked device to compute
 * the relation between MPD position data and desktop screen coordinates. In the
 * following, the possible devices are listed:
 *
 * @li Application Screen Position Device (ASPD): Provides 3D position and orientation of
 * the target applicaton desktop screen (6-DOF device).
 * @li Mouse Position Device (MPD): Provides spatial 3D position data for the calculation
 * of the desktop mouse cursor position (3-DOF device).
 * @li Mouse Button Device (MBD): Provides button state values either @e enabled or @e
 * disabled for each button at a time, at least the left, right and middle mouse button.
 * @li Mouse Wheel Device (MWD): Provides wheel state values either @e enabled or @e
 * disabled for wheel forward and backward rotation.
 *
 * To separate between tracking devices, each source device receives a corresponding
 * OpenTracker @e sink @e node where the tracking device data is linked to.  OTQt
 * implements the following sink nodes as classes derived from a common base class
 * @c ot::QtMouseEventSinkBase:
 *
 * @li Application Screen Position Sink (ASPS): class @c ot::QtAppScreenPosSink
 * @li Mouse Position Sink (MPS): class @c ot::QtMousePosSink
 * @li Mouse Button Sink (MBS): class @c ot::QtMousePosSink
 * @li Mouse Wheel Sink (MWS): class @c ot::QtMouseWheelSink
 *
 *
 * @subsection sec_otqt_impl_modules Modes and Modules
 *
 * All mentioned sinks are bundled within an OpenTracker @e module which does certain
 * operations depending on the mode. Within the OTQt context a @e mode describes the
 * computations which are performed on incoming tracking events. OTQt distinguishes
 * between two modes: @e calibration @e mode and @e operation @e mode. The latter is
 * executed within the target application context where the module actually generates
 * mouse events from tracking data. The calibration mode is employed in a separate
 * executable which performs the preliminary calibration procedure of the desktop
 * screen. In this mode, the module should remain quiet.  Thus, OTQt implements a separate
 * module for each mode. The module classes are derived from the common base class @c
 * ot::QtMouseEventModuleBase:
 *
 * @li Mouse Event Module (MEM): class @c ot::QtMouseEventModule
 * @li Mouse Event Calibration Module (MECM): class @c ot::QtMouseEventCalibModule
 *
 *
 * @subsection sec_otqt_impl_otqt_main_class OTQt main class
 *
 * At the top of the OTQt class hierarchy resides the @c ot::OTQt class. It provides the
 * public interface towards target Qt applications and offers some protected members to
 * deriving classes. Internally the class is responsible for initializing the OpenTracker
 * context and adding the MEM and MECM to a context factory.
 *
 * @subsection sec_otqt_impl_calibration_mode Calibration mode
 *
 * The calibration mode is applied during the calibration routine. The purpose of the
 * calibration routine is to calculate and store the location and extent of the target
 * application screen relative to the position and orientation of the tracked 6-DOF
 * ASPD. OTQt provides the @c otqt-mem-calib command line tool, which guides the user
 * through the calibration procedure. The procedure is implemented in class
 * @c ot::OTQtMEMCalibProc.
 *
 * @image html OTQt-WorldCS2ScreenPlaneTrafo.jpg "Transformation from world CS to target application screen plane"
 *
 * @subsection sec_otqt_impl_operation_mode Operation mode
 *
 * The @e operation @e mode describes the actual mouse event generating procedure of
 * OTQt. It assumes, that the OTQt configuraton steps have been successfully performed. A
 * final XML configuration file was produced and a OTQt hook was implemented in the target
 * Qt application. With the start of the Qt application the OTQt module is initalized and
 * started.
 *
 * The MEM employs a separate class @c ot::QtAppScreen which provides methods to update
 * the geometric information about the location of the desktop screen in case the ASPD
 * changed its position and/or orientation.
 *
 * @image html OTQt-ScreenCuboidWithPen.jpg "Screen Cuboid with Mouse Position Device (MPD) displayed as pen"
 *
 * OTQt defines a certain 3D volume located in front of the target desktop screen as mouse
 * event sensible region. This region is called @e screen @e cuboid (SC).
 *
 *
 * @subsection sec_otqt_impl_config_proc Configuration Notes
 *
 * OTQt requires the ASPS, MPS and MBS to be supplied with tracking data, while the MWS
 * remains optional and can be left out if no mouse wheel events are desired.  One and the
 * same tracked device can be employed as multifunctional device acting as data source for
 * several sinks. As an example, one device can act as MPD and MBD concurrently by
 * transmitting the device data into the corresponding sinks (MPS and MBS).
 *
 * OTQt provides a @e template XML configuration file and the corresponding XML Document
 * Type Defintion (DTD) file. the user is encouraged to edit the XML file template
 * associating actual devices to the single sinks. The third and final step consists of
 * the execution of the OTQt calibration tool. The purpose of the calibration tool is to
 * determine the spatial location and extent of the target application desktop screen
 * relative to the location and orientation of the ASPD.
 *
 * Back to @ref sec_otqt_index
 */

/**
 * @page page_otqt_manual OTQt Manual
 *
 * @section sec_otqt_installation Installation
 *
 * @subsection sec_otqt_install_general General
 *
 * This section briefly describes the workflow of compiling and installing the OTQt module
 * as part of OpenTracker library. <b>The guide must be read as an extension to the
 * general OpenTracker installation instructions.</b> It only covers additional settings
 * necessary in order to build the OTQt module parts and to add them to the default
 * OpenTracker library.  General instructions concerning the installation of OpenTracker
 * can be found in dedicated pages on the project homepage (see
 * http://studierstube.icg.tu-graz.ac.at/opentracker/).
 *
 * Current OpenTracker release versions can be downloaded from
 * http://studierstube.icg.tu-graz.ac.at/opentracker/download/.
 *
 * \b NOTE: OTQt is classified as experimental module! Currently it is not part of any
 * OpenTracker release. In order to obtain it you must check out the sources from the SVN
 * repository at https://svn.icg.tu-graz.ac.at/svn/opentracker/trunk/.
 *
 * After downloading OpenTracker, unpack the source package into some directory.
 *
 * OpenTracker/OTQt requires the following software packages. Please refer to the
 * installation instructions of the respective package for details.
 *
 * - Trolltech Qt library of the 3.3 series (versions <= 3.3 may work but were not tested,
 * series 4 is not compatible with the Unix autotools build environment): free edition
 * available from http://www.trolltech.com/download/opensource.html
 * - GNU autotools (other versions may work but were not tested) autoconf 2.5, automake
 * 1.9: available from http://www.gnu.org/ (@ref sec_otqt_install_unix only)
 *
 * Now continue with the instructions according to your OS and compiler platform.
 *
 * Note: Shell commands are denoted with a @c \$\> prefix. Except where explizitely
 * denoted, the base directory of the OpenTracker source package is assumed as current
 * working directory.
 *
 *
 * @subsection sec_otqt_install_unix UNIX
 *
 * Create a convenient GNU autotools @c configure script for the current operation system
 * platform:
 *
@verbatim
$> ./bootstrap@endverbatim
 *
 * The OTQt module is disabled in OpenTracker by default. The feature must be explicitely
 * enabled by adding @b --enable-otqt as switch of the @c configure script. Optionally,
 * the location of Trolltech's Qt3 library can be specified with the @b
 * --with-qtdir=\<DIR\> switch.
 *
@verbatim
$> ./configure --enable-otqt [--with-qtdir=<DIR>]@endverbatim
 *
 * Then compile and install the OpenTracker/OTQt library:
 *
@verbatim
$> make
$> make install@endverbatim
 *
 * On Unix systems, the different parts of the OpenTracker/OTQt package are installed into
 * the following destination directories (using the default prefix @c /usr/local):
 *
@verbatim
/usr/local/bin: Binaries such as calibration tool.
/usr/local/lib: The shared library.
/usr/local/include/OpenTracker: The C++ header files.
/usr/local/share/OpenTracker: Shared data such as template XML file and DTD file.@endverbatim
 *
 *
 * @subsection sec_otqt_install_windows_vs Windows (VS .NET)
 *
 * There are Visual Studio .NET 7.1 solution and project files available as part of the
 * source package (see the @e build/VS.NET directory):
 *
 * - @c opentracker+otqt.sln: OpenTracker/OTQt solution file contains the following
 * sub-projects:
 *   - @c opentracker+otqt.dll.vcproj: builds @e opentracker.dll including the OTQt module
 *   - @c otqt-mem-calib.bin.vcproj: builds @e otqt-mem-calib.exe OTQt calibration tool
 *   - @c opentracker+otqt.bin.vcproj: build the @e opentracker.exe standalone server
 *
 * Set following additional environment variables in the system control of your Windows:
 *
 * @li @b QTDIR: Installation directory of the Trolltech Qt library (eg @c C:\\Qt3.3.5\\)
 *
 * Build the solution either in @e Debug or @e Release mode. When builing in @e Debug
 * mode, all dll's and exe's basenames will be appended with "d" (example: @e
 * opentracker.dll and @e opentrackerd.dll). The project targets are put in the following
 * directories:
 *
@verbatim
%OTROOT%\bin\win32: binaries and dll's
%OTROOT%\lib\win32: OpenTracker/OTQt include library
@endverbatim
 *
 *
 * @subsection sec_otqt_install_windows_vs_manually Windows (VS .NET manually)
 *
 * If you want to edit the default OpenTracker project files @b manually, the following
 * additional settings have to be included (with Qt 3.3.5):
 *
@verbatim
SOURCES +=
  $(OTROOT)/src/otqt/*.cxx (except otqt_mem_calib_main.cxx)
  $(OTROOT)/extras/newmat10/*.cpp

HEADERS +=
  $(OTROOT)/src/otqt/*.h
  $(OTROOT)/extras/newmat10/*.h
  $(OTROOT)/extras/tclap/*.h

INCLUDE DIRECTORIES +=
  $(QTDIR)\include
  $(QTDIR)\mkspecs\win32-msvc.net
  $(OTROOT)
  $(OTROOT)\src

PREPROCESSOR_DEFINES += QT_THREAD_SUPPORT

LIBRARY DIRECTORIES += $(QTDIR)\lib

LIBS += qt-mt335.lib qtmain.lib@endverbatim
 *
 * The file @c src/otqt/OTQt.h must be MOC'ed with the Qt meta object compiler. The
 * required additional rule can be established by clicking the appropriate toolbar button
 * provided by some Qt VS plug-in such as @e QMsNet. If no such plug-in is available, open
 * the VS .NET project file with a text editor and add the following @c
 * \<FileConfiguraton\> twice for @e Debug (see the @c Name="Debug|Win32" attribute) and
 * @e Release mode nested in the @c \<File\> element of @c OTQt.h:
 *
@verbatim
<File
    RelativePath="..\..\src\otqt\OTQt.h">
  <FileConfiguration
      Name="Debug|Win32">
    <Tool
        Name="VCCustomBuildTool"
        Description="Moc&apos;ing OTQt.h..."
        CommandLine="$(QTDIR)\bin\moc.exe ..\..\src\otqt\OTQt.h -o tmp\moc\moc_OTQt.cpp"
        AdditionalDependencies="$(QTDIR)\bin\moc.exe"
        Outputs="tmp\moc\moc_OTQt.cpp"/>
  </FileConfiguration>
</File>@endverbatim
 *
 * Create another Win32 application project in order to build the @c otqt-mem-calib.exe
 * calibration tool. Inherit the default and OTQt-specific settings above. Add the source
 * files and specify the destination path of the project:
 *
@verbatim
SOURCES +=
  $(OTROOT)/src/otqt/otqt_mem_calib_main.cxx

DESTINATION :=
  $(OTROOT)/bin/win32/otqt-mem-calib.exe
@endverbatim
 *
 * When finally building the modified OpenTracker solution, the same results as in @ref
 * sec_otqt_install_windows_vs should be yielded.
 *
 * Back to @ref sec_otqt_index
 */

/**
 * @page page_otqt_manual OTQt Manual
 *
 * @section sec_otqt_configuration Configuration
 *
 * This section describes the configuration workflow for the usage of the OTQt module
 * together with a desktop Qt application. In the subsequent section an overview of the
 * workflow is given followed by a section which contains more detailed information about
 * the single configuration steps.
 *
 *
 * @section sec_otqt_config_overview Overview
 *
 * -# @ref sec_otqt_config_step_1\n\n
 * Copy @e template @c otqt-mem-config-template.xml} and @c opentracker.dtd from the OTQt
 * data directory to the execution directory of the target Qt application.
@verbatim
$> cp /usr/local/share/OpenTracker/* <QT_APP_LOCATION>@endverbatim
 * -# @ref sec_otqt_config_step_2\n\n
 * Modify the copied template XML file or create an @e intermediate configuration file
 * from scratch. Describe the tracking environment as OpenTracker data flow graph. Link
 * tracking data sources to the OTQt sinks. Apply preprocessing operations on the tracking
 * data to fulfill the interface specififytions of the OTQt sinks.
 * -# \n @ref sec_otqt_config_step_3\n\n
 * Execute the calibration routine providing the @e intermediate file in order
 * to generate a @e final XML configuration file.
@verbatim
$> otqt-mem-calib -o final.xml intermediate.xml@endverbatim
 * -# @ref sec_otqt_config_step_4\n\n
 * Implement OTQt in the target Qt application @c main method (using public static methods
 * from the @c OTQt class interface) providing the @e final configuration file as
 * argument.
@verbatim
OTQt::triggerMEM("final.xml")@endverbatim
 * -# @ref sec_otqt_config_step_5\n\n
 * Extend the build environment by specifying special compiler and linker flags for
 * OTQt. Compile the Qt application.
@verbatim
INCLUDEPATH += $${OPENTRACKER_DIR}/include
LIBS += -L$${OPENTRACKER_DIR}/lib -lOpenTracker@endverbatim
 * -# @ref sec_otqt_config_step_6
 *
 *
 * @section sec_otqt_config_details Details
 *
 * @subsection sec_otqt_config_step_1 Copy XML template and DTD files
 *
 * To ease the creation of a valid OpenTracker/OTQt XML configuration file, OTQt provides
 * a template XML configuration file and the corresponding DTD file. The mentioned files
 * reside in the @c /usr/local/share/OpenTracker directory if OTQt was installed as described
 * in the previous section. The DTD file must be put in the execution directory of the
 * target Qt application, where OpenTracker will try to find it at startup. Without the
 * appropriate DTD file, OpenTracker will fail to parse the XML configuration file.
 *
@verbatim
$> cp /usr/local/share/OpenTracker/* <QT_APP_LOCATION>
otqt-mem-config-template.xml -> <QT_APP_LOCATION>
opentracker.dtd -> <QT_APP_LOCATION>@endverbatim
 *
 * @subsection sec_otqt_config_step_2 Edit XML template file
 *
 * The copied template XML file must be customized according to the actual tracking
 * system. External sources must be specified, which supply data to the OTQt sinks. Other
 * OpenTracker nodes can be used to manipulate the external data to fit the interface
 * specifications of the OTQt sinks. The template configuration file looks like this:
 *
@verbatim
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE OpenTracker SYSTEM "opentracker.dtd">
<OpenTracker>

  <configuration>
    <!-- (I) Mouse Event (Calibration) Module
             configuration -->
    <QtMouseEventCalibConfig />
  </configuration>

  <!-- (II) Tracking Device Source Section -->
  <TODOUnknownSource DEF="AppScreenPositionDevice" />
  <TODOUnknownSource DEF="MousePositionDevice" />
  <TODOUnknownSource DEF="MouseButtonDevice" />
  <TODOUnknownSource DEF="MouseWheelDevice" />

  <!-- (III) Sink Section -->
  <QtAppScreenPosSink>
    <Ref USE="AppScreenPositionDevice" />
  </QtAppScreenPosSink>
  <QtMousePosSink>
    <Ref USE="MousePositionDevice" />
  </QtMousePosSink>
  <QtMouseButtonSink>
    <Ref USE="MouseButtonDevice" />
  </QtMouseButtonSink>
  <QtMouseWheelSink>
    <Ref USE="MouseWheelDevice" />
  </QtMouseWheelSink>

</OpenTracker>@endverbatim
 *
 * The @e template file is kept very simple, designed in a such a way that the user only
 * has to insert proper external tracking data sources in (II), which are then redirected
 * to the OTQt sinks in (III). One and the same tracking device can be redirected to
 * different sinks in (III) as well as manipulation nodes can be applied to preprocess
 * (merge, filter) the incoming tracking events. The template file can be edited
 * arbitrarily creating more complex data flow graphs. Exception: the line after (I) must
 * remain unedited, since this line is expected to look exactly that way in the subsequent
 * calibration routine. The resulting XML file of this step is called @e intermediate and
 * may look like this:
 *
@verbatim
<!-- [..] -->

<!-- (II) Tracking Device Source Section -->
<NetworkSource number="12" DEF="ASPD" />
<NetworkSource number="9"  DEF="MousePositionDevice" />
<NetworkSource number="20" DEF="MouseButtonDevice" />
<!-- (A) device not specified -->
<!--   <TODOUnknownSource DEF="MouseWheelDevice" /> -->

<!-- (III) Sink Section -->
<!-- [..] -->
<QtMouseButtonSink>
  <Ref USE="MouseButtonDevice" />
</QtMouseButtonSink>
<QtMouseWheelSink>
  <!-- (B) device reused -->
  <Ref USE="MouseButtonDevice" />
</QtMouseWheelSink>

<!-- [..] -->@endverbatim
 *
 * Here, the source data for the sinks arrives over the network at the specified @c
 * NetworkSource nodes in (II). With the number attribute, different streams for ASPD,
 * MPD, MBD are distinguished. Note, that no explizit MWD source for the MWS is specified
 * (A), rather the MPD supplies the data to the MWS in (B).
 *
 *
 * @subsection sec_otqt_config_step_3 Calibrate desktop screen
 *
 * The intermediate XML configuration file lacks the results from a calibration routine of
 * the target application desktop screen. The provided OTQt calibration tool for that
 * purpose is started from command line providing one mandatory argument, the filename of
 * a valid and compliant input configuration file, for example the intermediate
 * configuration file described before. With the optional @b "-o" flag the user may specify
 * the output XML filename. By default, the output is printed to console.
 *
@verbatim
$> otqt-mem-calib -o final.xml intermediate.xml@endverbatim
 *
 * The calibration tool does @b syntax-check the given input OTQt configuration
 * file. Errors occuring during the parsing of the file are reported to console by
 * OpenTracker. Once the OpenTracker context has been established successfully, the tool
 * performs @b tracking @b tests with the mandatorily specified devices ASPD, MPD and
 * MBD. If all of these devices are perceived correctly the calibration routine is
 * entered. Otherwise the tool quits with an appropriate error message.
 *
 * The calibration routine asks the user to <b>mark the four corners of the target
 * application desktop screen</b> in the following sequence: top left, top right, bottom
 * right, bottom left.
 *
 * Each screen corner is detected over the position of the MPD. The user has to direct the
 * MPD to the desired position and press the "left" button of the MBD.  The corner is
 * recognized by the tool after the MPD position remained stable for more than two seconds
 * and the "left" button remained pressed throughout that time. If the MPD position leaves
 * a certain threshold region during the measurement time, the timer is
 * restarted. Releasing the "left" MPD button also resets the timer.
 *
 * Once all four corner points of the desktop screen have been marked, the tool computes
 * the calibration data, edits the input XML file and <b>prints the resulting output XML
 * file</b> either to console or to the specified output destination file. The edited
 * parts of the returned XML file look the following:
 *
@verbatim
<!-- [..] -->
<configuration>
  <!-- (I) Mouse Event (Calibration) Module
       configuration -->
  <QtMouseEventConfig>
    <!-- (A) Calibration Data Element -->
    <QtAppScreen
      ASHeightVec="0.0000 -0.9999 0.0000"
      ASWidthVec="0.9999 0.0000 0.0000"
      CSOrientationQuat="0.0000 0.0000 -0.7071 0.7071"
      CSRoot2ScreenRootVec="-0.9999 -0.9999 0.0000"
    />
  </QtMouseEventConfig>
</configuration>
<!-- [..] -->@endverbatim
 *
 * The @c QtMouseEventCalibConfig XML element is replaced with a @c QtMouseEventConfig
 * element in (I). The nested @c QtAppScreen element (A) contains attributes which store
 * the computed calibration data.
 *
 * Summing up, the OTQt calibration tool converts an @e intermediate} XML file into a @e
 * final OTQt configuration file, applicable in the target Qt application.
 *
 *
 * @subsection sec_otqt_config_step_4 Implement OTQt hook in Qt application
 *
 * The OTQt public interface provides some static methods which abstract the
 * initialization startup of OTQt and are designed to be implemented in Qt application
 * @c main methods:
 *
@verbatim
#include "mainwindow.h"
#include <qapplication.h>
/* (A) Include OTQt header */
#include <otqt/OTQt.h>

int main( int argc, char ** argv )
{
  QApplication a( argc, argv );
  MainWindow w;
  a.setMainWidget(&w);
  w.show();

  /* (B) Initialize/Start Mouse Event Module */
  if (!ot::OTQt::triggerMEM("final.xml"))
    return -1;

  return a.exec();
}@endverbatim
 *
 * The integration is done by including the @c OTQt.h header file in (A) and using
 * the static wrapper method in (B) passing the final XML configuration file obtained from
 * the calibration tool.
 *
 * @subsection sec_otqt_config_step_5 Compile the Qt application with OTQt
 *
 * In order to successfully compile the Qt application with OTQt, the Qt application build
 * files must be extended if the OpenTracker and/or OTQt header files and libraries have
 * been installed to a non-system-default location. Assuming, that the Qt application
 * developer employs @e qmake as Makefile generator, the following lines should be
 * included into the application project files, adding the necessary compiler and linker
 * flags:
 *
@verbatim
# (I) Specify Installation Paths
OPENTRACKER_DIR = /non/default/install/directory/OpenTracker
# (II) Add Compiler + Linker Flags
INCLUDEPATH += $${OPENTRACKER_DIR}/include/OpenTracker
LIBS += -Wl,-rpath -Wl,$${OPENTRACKER_DIR}/lib
LIBS += -L$${OPENTRACKER_DIR}/lib -lOpenTracker@endverbatim
 *
 * @subsection sec_otqt_config_step_6 Start the Qt application with OTQt
 *
 * After compilation, the Qt application can be started. The OTQt module is started
 * automatically together with the application providing the mouse event generating
 * services.
 *
 * Back to @ref sec_otqt_index
 */


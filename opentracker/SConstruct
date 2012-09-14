#!/usr/bin/env python

import sys
import os
sys.path.append('ICGBuilder')
import icgbuilder
import buildutils

env = Environment (ENV = os.environ)
if ARGUMENTS.has_key("ENABLE_CORBA"):
    import OmniIdlCxx
    OmniIdlCxx.generate(env)

if ARGUMENTS.has_key("ENABLE_OMNIORBPY"):
    import OmniIdlPy
    OmniIdlPy.generate(env)
#***************************************************************************************
#
#                  BUILD TARGETS DEFINITION
# This section defines the build targets as dictionaries containing the relevant
# information. The targets could also be loaded from an external file, this is to be
# implemented in the next version of the build system.
# The available fields to provide information about a target are as follows:
#
#   name :      string representing the name of the compiled target without extension.
#   type :      string, depends on the kind of compile, available values are
#                      DL  for dynamic libraries
#                      LIB for static libraries
#                      PRG for consonle applications
#   libs :      list of libraries that will be linked with the target, the builder
#               will attempt to find the appropriate include path, and libpath for
#               these libraries, failure to get this information will result in
#               a script error.
#   use  :      dictionary of other libraries on which the target depends. Each of these
#               will result in the inclusion of one defines flag and the addition of
#               an include path to the compilation. Failure to find include paths
#               for USE variables will result in a script warning
#   src_root:   [Optional] the root directory of the source tree, if missing it
#               will be set by the ConfigurationBuilder
#   src_ignore: [Optional] list of directories or files inside the src_root tree
#               that must be ignored for this target
#   src_use:    [Optional] list of directories or files inside the src_root tree
#               that must be used to build the target. If not defined, all folders
#               and files inside the src_root will be used to build the target
#
#***************************************************************************************

# if you want the builder to use a custom setup for targets, you can create a
# custom.py file specifying the whole setup. 
if os.path.exists('custom.py'):
    from custom import *

else:
    #****************** default configuration ********************************
    #   if you want to use a custom.py, start by copying the following block
    #   then change the definitions to whatever you like.
    #*************************************************************************
    
    version      = '2.0.0'
    project      = 'opentracker'
    description  = project + ' is a modified "small" library for handling tracking'
    mainlib      = 'opentracker'
    installRoot  = os.getcwd()
    includeDir   = os.path.join(os.getcwd(),'include')
    prefix       = ''

    enableFlags = ['muddleware','corba','pyqt','otqt']
    gllibs = []
    targetList = []

    # use flags that will be shared both by static and dynamic libs.
    use= ['xerces']

    if sys.platform == 'win32':
        # list of libraries that will be searched by the scanner.
        # The scanner will try to locate the libraries and the flags
        # needed to build with those libraries. The obtained result
        # will be used by the build, for each target that lists a
        # library in its 'libs' or in its 'use' sections.
        libraryList =['boost', 'ace', 'xercesc', 'omniorb', 'muddleware','ot',
		      'openvideo', 'tinyxml', 'tinyxmlmod','artoolkitplus','ptu', 'midi', 'icubex']

        use= ['xercesc', 'midi', 'icubex']

        ot ={'name': 'opentracker',
             'type': 'DL',
             'libs': ['ace'],
             'use' : use,
             'src_ignore': ['standalones','otqt', 'tool\\tool.cxx', 'tool\\ToolAppWin.cxx', 'tool\\ToolIOModule.cxx', 'network\\CORBAModule.cxx', 'network\\CORBASink.cxx', 'network\\CORBAUtils.cxx', 'misc\\wince'],
             'src_use' : ['ALL', os.environ['OTROOT']+'\\extras\\intersense\\isense.c'],
             'defines' : ['"USE_ARTOOLKITPLUS=1"','"TINYXML_MOD_DLL=1"', '"ARTOOLKITPLUS_DLL=1"', 'OPENTRACKER_EXPORTS', 'USING_SCONS']
             }

        otlib ={'name':'opentrackerlib',
                'type':'LIB',
                'libs': ['ace'],
                'use' :use,
                'src_ignore': ['standalones','otqt', 'tool\\tool.cxx', 'tool\\ToolAppWin.cxx', 'tool\\ToolIOModule.cxx', 'network\\CORBAModule.cxx', 'network\\CORBASink.cxx', 'network\\CORBAUtils.cxx', 'misc\\wince'],
                'src_use' : ['ALL', os.environ['OTROOT']+'\\extras\\intersense\\isense.c'],
                'defines' : ['"TINYXML_MOD_DLL=1"', '"ARTOOLKITPLUS_DLL=1"', 'OPENTRACKER_STATIC','USING_SCONS']	
                }
        
        otcon ={'name':'opentracker',
                'type':'PRG',
                'libs':['opentracker'],
                'src_use': ['standalones/main.cxx']
                }
        
	otcondemand ={'name':'opentrackerdemand',
                'type':'PRG',
                'libs':['opentracker'],
                'src_use': ['standalones/main_ondemand.cxx']
                }
	
	otconubitrack ={'name':'opentrackerubitrack',
                'type':'PRG',
                'libs':['opentracker'],
                'src_use': ['standalones/main_ubitrack.cxx']
                }

        middleware ={'name':'middleware',
                'type':'PRG',
                'libs':['opentracker'],
                'src_use': ['standalones/middleware.cxx']
                }

        otcon2 ={'name':'opentracker2c',
                 'type':'PRG',
                 'libs':['opentracker', 'ace'],
                 'src_use': ['standalones/configurable.cxx']
                 }

        targetList.append(ot)
        targetList.append(otlib)
        targetList.append(otcon)
	targetList.append(otcondemand)
	targetList.append(otconubitrack)
        targetList.append(middleware)
        targetList.append(otcon2)
    elif sys.platform == 'linux' or sys.platform == 'linux2':
        # list of libraries that will be searched by the scanner.
        # The scanner will try to locate the libraries and the flags
        # needed to build with those libraries. The obtained result will
        # be used by the build, for each target that lists a library
        # in its 'libs' or in its 'use' sections.
        libraryList =['boost', 'ACE', 'xerces', 'omniorb', 'muddleware','ot',
		      'openvideo', 'tinyxml', 'tinyxmlmod','artoolkitplus']

        ot ={'name': 'opentracker',
             'type': 'DL',
	     'defines' : ['USE_ARTOOLKITPLUS=1', 'USE_ARTOOLKITPLUS'],
             'libs': ['ACE','xerces-c','curses','artoolkitplus','tinyxmlmod'],
             'use' : use,
             'src_ignore': ['standalones','otqt', 'tool/tool.cxx', 'tool/ToolAppWin.cxx', 'tool/ToolIOModule.cxx', 'network/CORBAModule.cxx', 'network/CORBASink.cxx', 'network/CORBAUtils.cxx', 'misc/wince', 'misc/portio.cxx'],


             'src_use' : ['ALL', os.path.join(os.getcwd(), 'extras/intersense/isense.c') ]
             }

        otcon ={'name':'opentracker',
                'type':'PRG',
                'libs':['opentracker','ACE'],
                'src_use': ['standalones/main.cxx']
                }
	
	otcondemand ={'name':'opentrackerdemand',
                'type':'PRG',
                'libs':['opentracker','ACE'],
                'src_use': ['standalones/main_ondemand.cxx']
                }
	
	otconubitrack ={'name':'opentrackerubitrack',
                'type':'PRG',
                'libs':['opentracker','ACE'],
                'src_use': ['standalones/main_ubitrack.cxx']
                }
    
        middleware ={'name':'middleware',
                'type':'PRG',
                'libs':['opentracker','ACE'],
                'src_use': ['standalones/middleware.cxx']
                }
    
        otcon2 ={'name':'opentracker2c',
                 'type':'PRG',
                 'libs':['opentracker','ACE'],
                 'src_use': ['standalones/configurable.cxx']
                 }
        if ARGUMENTS.has_key('ENABLE_CORBA'):
            otcon2corba ={'name':'opentracker2con',
                          'type':'PRG',
                          'libs':['opentracker','ACE','omniORB4', 'COS4',
                                  'omniDynamic4', 'omnithread'],
                          'src_use': ['standalxfones/corba_configurable.cxx']
                          }
            targetList.append(otcon2corba)
            
        targetList.append(ot)
        targetList.append(middleware)        
        targetList.append(otcon)
        targetList.append(otcondemand)
	targetList.append(otconubitrack)
        targetList.append(otcon2)

    elif sys.platform =='darwin':
	env.Append(CPPPATH = ['/opt/local/include']) # used by Darwin Ports
	env.Append(LIBPATH = ['/opt/local/lib'])     # used by Darwin Ports
        # list of libraries that will be searched by the scanner.
        # The scanner will try to locate the libraries and the flags
        # needed to build with those libraries. The obtained result will
        # be used by the build, for each target that lists a library
        # in its 'libs' or in its 'use' sections.
        libraryList =['boost', 'ACE', 'xerces', 'omniorb', 'muddleware','ot',
		      'openvideo', 'artoolkitplus']                      
	#	      'openvideo', 'tinyxml', 'tinyxmlmod','artoolkitplus']

        ot ={'name': 'opentracker',
             'type': 'DL',
             'libs': ['ACE','xerces-c','curses'],
             'use' : use,
             'src_ignore': ['standalones','otqt', 'tool/tool.cxx', 'tool/ToolAppWin.cxx', 'tool/ToolIOModule.cxx', 'network/CORBAModule.cxx', 'network/CORBASink.cxx', 'network/CORBAUtils.cxx', 'misc/wince', 'misc/portio.cxx', 'input/ParButtonModule.cxx','input/InterSenseModule.cxx','input/LinmouseModule.cxx'],
             'src_use' : ['ALL']
             }
        
        middleware ={'name':'middleware',
                'type':'PRG',
                'libs':['opentracker','ACE'],
                'src_use': ['standalones/middleware.cxx']
                }

        otcon ={'name':'opentracker',
                'type':'PRG',
                'libs':['opentracker','ACE'],
                'src_use': ['standalones/main.cxx']
                }
    
	otcondemand ={'name':'opentrackerdemand',
                'type':'PRG',
                'libs':['opentracker','ACE'],
                'src_use': ['standalones/main_ondemand.cxx']
                }
	otconubitrack ={'name':'opentrackerubitrack',
                'type':'PRG',
                'libs':['opentracker','ACE'],
                'src_use': ['standalones/main_ubitrack.cxx']
                }

        otcon2 ={'name':'opentracker2c',
                 'type':'PRG',
                 'libs':['opentracker','ACE'],
                 'src_use': ['standalones/configurable.cxx']
                 }

        targetList.append(ot)
        targetList.append(otcon)
        targetList.append(otcondemand)
	targetList.append(otconubitrack)
        targetList.append(otcon2)
        targetList.append(middleware)
    if ARGUMENTS.has_key("ENABLE_OMNIORBPY") or ARGUMENTS.has_key("ENABLE_CORBA"):
        print "Currently it is not possible to run 'scons install' in one single step"
        print "It is necessary to first run 'scons' in order to generate stubs, "
        print "including python stubs if appropriate, and have it recognised in a "
        print "scan of the filesystem.\n"
    if ARGUMENTS.has_key('ENABLE_MUDDLEWARE'):
        print "Support for Muddleware....... enabled."
        print 'Please modify the targets to build support for Muddleware. IF YOU SEE THIS MESSAGE WHILE COMPILING IS BECAUSE THIS FEATURE HAS NOT BEEN ADDED TO THE BUILD SCRIPT. Developers should modify the targetList at the point where this message is generated to add support for Muddleware.\n'
	os.abort()        	
    if ARGUMENTS.has_key('ENABLE_CORBA'):
        print 'Support for Corba....... enabled.'
        corba_libs = ['omniORB4', 'COS4', 'omniDynamic4', 'omnithread']
        ot['libs']  += corba_libs
        stubsandskeletons = env.OMNIIDLCXX(os.path.join('idl', 'OT_CORBA.idl'),
                                           OMNIIDL_INSTALL_DIRECTORY=os.path.join('idl', 'skeletons')) + env.OMNIIDLCXX(os.path.join('idl', 'UTS.idl'),
                                           OMNIIDL_INSTALL_DIRECTORY=os.path.join('idl', 'skeletons')) 
        if ARGUMENTS.has_key('ENABLE_OMNIORBPY'):
            pythonstubsandskeletons =  env.OMNIIDLPY(os.path.join('idl', 'OT_CORBA.idl'), OMNIIDL_INSTALL_DIRECTORY=os.path.join('lib', 'python'))
            pythonstubsandskeletons += env.OMNIIDLPY(os.path.join('idl', 'UTS.idl'), OMNIIDL_INSTALL_DIRECTORY=os.path.join('lib', 'python'))
            corbapystubs ={'name': 'corbastubs',
                 'type': 'PY',
                 'src_use' : [os.path.abspath(str(stub)) for stub in pythonstubsandskeletons]
                 }
            targetList.append(corbapystubs)
        try:
            ot['defines'] += ['USE_CORBA']
        except KeyError:
            ot['defines'] = ['USE_CORBA']
        corba_files = [os.path.join('src', 'network', f) for f in ['CORBAModule.cxx', 'CORBASink.cxx', 'CORBAUtils.cxx']]
        cxxstubsandskeletons = []
        for stub in stubsandskeletons:
            if str(stub).endswith('.cc'):
                #stub_in_tree = os.path.join('src', 'skeletons', os.path.basename(str(stub)))
                #print stub_in_tree, str(stub)
                #env.Copy(stub_in_tree, stub)
                cxxstubsandskeletons.append(stub) 
            if str(stub).endswith('.hh'):
                stub_header = os.path.join('include', 'OpenTracker', 'skeletons', os.path.basename(str(stub)))
                #print stub_header, stub
                env.Command(stub_header, stub, Copy("$TARGET", "$SOURCE"))
        #headers = [os.path.join for f in stubsandskeletons if str(f).endswith('.hh')
        #ot['src_use']    += corba_files + cxxstubsandskeletons
        ot['src_use']    += [os.path.abspath(str(f)) for f in cxxstubsandskeletons]
        for f in corba_files:
            #print f, os.path.basename(f), ot['src_ignore']
            ot['src_ignore'].remove(os.path.join('network', os.path.basename(f)))
        otcon['libs']  += corba_libs
        otcon2['libs'] += corba_libs
        #print 'Please modify the targets to build support for Corba. IF YOU SEE THIS MESSAGE WHILE COMPILING IS BECAUSE THIS FEATURE HAS NOT BEEN ADDED TO THE BUILD SCRIPT. Developers should modify the targetList at the point where this message is generated to add support for Corba.\n'
	#os.abort()
    if ARGUMENTS.has_key('ENABLE_OMNIEVENTS'):
        if not ARGUMENTS.has_key('ENABLE_CORBA'):
            print "Can't have support for OmniEvents without having support for CORBA!"
            print "set ENABLE_CORBA=1"
            os.abort()
        ot['defines'] += ['USE_OMNIEVENTS']
        stubsandskeletons = env.OMNIIDLCXX(os.path.join('idl', 'OT_EventChannel.idl'),
                                           CPPPATH=['/usr/local/share/idl/omniORB', '/usr/share/idl/omniORB'],
                                           OMNIIDL_INSTALL_DIRECTORY=os.path.join('idl', 'skeletons'))
        if ARGUMENTS.has_key('ENABLE_OMNIORBPY'):
            pythonstubsandskeletons = env.OMNIIDLPY(os.path.join('idl', 'OT_EventChannel.idl'),
                                                    CPPPATH=['/usr/local/share/idl/omniORB', '/usr/share/idl/omniORB'],
                                                    OMNIIDL_INSTALL_DIRECTORY=os.path.join('lib', 'python'))
            eventpystubs ={'name': 'eventstubs',
                           'type': 'PY',
                           'src_use' : [os.path.abspath(str(stub)) for stub in pythonstubsandskeletons]
                           }
            targetList.append(eventpystubs)
        for stub in [str(f) for f in stubsandskeletons if str(f).endswith('.hh')]:
            env.Command(os.path.join('include','OpenTracker', 'skeletons', os.path.basename(stub)), stub, Copy("$TARGET", "$SOURCE"))
        ot['src_use'] += [os.path.abspath(str(f)) for f in stubsandskeletons if str(f).endswith('.cc')]
        print 'Support for OmniEvents....... enabled.'
    if ARGUMENTS.has_key('ENABLE_SHARED'):
        if not ARGUMENTS.has_key('ENABLE_CORBA'):
            print "Can't have support for Shared without having support for CORBA!"
            print "set ENABLE_CORBA=1"
            os.abort()
        if not ARGUMENTS.has_key('ENABLE_CORBA'):
            print "Can't have support for Shared without having support for OmniEvents!"
            print "set ENABLE_OMNIEVENTS=1"
            os.abort()
        print 'Support for Shared ....... enabled.'
        ot['defines'] += ['USE_SHARED']
        ot['libs'] += ['shared']
    if ARGUMENTS.has_key('ENABLE_PHANTOM_MIDDLEWARE'):
        print 'Support for Phantom Middleware....... enabled.'
        ot['defines'] += ['USE_PHANTOMMIDDLEWARE']
        ot['libs'] += ['phantom']
    if ARGUMENTS.has_key('ENABLE_PHANTOM'):
        print 'Support for Phantom Omni Haptic Device....... enabled.'
        try:
            ot['defines'] += ['USE_PHANTOM']
        except KeyError:        
            ot['defines'] = ['USE_PHANTOM']
        try:
            ot['libs'] += ['HDU','HD','HLU','HL']           
        except KeyError:        
            ot['libs'] = ['HDU','HD','HLU','HL']
        
    if ARGUMENTS.has_key('ENABLE_PYQT'):
        print 'Support for PyQt....... enabled.'
        print 'Please modify the targets to build support for PyQt. IF YOU SEE THIS MESSAGE WHILE COMPILING IS BECAUSE THIS FEATURE HAS NOT BEEN ADDED TO THE BUILD SCRIPT. Developers should modify the targetList at the point where this message is generated to add support for PyQt.\n'
	os.abort()        	
    if ARGUMENTS.has_key('ENABLE_OTQT') or ARGUMENTS.has_key('ENABLE_OTQT_QT3'):
        # make the library scanner search the configuration of qt
        libraryList.append('qt-mt')
        # add the qt flags to the dll
        ot['libs'].append('qt-mt')
        try:
            ot['defines'] += ['QT_NO_DEBUG', 'QT_THREAD_SUPPORT', 'USE_OTQT']
        except KeyError:
            ot['defines'] = ['QT_NO_DEBUG', 'QT_THREAD_SUPPORT', 'USE_OTQT']
        ot['src_use'].append('../extras/newmat10')
        #ot['src_use'].append('../include/OpenTracker/otqt')
	# adding a source that does not exist to the target
        ot['src_ignore'].remove('otqt')
        ot['src_ignore'].append('otqt/otqt_mem_calib_main.cxx')
        #use['otqt'] = 'true'
        otcon['libs'].append('qt-mt')
        otcondemand['libs'].append('qt-mt')
        otconubitrack['libs'].append('qt-mt')
        otcon2['libs'].append('qt-mt')
        
        otqtcalib = {'name':'otqt_calib',
                'type':'PRG',
                'libs':['opentracker','ACE','qt-mt'],
                'src_use': ['otqt/otqt_mem_calib_main.cxx','../extras/newmat10']
                }
        try:
            otqtcalib['defines'] += ['QT_NO_DEBUG', 'QT_THREAD_SUPPORT', 'USE_OTQT']
        except KeyError:
            otqtcalib['defines'] = ['QT_NO_DEBUG', 'QT_THREAD_SUPPORT', 'USE_OTQT']
        mocaction='moc-qt3 $SOURCE -o $TARGET'
        bld = Builder(action=mocaction)
        env = Environment(ENV=os.environ, tools=['default','qt'])
        env['QT_LIB'] = 'qt-mt'
        #env['QT_DEBUG'] = 1
        #env['QT_AUTOSCAN'] = 0
        env['BUILDERS']['moc']= bld
        mocdep = env.moc('src/otqt/moc_OTQT.cxx', 'include/OpenTracker/otqt/OTQt.h')

	# a NodeList is added to the src_use. In this case they will be added as 
	# source files for the compiler, and also as dependencies in the environment
	# so that the appropriate builder is called before compiling the target
        ot['src_use'].append(mocdep)

        targetList.append(otqtcalib)

        print 'Support for OTQt based on Qt version 3.x ....... enabled.'
    elif ARGUMENTS.has_key('ENABLE_OTQT_QT4'):
        ### env.Append( QT_MOC = '$QT_BINPATH/moc-qt4' )
        # make the library scanner search the configuration of qt
        libraryList.append('QtCore')
        libraryList.append('QtGui')
        # add the qt flags to the dll
        ot['libs'].append('QtCore')
        ot['libs'].append('QtGui')
        try:
            ot['defines'] += ['QT_NO_DEBUG', 'USE_OTQT', 'USE_QT4']
        except KeyError:
            ot['defines'] = ['QT_NO_DEBUG', 'USE_OTQT', 'USE_QT4' ]
        ot['src_use'].append('../extras/newmat10')
        #ot['src_use'].append('../include/OpenTracker/otqt')
	# adding a source that does not exist to the target
        ot['src_ignore'].remove('otqt')
        ot['src_ignore'].append('otqt/otqt_mem_calib_main.cxx')
        #use['otqt'] = 'true'
        otcon['libs'].append('QtCore')
        otcon['libs'].append('QtGui')
        otcondemand['libs'].append('QtCore')
        otcondemand['libs'].append('QtGui')
        otconubitrack['libs'].append('QtCore')
        otconubitrack['libs'].append('QtGui')
        otcon2['libs'].append('QtCore')
        otcon2['libs'].append('QtGui')
        
        otqtcalib = {'name':'otqt_calib',
                'type':'PRG',
                'libs':['opentracker','ACE','QtCore','QtGui'],
                'src_use': ['otqt/otqt_mem_calib_main.cxx','../extras/newmat10']
                }
        try:
            otqtcalib['defines'] += ['QT_NO_DEBUG', 'QT_THREAD_SUPPORT', 'USE_OTQT4']
        except KeyError:
            otqtcalib['defines'] = ['QT_NO_DEBUG', 'QT_THREAD_SUPPORT', 'USE_OTQT4']
        mocaction='moc-qt4 -DUSE_OTQT=1 -DUSE_QT4 $SOURCE -o $TARGET'
        bld = Builder(action=mocaction)
        env = Environment(ENV=os.environ, tools=['default','qt'])
        env['QT_LIB'] = 'QtCore'
        #env['QT_DEBUG'] = 1
        #env['QT_AUTOSCAN'] = 0
        env['BUILDERS']['moc']= bld
        mocdep = env.moc('src/otqt/moc_OTQT.cxx', 'include/OpenTracker/otqt/OTQt.h')

	# a NodeList is added to the src_use. In this case they will be added as 
	# source files for the compiler, and also as dependencies in the environment
	# so that the appropriate builder is called before compiling the target
        ot['src_use'].append(mocdep)

        targetList.append(otqtcalib)

        print 'Support for OTQt based on Qt version 4.x ....... enabled.'

    if ARGUMENTS.has_key('NO_OT_LOCAL_GRAPH'):
	for t in targetList:
	    if t.has_key('defines'):
		t['defines'].append('NO_OT_LOCAL_GRAPH')
	    else:
		t['defines']=['NO_OT_LOCAL_GRAPH']

#**********************  End of the default configuration*******************************
#     if you want to use a custom.py, you should first copy up to here, and then
#     change the values to suit your own needs.
#***************************************************************************************


#======================== CONFIGURATION SECTION =============================
# Following is the configuration section. In it most of the options for
# building the specified targets will be set, and then written to a file
# called config.user. This file can be later personalized to allow some
# other user defined configuration.
#===========================================================================#

# Following are default flags for creating the supported target types. 
# Default flags will be applied to all targets of the same sort. In this
# section flags for different types of builds in different platforms can be
# set so that they apply to all targets of the same sort. Supported targets
# are dynamic libraries (dllflags), static libraries (libflags), and 
# executables (prgflags).


if sys.platform == 'win32':
	from win32Flags import defaultBuildFlags
	
	# list of libraries that might be used by this project
	scannerType = 'env'

	# directories needed for the build and install process

	buildDir     = os.path.join(os.getcwd(), 'build', 'win32')



	# attempt to add the define DLLNAME_EXPORTS to all targets of type DL
	# in most of the cases it might solve the problem of exporting symbols
	for t in targetList:
		if t['type'] == 'DL':
			if t.has_key('defines'):
				t['defines'].append(t['name'].upper() + '_EXPORTS')
			else:
				t['defines'] = [t['name'].upper() + '_EXPORTS']
	
elif sys.platform == 'linux' or sys.platform == 'linux2':
	from linuxFlags import *
	
	scannerType = 'mix'

	# directories needed for the build and install process
	buildDir     = os.path.join(os.getcwd(), 'build', 'linux')



elif sys.platform == 'darwin':
	from darwinFlags import *
	
	# list of libraries that might be used by this project
	scannerType = 'mix'

	buildDir     = os.path.join(prefix, 'build', 'darwin')


srcDir       = os.path.join(os.getcwd(), 'src')	


	
print 'CONFIGURE: Searching for installed libraries'
# to get the include path for all libraries we need to retrieve 
# the environment vars pointing to the root of each lib
envvars = os.environ

targets = []
# create the builder with an empty target list
buildConfig = icgbuilder.ConfigBuilder(project, scannerType, envvars, ARGUMENTS , libraryList)

# add extra configuration flags
buildConfig.desc     = description
buildConfig.mainlib  = mainlib
buildConfig.version  = version
buildConfig.prefix   = prefix
buildConfig.buildDir = buildDir
buildConfig.srcDir   = srcDir
buildConfig.enableFlags = enableFlags

# setup the default build flags in the configurator
buildConfig.setDefaultBuildFlags(defaultBuildFlags)

# add main include directory to all configurations
buildConfig.setIncDir(includeDir)
buildConfig.addIncDir(os.path.join(os.getcwd(), 'extras', 'intersense'))
buildConfig.addIncDir(os.path.join(os.getcwd(), 'extras'))
buildConfig.addIncDir(os.path.join(os.getcwd(), 'extras', 'newmat10'))


# create the build targets
buildConfig.setTargetList(targetList)

# this call makes all the targets try to guess their include and library flags
# and prepares the configuration for the build
buildConfig.createBuildEnvironments()


# generate options for the first time to 

# write a config file to be read by scons and used to build each target
buildConfig.writeConfigFile(ARGUMENTS)
buildConfig.generateOptions()
#-----------------------------------------------------------------------------
# Read the options from the config file and update the various necessary flags
# 
#
#-----------------------------------------------------------------------------

Help(buildConfig.getHelpText())

# write all dependencies generated by external modules, before exporting the dict
buildConfig.writeDependencies()
# get the dict for the compile-install phase
user_options_dict = buildConfig.getOptionsDict()

ibuilder = icgbuilder.IcgBuilder(user_options_dict, env)


print "\n"
print "============================================================"
print "=      Configuration options for "+ project +" compile       ="
print "============================================================"



#==================    BUILD SECTION    ====================================
#
# In this section, the targets are compiled using the options from 
# config.opts, which are derived themselves from those in config.user
#
#===========================================================================


ibuilder.buildAll()
ibuilder.installAll()

###########################################################################
# Done:)
###########################################################################


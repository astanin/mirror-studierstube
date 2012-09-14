#!/usr/bin/env python

import sys
import os
sys.path.append('ICGBuilder')
import icgbuilder
import buildutils


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

version      = '4.0.0'
project      = 'stb'
description  = project + ' is a modified "big" library providing an AR support'
mainlib      = 'stbkernel'
installRoot  = os.getcwd()
includeDir   = os.path.join(os.getcwd(),'include')
prefix       = ''

targetList = []

# use flags that will be shared both by static and dynamic libs.
use= {
      'stb4'          : 'true',
      'coin'          : 'true'
	}

usek= {
  'tinyxmlmod'          : 'true',	
      'stb4'          : 'true',
      'coin'          : 'true'
	}


if sys.platform == 'win32':
# list of libraries that will be searched by the scanner. The scanner will try to locate the libraries
# and the flags needed to build with those libraries. The obtained result will be used by the build, for
# each target that lists a library in its 'libs' or in its 'use' sections.
    libraryList =['ace', 'xercesc', 'tinyxmlmod', 'coin', 'stb4', 'openvideo', 'ot', 'boost']

    bootloader ={'name': 'bootloader',
                 'type': 'PRG',
                 'libs': ['ACE'],
                 'defines': ['_CONSOLE'],
                 'src_use' : ['bootloader','base']
                 }

    kernel ={'name'    :  'stbkernel',
             'type'    : 'DL',
             'use'     : use,
             'libs'    : ['ACE', 'Coin2', 'TinyXML_Mod','opengl32'],
             'defines' : ['COIN_DLL', 'KERNEL_EXPORTS'],
             'src_use' : ['kernel', 'base']
         }

    event ={'name'    : 'stbevent',
            'type'    : 'DL',
            'libs'    : ['Coin2', 'opentracker', 'boost', 'stbkernel','stbstarlight', 'openvideo'],
            'use'     : use,
            'defines' : ['COIN_DLL', 'HAVE_OPENVIDEO','EVENT_EXPORTS', 'USE_OT_2_0'],
            'src_use' : ['components/event']
            }

    starlight ={'name': 'stbstarlight',
                 'type': 'DL',
                 'libs': ['ACE', 'Coin2', 'opengl32', 'glu32', 'openvideo', 'stbkernel'],
                 'use' : use,
                'defines' : ['COIN_DLL','STARLIGHT_EXPORTS'],
                 'src_use' : ['components/starlight']
                }

    video ={'name': 'video',
            'type': 'DL',
            'libs': ['openvideo', 'stbkernel'],
            'use' : use,
            'defines' : ['COIN_DLL','VIDEO_EXPORTS'],
            'src_use' : ['components/video']
            }

    viewer_simple ={'name': 'viewer_simple',
            'type': 'DL',
            'libs': ['Coin2', 'SoSimple', 'opengl32', 'glu32', 'openvideo', 'stbkernel'],
            'use' : use,
            'defines' : ['COIN_DLL', 'SIMAGE_DLL','USE_SOSIMPLE', 'VIEWER_EXPORTS', 'HAVE_OPENVIDEO'],
            'incpath' : ['src/gui'], 
            'src_use' : ['components/viewer']
            }

    viewer_win ={'name': 'stbviewer_win',
            'type': 'DL',
            'libs': ['Coin2', 'SoWin1', 'opengl32', 'glu32', 'openvideo', 'stbkernel'],
            'use' : use,
            'defines' : ['COIN_DLL', 'SIMAGE_DLL','USE_SOWIN', 'VIEWER_EXPORTS', 'HAVE_OPENVIDEO'],
            'src_use' : ['components/viewer']
            }

    sosimple ={'name': 'SoSimple',
               'type': 'DL',
               'libs': ['Coin2', 'opengl32', 'glu32'],
               'use' : use,
               'incpath':  ['src/gui'],
               'defines' : ['COIN_DLL'],
               'src_use' : ['gui']
               }

    
    targetList.append(bootloader)
    targetList.append(kernel)
    targetList.append(event)
    targetList.append(starlight)
    targetList.append(video)
    targetList.append(viewer_simple)
    targetList.append(viewer_win)
    targetList.append(sosimple)

elif sys.platform == 'linux' or sys.platform == 'linux2' or sys.platform == 'darwin':
    # list of libraries that will be searched by the scanner. The scanner will try to locate the libraries
    # and the flags needed to build with those libraries. The obtained result will be used by the build, for
    # each target that lists a library in its 'libs' or in its 'use' sections.
    libraryList =['ACE', 'xercesc', 'tinyxmlmod', 'ot', 'openvideo', 'qt-mt', 'soqt', 'coin']


    kernel ={'name': 'stbkernel',
             'type': 'DL',
             'libs': ['ACE', 'TinyXML_Mod', 'Coin'],
             'use' : usek,
             'src_use' : ['kernel']
         }


    bootloader ={'name': 'bootloader',
                 'type': 'PRG',
                 'libs': ['ACE'],
                 'use' : use,
                 'src_use' : ['bootloader', 'base']
                 }
    
    starlight ={'name': 'stbstarlight',
                 'type': 'DL',
                 'libs': ['ACE', 'Coin'],
                 'use' : use,
                 'src_use' : ['components/starlight']
                }
    
    event ={'name': 'stbevent',
                'type': 'DL',
                'libs': ['ACE', 'Coin', 'opentracker', 'boost', 'stbstarlight'],
                'use' : use,
                'src_use' : ['components/event']
                }

    viewer ={'name': 'stbviewer_qt',
                'type': 'DL',
                'libs': ['ACE', 'Coin', 'SoQt', 'openvideo', 'qt-mt'],
	        'defines': ['USE_SOQT'],
                'use' : use,
                'src_use' : ['components/viewer']
                }

    
    if sys.platform == 'darwin':
        starlight['defines'] = ['COIN_LDFLAGS', 'dynamic_lookup']
    else:
        starlight['defines'] = ['COIN_LDFLAGS']

    targetList.append(kernel)
    targetList.append(bootloader)



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

config_file='config.opts'
	
print 'CONFIGURE: Searching for installed libraries'
# to get the include path for all libraries we need to retrieve 
# the environment vars pointing to the root of each lib
envvars = os.environ


targets = []
# create the builder with an empty target list
buildConfig = icgbuilder.ConfigBuilder(project, scannerType, envvars,
				       targets, libraryList)

# add extra configuration flags
buildConfig.desc     = description
buildConfig.mainlib  = mainlib
buildConfig.version  = version
buildConfig.prefix   = prefix
buildConfig.buildDir = buildDir
buildConfig.srcDir   = srcDir

# setup the default build flags in the configurator
buildConfig.setDefaultBuildFlags(defaultBuildFlags)

# add main include directory to all configurations
buildConfig.setIncDir(includeDir)

# create the build targets
buildConfig.setTargetList(targetList)

# this call makes all the targets try to guess their include and library flags
# and prepares the configuration for the build
buildConfig.createBuildEnvironments()

# write a config file to be read by scons and used to build each target
buildConfig.writeConfigFile(config_file, ARGUMENTS)

#-----------------------------------------------------------------------------
# Read the options from the config file and update the various necessary flags
# 
#
#-----------------------------------------------------------------------------

Help(buildConfig.getHelpText())
user_options_dict = buildConfig.getOptionsDict()
env = Environment (ENV = os.environ)
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


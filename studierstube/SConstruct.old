#!/usr/bin/env python
import os
import sys
import buildutils
import re
import string

# Studierstube version.
version='4.0'

ace_env = Environment (ENV = os.environ)
tinyxml_env = Environment (ENV = os.environ)
tinyxmlmod_env = Environment (ENV = os.environ)
coin_env = Environment (ENV = os.environ)
qt_env = Environment (ENV = os.environ)
soqt_env = Environment (ENV = os.environ)
opentracker_env = Environment (ENV = os.environ)
openvideo_env = Environment (ENV = os.environ)
muddleware_env = Environment (ENV = os.environ)
env = Environment (ENV = os.environ)
env.CacheDir(os.environ['HOME'] + '/.scache')

if sys.platform.startswith('linux') or sys.platform == 'darwin':
    print "System Platform: " + sys.platform
    config_file       = 'config.opts'
    root_build_dir    = 'build/linux'
    prefix            = '/usr/local'
    if sys.platform == 'darwin':
	root_build_dir 	= 'build/darwin'
	prefix 		= '/opt/local'
    libdir            = prefix + '/lib'
    install_root_dir  = ''
    
    use_soqt = 'true'
    release_flags = ['-O2']
    debug_flags = ['-O2', '-g']
    extra_flags = ['-pipe']
    if sys.platform == 'darwin':
	cxxflags = ['-I/opt/local/include']
    else:
	cxxflags = []
    defines = []
    warn_flags = ['-Wall']
    source_filter = '\.cxx$|\.cpp$'
    include_filter = '\.h$'
    ignore_filter = '/ignore/|/ignore$|\.scon|\.svn|\.#|bak|~$|SConscript'

    # OpenGL library information
    opengl_lib = ['GL', 'GLU']
    opengl_libpath = ['/usr/lib', '/usr/X11R6/lib']
    opengl_include = ['/usr/include', '/usr/X11R6/include']
    # ACE library information
    ace_env.ParseConfig ('pkg-config --silence-errors --cflags --libs ACE')
    ace_cflags = ace_env.Dictionary()['CCFLAGS']
    ace_include = ace_env.Dictionary()['CPPPATH']
    ace_lib = ace_env.Dictionary()['LIBS']
    ace_libpath = ace_env.Dictionary()['LIBPATH']
    # TinyXML library information
    if os.environ.has_key('TINYXMLROOT'):
	tinyxml_env['ENV']['PKG_CONFIG_PATH'] = os.environ['TINYXMLROOT'] + '/lib/pkgconfig'
	tinyxml_env.Append(PKG_CONFIG_PATH = os.environ['TINYXMLROOT'] + '/lib/pkgconfig')
    try:
        tinyxml_env.ParseConfig ('pkg-config --silence-errors --cflags --libs tinyxml')
        tinyxml_cflags = tinyxml_env.Dictionary()['CCFLAGS']
        tinyxml_include = tinyxml_env.Dictionary()['CPPPATH']
        tinyxml_lib = tinyxml_env.Dictionary()['LIBS']
        tinyxml_libpath = tinyxml_env.Dictionary()['LIBPATH']
    except OSError:
        print "Unable to obtain package information for TinyXML"
        tinyxml_lib = []
    # TinyXMLMod library information
    if os.environ.has_key('TINYXMLMODROOT'):
	tinyxmlmod_env['ENV']['PKG_CONFIG_PATH'] = os.environ['TINYXMLMODROOT'] + '/lib/pkgconfig'
	tinyxmlmod_env.Append(PKG_CONFIG_PATH = os.environ['TINYXMLMODROOT'] + '/lib/pkgconfig')
    tinyxmlmod_env.ParseConfig('pkg-config --silence-errors --cflags --libs TinyXMLMod')
    tinyxmlmod_cflags = tinyxmlmod_env.Dictionary()['CCFLAGS']
    tinyxmlmod_include = tinyxmlmod_env.Dictionary()['CPPPATH']
    tinyxmlmod_lib = tinyxmlmod_env.Dictionary()['LIBS']
    tinyxmlmod_libpath = tinyxmlmod_env.Dictionary()['LIBPATH']
    if tinyxmlmod_lib == [] and tinyxml_lib == []:
	print "Need either TinyXML or TinyXMLMod - none found"	
	#print tinyxmlmod_env['ENV']['PKG_CONFIG_PATH']
	#print tinyxmlmod_env['ENV']['PATH']
	#print tinyxmlmod_libpath
	tinyxmlmod_libpath = '/opt/local/lib'
	tinyxmlmod_lib = '-ltinyxmlmod'
	use_tinyxmlmod = 'true'
    elif tinyxml_lib == []:
	print "INFO: Only have modified TinyXML."
        use_tinyxmlmod = 'true'
    elif tinyxmlmod_lib ==[]:
	print "INFO: Only have original TinyXML."
	use_tinyxmlmod = 'false'
    if (tinyxml_lib != [] and tinyxmlmod_lib != []):
        print "INFO: Both original and modified TinyXML packages are present."
        print "INFO: Taking modified version..."
        use_tinyxmlmod = 'true'

    # Coin library information
    coin_env.ParseConfig ('coin-config --cppflags --ldflags --libs ')
    coin_cflags = coin_env.Dictionary()['CCFLAGS']
    coin_include = coin_env.Dictionary()['CPPPATH']
    coin_lib = coin_env.Dictionary()['LIBS']
    coin_libpath = coin_env.Dictionary()['LIBPATH']
    coin_ldflags = []
    if sys.platform == 'darwin':
	coin_include = [string.strip(os.popen('coin-config --includedir').read())]
	coin_ldflags = Split(string.strip(os.popen('coin-config --ldflags').read()))
    #print coin_cflags
    #print coin_include
    #print coin_ldflags
    #print coin_lib
    #print coin_libpath
    #print coin_env.Dictionary()
    #Exit(1)
    # QT library information
    qt_env.ParseConfig ('pkg-config --silence-errors --cflags --libs qt-mt')
    qt_cflags = qt_env.Dictionary()['CCFLAGS']
    qt_include = qt_env.Dictionary()['CPPPATH']
    qt_lib = qt_env.Dictionary()['LIBS']
    qt_libpath = qt_env.Dictionary()['LIBPATH']
    # Soqt library information
    soqt_env.Append(PKG_CONFIG_PATH = '/opt/local/lib/pkgconfig')
    soqt_env.ParseConfig ('soqt-config --cppflags --ldflags --libs')
    soqt_cflags = soqt_env.Dictionary()['CCFLAGS']
    soqt_include = soqt_env.Dictionary()['CPPPATH']
    soqt_lib = soqt_env.Dictionary()['LIBS']
    soqt_libpath = soqt_env.Dictionary()['LIBPATH']
    soqt_ldflags = soqt_env.Dictionary()['LINKFLAGS']
    # Opentracker library information
    #print os.environ['OTROOT'] + '/lib/pkgconfig'
    ot_pkgcfgpath = ''
    if os.environ.has_key('OTROOT'):
	ot_pkgcfgpath = os.environ['OTROOT'] + '/lib/pkgconfig'
	opentracker_env['ENV']['PKG_CONFIG_PATH'] = ot_pkgcfgpath
	opentracker_env.Append(PKG_CONFIG_PATH = ot_pkgcfgpath)
    opentracker_env.ParseConfig ('PKG_CONFIG_PATH=' + ot_pkgcfgpath + ' pkg-config --silence-errors --cflags --libs ot')
    opentracker_version = os.popen('PKG_CONFIG_PATH=' + ot_pkgcfgpath + ' pkg-config --silence-errors --modversion ot').read()
    opentracker_version = opentracker_version.strip()
    if opentracker_env.Dictionary()['LIBS'] == []:
	opentracker_env.ParseConfig ('opentracker-config --cppflags --libs')
	opentracker_version = os.popen('echo `opentracker-config --version`').read()
	opentracker_version = opentracker_version.strip()

    opentracker_cflags = opentracker_env.Dictionary()['CCFLAGS']
    opentracker_include = opentracker_env.Dictionary()['CPPPATH']
    opentracker_lib = opentracker_env.Dictionary()['LIBS']
    opentracker_libpath = opentracker_env.Dictionary()['LIBPATH']
    
    if opentracker_lib == []:
	print "Could not find OpenTracker, which is mandatory!"
	Exit(1)
    
    # BOOST library information
    boost_cflags = []
    boost_include = ''
    boost_lib = []
    boost_libpath = []
    if sys.platform == 'darwin':
	boost_libpath = ['/sw/lib']
	boost_include = ['/sw/include']

    # Openvideo library information
    openvideo_env.ParseConfig ('pkg-config --silence-errors --cflags --libs OpenVideo')
    openvideo_cflags = openvideo_env.Dictionary()['CCFLAGS']
    openvideo_include = openvideo_env.Dictionary()['CPPPATH']
    openvideo_lib = openvideo_env.Dictionary()['LIBS']
    openvideo_libpath = openvideo_env.Dictionary()['LIBPATH']

    if openvideo_lib == []:
        enable_openvideo = 'false'
    else:
        enable_openvideo = 'true'
        
    # Muddleware library information
    try:
        muddleware_env.ParseConfig ('pkg-config --silence-errors --cflags --libs XMLClient')
        muddleware_cflags = muddleware_env.Dictionary()['CCFLAGS']
        muddleware_include = muddleware_env.Dictionary()['CPPPATH']
        muddleware_lib = muddleware_env.Dictionary()['LIBS']
        muddleware_libpath = muddleware_env.Dictionary()['LIBPATH']
    except OSError:
        print "Unable to obtain package information for XMLClient"

    build_example_app = 'true'
    enable_muddleware = 'false'
else:
    print "Other platforms not supported by scons!"
    exit

#-----------------------------------------------------------------------------
# User configurable options to be saved in a config file.
#-----------------------------------------------------------------------------
# Checking for an existing config file - use that one if it exists,
# otherwise create one.
my_defines = []
my_ccflags = []
my_cxxflags = ['-Iinclude']
#my_ldflags = [' -L' + root_build_dir + '/lib ']
my_ldflags = []


if os.path.exists (config_file):
	print "Using config file: " + config_file
else:
	print "Creating new config file: " + config_file
	env_dict = env.Dictionary()
	config=open (config_file, 'w')
	config.write ("# Configuration file containing user definable options.\n")
	config.write ("VERSION = %r\n"%(version))
	config.write ("BUILD_BINARY = 'release'\n")
	config.write ("BUILD_DIR = %r\n"%(root_build_dir))
	config.write ("PREFIX = %r\n"%(prefix))
	config.write ("LIBDIR = %r\n"%(libdir))
	config.write ("INSTALL_ROOT_DIR = %r\n"%(install_root_dir))
	
	config.write ("\n# Extra compiler flags can be defined here.\n")
	config.write ("DEFINES = %s\n"%(my_defines))
	config.write ("CCFLAGS = %s\n"%(my_ccflags))
	config.write ("CXXFLAGS = %s\n"%(my_cxxflags))
	config.write ("LDFLAGS = %s\n"%(my_ldflags))

        config.write ("SOURCE_FILTER = %r\n"%(source_filter))
	config.write ("INCLUDE_FILTER = %r\n"%(include_filter))
        config.write ("IGNORE_FILTER = %r\n"%(ignore_filter))
        
        #config.write ("USE_OPENAL = %r\n"%(use_openal))
        #config.write ("USE_FMOD = %r\n"%(use_fmod))

        config.write ("USE_SOQT = %r\n"%(use_soqt))
        config.write ("USE_TINYXMLMOD = %r\n"%(use_tinyxmlmod))
        
        config.write ("\n# ACE library.\n")
        config.write ("ACE_CFLAGS = %r\n"%(ace_cflags))
	config.write ("ACE_INCLUDE = %r\n"%(ace_include))
	config.write ("ACE_LIBPATH = %r\n"%(ace_libpath))
	config.write ("ACE_LIBRARY = %r\n"%(ace_lib))

        config.write ("\n# TinyXML library.\n")
        try:
            config.write ("TINYXML_CFLAGS = %r\n"%(tinyxml_cflags))
            config.write ("TINYXML_INCLUDE = %r\n"%(tinyxml_include))
            config.write ("TINYXML_LIBPATH = %r\n"%(tinyxml_libpath))
            config.write ("TINYXML_LIBRARY = %r\n"%(tinyxml_lib))
        except NameError:
            pass

        config.write ("\n# TinyXMLMod library.\n")
        config.write ("TINYXMLMOD_CFLAGS = %r\n"%(tinyxmlmod_cflags))
	config.write ("TINYXMLMOD_INCLUDE = %r\n"%(tinyxmlmod_include))
	config.write ("TINYXMLMOD_LIBPATH = %r\n"%(tinyxmlmod_libpath))
	config.write ("TINYXMLMOD_LIBRARY = %r\n"%(tinyxmlmod_lib))

        config.write ("\n# Coin library.\n")
        config.write ("COIN_CFLAGS = %r\n"%(coin_cflags))
	config.write ("COIN_INCLUDE = %r\n"%(coin_include))
	config.write ("COIN_LDFLAGS = %r\n"%(coin_ldflags))
	config.write ("COIN_LIBPATH = %r\n"%(coin_libpath))
	config.write ("COIN_LIBRARY = %r\n"%(coin_lib))

        config.write ("\n# QT library.\n")
        config.write ("QT_CFLAGS = %r\n"%(qt_cflags))
	config.write ("QT_INCLUDE = %r\n"%(qt_include))
	config.write ("QT_LIBPATH = %r\n"%(qt_libpath))
	config.write ("QT_LIBRARY = %r\n"%(qt_lib))

        config.write ("\n# Soqt library.\n")
        config.write ("SOQT_CFLAGS = %r\n"%(soqt_cflags))
	config.write ("SOQT_INCLUDE = %r\n"%(soqt_include))
	config.write ("SOQT_LIBPATH = %r\n"%(soqt_libpath))
	config.write ("SOQT_LIBRARY = %r\n"%(soqt_lib))
	config.write ("SOQT_LDFLAGS = %r\n"%(soqt_ldflags))

        config.write ("\n# OpenTracker library.\n")
        config.write ("OPENTRACKER_CFLAGS = %r\n"%(opentracker_cflags))
	config.write ("OPENTRACKER_INCLUDE = %r\n"%(opentracker_include))
	config.write ("OPENTRACKER_LIBPATH = %r\n"%(opentracker_libpath))
	config.write ("OPENTRACKER_LIBRARY = %r\n"%(opentracker_lib))
        
	config.write ("\n# Boost library.\n")
        config.write ("BOOST_CFLAGS = %r\n"%(boost_cflags))
	config.write ("BOOST_INCLUDE = %r\n"%(boost_include))
	config.write ("BOOST_LIBPATH = %r\n"%(boost_libpath))
	config.write ("BOOST_LIBRARY = %r\n"%(boost_lib))

        config.write ("\n# OpenVideo library.\n")
        config.write ("OPENVIDEO_CFLAGS = %r\n"%(openvideo_cflags))
	config.write ("OPENVIDEO_INCLUDE = %r\n"%(openvideo_include))
	config.write ("OPENVIDEO_LIBPATH = %r\n"%(openvideo_libpath))
	config.write ("OPENVIDEO_LIBRARY = %r\n"%(openvideo_lib))

        config.write ("\n# Muddleware library.\n")
        try:
            config.write ("MUDDLEWARE_CFLAGS = %r\n"%(muddleware_cflags))
            config.write ("MUDDLEWARE_INCLUDE = %r\n"%(muddleware_include))
            config.write ("MUDDLEWARE_LIBPATH = %r\n"%(muddleware_libpath))
            config.write ("MUDDLEWARE_LIBRARY = %r\n"%(muddleware_lib))
        except NameError:
            pass

	config.write ("OPENGL_INCLUDE = %r\n"%(opengl_include))
	config.write ("OPENGL_LIBPATH = %r\n"%(opengl_libpath))
	config.write ("OPENGL_LIBRARY = %r\n"%(opengl_lib))

        config.write ("BUILD_EXAMPLE_APP = %r\n"%(build_example_app))
        config.write ("ENABLE_OPENVIDEO = %r\n"%(enable_openvideo))
        config.write ("ENABLE_MUDDLEWARE = %r\n"%(enable_muddleware))
        config.close ()

#-----------------------------------------------------------------------------
# Read the options from the config file and update the various necessary flags
#-----------------------------------------------------------------------------
list_opts = []
user_options = Options (config_file, ARGUMENTS)
user_options_env = Environment (ENV = os.environ, options = user_options)
user_options.AddOptions (
		('VERSION', 'Studierstube version', version),
		(EnumOption ('BUILD_BINARY', 'release',
					'Select a release or debug binary.',
					 allowed_values = ('release', 'debug'))),
		('BUILD_DIR', 'Target directory for intermediate files.',
					root_build_dir),
		('PREFIX', 'Target directory for installed files.',
					prefix),
		('LIBDIR', 'Target directory for library files.',
					libdir),
		('INSTALL_ROOT_DIR', 'Target directory for building packages.',
					install_root_dir),
		#(BoolOption ('USE_OPENAL',
		#			'Set to 1 to build the game engine with OpenAL support.',
		#			'false')),
		#(BoolOption ('USE_FMOD',
		#			'Set to 1 to build the game engine with FMod support.',
		#			'false')),
		(BoolOption ('USE_SOQT',
					'Set to 1 to use SoQt (default).',
					'true')),
		(BoolOption ('USE_TINYXMLMOD',
					'Set to 1 to use TinyXMLMod instead of TinyXML.',
					'false')),
		('ACE_CFLAGS', 'Necessary CFLAGS when using ACE functionality.'),
		('ACE_INCLUDE', 'Include directory for ACE header files.'),
		('ACE_LIBPATH', 'Library path where the ACE library is located.'),
		('ACE_LIBRARY', 'ACE library name.'),
		('COIN_CFLAGS', 'Necessary CFLAGS when using COIN functionality.'),
		('COIN_INCLUDE', 'Include directory for COIN header files.'),
		('COIN_LDFLAGS', 'Options for linking with COIN.'),
		('COIN_LIBPATH', 'Library path where the COIN library is located.'),
		('COIN_LIBRARY', 'COIN library name.'),
		('QT_CFLAGS', 'Necessary CFLAGS when using QT functionality.'),
		('QT_INCLUDE', 'Include directory for QT header files.'),
		('QT_LIBPATH', 'Library path where the QT library is located.'),
		('QT_LIBRARY', 'QT library name.'),
		('SOQT_CFLAGS', 'Necessary CFLAGS when using SOQT functionality.'),
		('SOQT_INCLUDE', 'Include directory for SOQT header files.'),
		('SOQT_LIBPATH', 'Library path where the SOQT library is located.'),
		('SOQT_LIBRARY', 'SOQT library name.'),
		('SOQT_LDFLAGS', 'SOQT library linking flags.'),
		('OPENTRACKER_CFLAGS', 'Necessary CFLAGS when using OPENTRACKER functionality.'),
		('OPENTRACKER_INCLUDE', 'Include directory for OPENTRACKER header files.'),
		('OPENTRACKER_LIBPATH', 'Library path where the OPENTRACKER library is located.'),
		('OPENTRACKER_LIBRARY', 'OPENTRACKER library name.'),
		('BOOST_CFLAGS', 'Necessary CFLAGS when using BOOST functionality.'),
		('BOOST_INCLUDE', 'Include directory for BOOST header files.'),
		('BOOST_LIBPATH', 'Library path where the BOOST library is located.'),
		('BOOST_LIBRARY', 'BOOST library name.'),
		('OPENVIDEO_CFLAGS', 'Necessary CFLAGS when using OPENVIDEO functionality.'),
		('OPENVIDEO_INCLUDE', 'Include directory for OPENVIDEO header files.'),
		('OPENVIDEO_LIBPATH', 'Library path where the OPENVIDEO library is located.'),
		('OPENVIDEO_LIBRARY', 'OPENVIDEO library name.'),
		('MUDDLEWARE_CFLAGS', 'Necessary CFLAGS when using MUDDLEWARE functionality.'),
		('MUDDLEWARE_INCLUDE', 'Include directory for MUDDLEWARE header files.'),
		('MUDDLEWARE_LIBPATH', 'Library path where the MUDDLEWARE library is located.'),
		('MUDDLEWARE_LIBRARY', 'MUDDLEWARE library name.'),
		('TINYXML_CFLAGS', 'Necessary CFLAGS when using TINYXML functionality.'),
		('TINYXML_INCLUDE', 'Include directory for TINYXML header files.'),
		('TINYXML_LIBPATH', 'Library path where the TINYXML library is located.'),
		('TINYXML_LIBRARY', 'TINYXML library name.'),
		('TINYXMLMOD_CFLAGS', 'Necessary CFLAGS when using TINYXMLMOD functionality.'),
		('TINYXMLMOD_INCLUDE', 'Include directory for TINYXMLMOD header files.'),
		('TINYXMLMOD_LIBPATH', 'Library path where the TINYXMLMOD library is located.'),
		('TINYXMLMOD_LIBRARY', 'TINYXML library name.'),
		('DEFINES', 'Extra Preprocessor defines.'),
		('CCFLAGS', 'Extra C Compiler flags.'),
		('CXXFLAGS','Extra C++ Compiler flags.'),
		('LDFLAGS', 'Extra Linker flags.'),
                ('SOURCE_FILTER', 'Specify the source filter for compiling sources'),
                ('INCLUDE_FILTER', 'Specify the header files which should be considered'),
                ('IGNORE_FILTER', 'Specify the ignore filter for excluding source files'),
		(BoolOption ('BUILD_EXAMPLE_APP',
					'Set to 1 to build the example application.',
					'true')),
		(BoolOption ('ENABLE_OPENVIDEO',
					'Set to 1 to use the OpenVideo library.',
					'false')),
		(BoolOption ('ENABLE_MUDDLEWARE',
					'Set to 1 to use the Muddleware library.',
					'false'))
	)
user_options.Update (user_options_env)
user_options.Save('config.opts', user_options_env)
user_options_dict = user_options_env.Dictionary()
Help(user_options.GenerateHelpText(user_options_env))
root_build_dir = user_options_dict['BUILD_DIR']

print "\n"
print "============================================================"
print "=      Configuration options for Studierstube compile      ="
print "============================================================"


# ############################################################
# OpenTracker

print "OpenTracker version ... " + opentracker_version
ot11_re = re.compile('^1\.1.*')
ot12_re = re.compile('^1\.2.*')
ot13_re = re.compile('^1\.3.*')
ot20_re = re.compile('^2\.0.*')
if ot11_re.match(opentracker_version):
    defines += ['USE_OT_1_1']
elif ot12_re.match(opentracker_version):
    defines += ['USE_OT_1_2']
elif ot13_re.match(opentracker_version):
    defines += ['USE_OT_1_3']
elif ot20_re.match(opentracker_version):
    defines += ['USE_OT_2_0']
else:
    print "WARNING, OpenTracker version not supported!"


# ############################################################
# Muddleware

if user_options_dict['ENABLE_MUDDLEWARE'] == 1:
    print "Muddleware support ... yes"
    defines += ['ENABLE_MUDDLEWARE']
else:
    print "Muddleware support ... no"

if user_options_dict['USE_SOQT'] == 1:
    defines += ['USE_SOQT']

if user_options_dict['USE_TINYXMLMOD'] == 1:
    defines += ['USE_TINYXMLMOD']

# ############################################################
# OpenVideo

if user_options_dict['ENABLE_OPENVIDEO'] == 1:
    defines += ['HAVE_OPENVIDEO']
    print "OpenVideo support ... yes"
else:
    print "OpenVideo support ... no"

# ############################################################
# Debug/Release

if user_options_dict['BUILD_BINARY'] == 'release':
	cflags = extra_flags + release_flags + warn_flags
	defines += ['NDEBUG']
        print "Compile version ... release"
else:
	cflags = extra_flags + debug_flags + warn_flags
	defines += ['_DEBUG']
        print "Compile version ... debug"

print "============================================================\n"

if user_options_dict['USE_SOQT'] == 1:
    defines += ['USE_SOQT']

defines += user_options_dict['DEFINES']
defines += ['LINUX']

cflags += user_options_dict['CCFLAGS']
cxxflags += user_options_dict['CXXFLAGS']

# Extract defines for pkg-config file
global_defs = ''
for gd in defines:
    global_defs += '-D' + gd + ' '

# Create the build directory. SCons does this automatically, but since we
# don't want to put scons-generated .sconsign files in the source tree, but in
# the root_build_dir, we have to create that dir ourselves before SCons tries
# to access/create the file containing .sconsign data.
if os.path.isdir (root_build_dir) == 0:
	os.makedirs (root_build_dir)
	os.makedirs (root_build_dir+os.sep+'src')

#****************************************************************************
# Set project details used in the package-config (.pc) file
#****************************************************************************
env['STB_PROJECT_NAME']        = "Studierstube"
env['STB_PROJECT_DESCRIPTION'] = "Studierstube - The Augmented Reality Environment"
env['STB_PROJECT_VERSION']     = "4.0"
env['STB_PROJECT_LIBNAME']     = "stbkernel"
env['STB_PREFIX']              = user_options_dict['PREFIX']
env['STB_LIBDIR']              = user_options_dict['LIBDIR']
env['STB_PROJECT_DEFINES']     = global_defs

install_root_dir = user_options_dict['INSTALL_ROOT_DIR'] + os.sep

buildutils.appendbuilders(env)
outname = env.AlwaysBuild(env.Substitute('stb.pc', 'stb.pc.in'))
ic = env.Alias(target = ["install-config"], source = env.AlwaysBuild(env.Install(dir = install_root_dir + user_options_dict['LIBDIR'] + '/pkgconfig', source = outname)))
il = env.Alias('install-lib', install_root_dir + user_options_dict['LIBDIR'])
ib = env.Alias('install-bin', install_root_dir + user_options_dict['PREFIX'] + '/bin')
ih = env.Alias('install-header', install_root_dir + user_options_dict['PREFIX'] + '/include')
iss = env.Alias('install-share', install_root_dir + user_options_dict['PREFIX'] + '/share')
env.Alias('install', [ic, il, ib, ih, iss])

#****************************************************************************
# Specify source filters
#****************************************************************************
#env['SOURCE_FILTER']  = '\.cxx$|\.cpp$'
#env['INCLUDE_FILTER'] = '\.h$'
#env['IGNORE_FILTER']  = '/ignore/|/ignore$|\.scon|\.svn|\.#|bak|~$|SConscript'

#-----------------------------------------------------------------------------
# Generic library generation environment. This one is the basis for each
# library.
#-----------------------------------------------------------------------------
library_env = env.Copy ()
library_env.Append (CCFLAGS = cflags)
library_env.Append (CXXFLAGS = cxxflags)
library_env.Append (LIBPATH = user_options_dict['LDFLAGS'])
if sys.platform == 'darwin':
    library_env.Append (LINKFLAGS = '-undefined dynamic_lookup')
    library_env.Append (LIBPATH = '/opt/local/lib')
library_env.Append (CPPDEFINES = defines)
library_env.SConsignFile (root_build_dir+'scons-signatures')

libpath = (['#'+root_build_dir+'/lib'])
library_env.Append (LIBPATH = libpath)

#-----------------------------------------------------------------------------
# Settings to be exported to other SConscript files
#-----------------------------------------------------------------------------

Export ('cflags')
Export ('defines')
Export ('user_options_dict')
Export ('library_env')
Export ('env')

#****************************************************************************
# Build by executing SConscript file
#****************************************************************************

# Print welcoming message that makes you feel warm and fuzzy inside;)
print "\n...Building...\n"

BuildDir (root_build_dir+'/src', 'src', duplicate=0)
SConscript (root_build_dir+'/src/SConscript')
SConscript (['include/SConscript'])
SConscript (['data/SConscript'])
#SConscript (['bin/linux/SConscript'])



###########################################################################
# Done:)
###########################################################################


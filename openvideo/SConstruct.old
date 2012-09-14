#!/usr/bin/python
import os
import sys
import buildutils

#****************************************************************************
# The options are saved to a file and reapplied automatically
#****************************************************************************
opts = Options(['build.opts'], ARGUMENTS)
opts.Add('PREFIX'                               , 'Sets the project installation directory', '/usr/local')
opts.Add('SUBPREFIX'                            , 'Sets the sub-installation directory. Relative to PREFIX', '')
opts.Add('LIBDIR'                               , 'Sets the relative library installation directory. Overrides SUBPREFIX', 'lib')
opts.Add('INCLUDEDIR'                           , 'Sets the relative include files installation directory. Overrides SUBPREFIX', 'include')
opts.Add(BoolOption('ENABLE_VIDEOWRAPPERSRC'    , 'Enables the videowrapper source', 0))
opts.Add(BoolOption('ENABLE_SPECTECSRC'         , 'Enables the spectec source', 0))
opts.Add(BoolOption('ENABLE_GLUTSINK'           , 'Enables the GLUT sink', 1))
opts.Add(BoolOption('ENABLE_GL_TEXTURE_2D_SINK' , 'Enables the GL texture sink', 1))
opts.Add(BoolOption('ENABLE_TESTSRC'            , 'Enables the test source', 1))
opts.Add(BoolOption('ENABLE_V4LSRC'             , 'Enables the Video4Linux source', 0))
opts.Add(BoolOption('ENABLE_V4L2SRC'            , 'Enables the Video4Linux2 source', 0))
opts.Add(BoolOption('ENABLE_LIVE555SRC'         , 'Enables the LIVER555Src support', 0))
opts.Add(BoolOption('ENABLE_FFMPEGSRC'          , 'Enables FFmpeg decoding support', 0))
opts.Add(BoolOption('ENABLE_FFMPEGSINK'         , 'Enables FFmpeg encoding support', 0))
opts.Add(BoolOption('ENABLE_VIDEOSINK'          , 'Enables the video sink', 1))
opts.Add(BoolOption('ENABLE_OPENCV'             , 'Enables the use of OpenCV capture source', 1))
opts.Add(BoolOption('OPENVIDEO_DEBUG'           , 'Enables debugging', 0))

#****************************************************************************
# Set up environment and save options to disk
#****************************************************************************
env = Environment(options = opts, ENV = os.environ)
env.CacheDir(os.environ['HOME'] + '/.scache')
opts.Save('build.opts', env)

#****************************************************************************
# Make sure ACE is available
#****************************************************************************
conf = Configure(env)
if conf.TryAction('pkg-config --exists ACE')[0]:
    env.ParseConfig('pkg-config --cflags --libs ACE')
else:
    print "****************************************************************************"
    print "ERROR: You need to have ACE installed and ACE.pc in your PKG_CONFIG_PATH"
    print "Make sure you installed ACE using the ./configure && make && make install"
    print "process..."
    print "****************************************************************************"
    sys.exit(-1)

#****************************************************************************
# Make sure TinyXml is available
#****************************************************************************
if conf.TryAction('pkg-config --exists tinyxml')[0]:
    env.ParseConfig('pkg-config --cflags --libs tinyxml')
else:
    print "****************************************************************************"
    print "ERROR: You need to have TinyXml installed and tinyxml.pc in your PKG_CONFIG_PATH"
    print "Make sure you installed TinyXml from CVS using scons install"
    print "****************************************************************************"
    sys.exit(-1)

#****************************************************************************
# Add defines if using a set of nodes
#****************************************************************************
if env['ENABLE_VIDEOWRAPPERSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_VIDEOWRAPPERSRC'])

if env['ENABLE_SPECTECSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_SPECTECSRC'])
    
if env['ENABLE_GLUTSINK']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_GLUTSINK'])
    env.AppendUnique(LIBS = ['GL', 'GLU', 'glut'])
    
if env['ENABLE_GL_TEXTURE_2D_SINK']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_GL_TEXTURE_2D_SINK'])
    env.AppendUnique(LIBS = ['GL', 'GLU'])
    
if env['ENABLE_TESTSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_TESTSRC'])
    
if env['ENABLE_V4LSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_V4LSRC'])

if env['ENABLE_V4L2SRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_V4L2SRC'])
#    env.AppendUnique(CCFLAGS = ['-g'])

if env['ENABLE_LIVE555SRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_LIVE555SRC'])

if env['ENABLE_FFMPEGSINK']:
    env.ParseConfig('ffmpeg-config --cflags --libs avformat')
    env.AppendUnique(CPPDEFINES = ['ENABLE_FFMPEGSINK'])

if env['ENABLE_FFMPEGSRC']:
    env.ParseConfig('ffmpeg-config --cflags --libs avformat')
    env.AppendUnique(CCFLAGS = ['-g'])
    env.AppendUnique(CPPDEFINES = ['ENABLE_FFMPEGSRC'])

if env['ENABLE_VIDEOSINK']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_VIDEOSINK'])

if env['ENABLE_OPENCV']:
    if conf.TryAction('pkg-config --exists opencv')[0]:
        env.ParseConfig('pkg-config --cflags --libs opencv')
        env.AppendUnique(CPPDEFINES = ['ENABLE_OPENCV'])
    else:
        print "****************************************************************************"
        print "WARNING: You need to have OpenCV installed and opencv.pc in"
        print "your PKG_CONFIG_PATH in order for OpenCV stuff to work."
        print "****************************************************************************"
        
if env['OPENVIDEO_DEBUG']:
    env.AppendUnique(CPPDEFINES = ['OPENVIDEO_DEBUG=1'])

conf.Finish()

# Add the platform to the defines. Treat all linux platforms in common.
if sys.platform.upper().startswith('LINUX'):
    env.AppendUnique(CPPDEFINES = ['LINUX'])
else:
    env.AppendUnique(CPPDEFINES = [sys.platform.upper()])

#****************************************************************************
# Set general information
#****************************************************************************
env['OPENVIDEO_PROJECT_NAME']        = "OpenVideo"
env['OPENVIDEO_PROJECT_DESCRIPTION'] = "OpenVideo is a library which abstract video streams in a similar way as OpenTracker abstracts tracking data"
env['OPENVIDEO_PROJECT_VERSION']     = "1.0.0"
env['OPENVIDEO_PROJECT_LIBNAME']     = "openvideo"

#****************************************************************************
# Set project details used in the package-config (.pc) file
# See OpenVideo.pc.in
#****************************************************************************
pkgenv = env.Copy()
pkgenv.AppendUnique(LIBS = ['openvideo'])
pkgenv.AppendUnique(CPPPATH = [pkgenv['PREFIX'] + os.sep + pkgenv['INCLUDEDIR']])

#****************************************************************************
# Generate strings usable from the pkg-config file
#****************************************************************************
defines = ''
if pkgenv.has_key('CPPDEFINES'):
    for define in pkgenv['CPPDEFINES']:
        defines += '-D' + define + ' '

cpppaths = ''
if pkgenv.has_key('CPPPATH'):
    for path in pkgenv['CPPPATH']:
        cpppaths += '-I' + path + ' '

libpaths = ''
if pkgenv.has_key('LIBPATH'):
    for path in pkgenv['LIBPATH']:
        libpaths += '-L' + path + ' '

libs = ''
if pkgenv.has_key('LIBS'):
    for lib in pkgenv['LIBS']:
        libs += '-l' + lib + ' '

pkgenv['OPENVIDEO_PROJECT_DEFINES'] = defines
pkgenv['OPENVIDEO_PROJECT_CPPPATH'] = cpppaths
pkgenv['OPENVIDEO_PROJECT_LIBPATH'] = libpaths
pkgenv['OPENVIDEO_PROJECT_LIBS']    = libs

# Set the library install location
libsubprefix = env['SUBPREFIX'] + os.sep + 'lib'
if env['LIBDIR'] != '':
    libsubprefix = env['LIBDIR']
if not libsubprefix.startswith(os.sep):
    libsubprefix = os.sep + libsubprefix

buildutils.appendbuilders(pkgenv)
outname = pkgenv.AlwaysBuild(pkgenv.Substitute('OpenVideo.pc', 'OpenVideo.pc.in'))
env.Alias(target = ["install"], source = pkgenv.AlwaysBuild(pkgenv.Install(dir = '${PREFIX}' + libsubprefix + '/pkgconfig', source = outname)))

#****************************************************************************
# Need to specify where to look for local include files
#****************************************************************************
env.AppendUnique(CPPPATH = [Dir('#').abspath + os.sep + 'include'])
env.AppendUnique(CPPPATH = [Dir('#').abspath + os.sep + 'src'])

#****************************************************************************
# Generate help message
#****************************************************************************
Help(opts.GenerateHelpText(env))

#****************************************************************************
# Build by executing SConscript file
#****************************************************************************

# Print welcoming message that makes you feel warm and fuzzy inside;)
print "\n...Building...\n"

# Call subsidiary build scripts.
env['BUILDDIR'] = 'build' + os.sep + sys.platform
SConscript(['src/SConscript'], exports='env', build_dir=env['BUILDDIR'], duplicate=1)

###########################################################################
# Done:)
###########################################################################

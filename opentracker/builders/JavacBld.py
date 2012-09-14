import os, re
import os.path
import string

import SCons.Action
import SCons.Builder
from SCons.Node.FS import _my_normcase
from SCons.Tool.JavaCommon import parse_java_file
import SCons.Util

def stringify_list(_list, separator=''):
    _string = ''
    for l in _list:
        _string += str(l) + separator
    return _string[:-1]

def classname(path):
    """Turn a string (path name) into a Java class name."""
    return string.replace(os.path.normpath(path), os.sep, '.')

def subtractDirectories(subtractee, mask):
    if mask == '.' or mask == './' or mask == '':
        return subtractee[:-5] # i.e. do nothing except remove '.java' suffix
    else:
        pattern = mask + os.sep + r'([\w+' + os.sep + ']*)'
        re_mask = re.compile(pattern)
        iterator = re_mask.finditer(subtractee)
        result = iterator.next().group(1)
        return result

def emit_java_classes(target, source, env):
#    raw_input("emit_java_classes")
    """Create and return lists of source java files
    and their corresponding target class files.
    """
    java_suffix = env.get('JAVASUFFIX', '.java')
    class_suffix = env.get('JAVACLASSSUFFIX', '.class')
    install_dir  = env['CLASSINSTALLDIR']
    source_path  = env['SOURCEDIR']
    tlist = []
    for f in source:
	try:
	    pkg_dir, classes = parse_java_file(os.path.abspath(str(f)))
	    if pkg_dir:
		tlist += [f.Dir(os.path.join(install_dir, pkg_dir)).File(c+class_suffix) for c in classes]
	    elif classes:
		tlist += [f.Dir(os.path.join(install_dir, os.path.dirname(str(f)))).File(env['ESCAPE'](c+class_suffix)) for c in classes]
	    else:
		# This is an odd end case:  no package and no classes.
		# Just do our best based on the source file name.
		base = str(f)[:-len(java_suffix)]
		tlist += [f.Dir(os.path.join(install_dir, os.path.dirname(str(f)))).File(base+class_suffix)]
	except IOError:
	    # File doesn't exist and therefore can't be parsed. Bummer.
	    # Have to make do with a best guess
	    tlist += [os.path.join(install_dir, str(f)[len(source_path)+1:-len(java_suffix)]) + class_suffix]
    slist = [file for file in source]
    return tlist, slist

def generate_javac_actions(source, target, env, for_signature):
    listCmd = []
    sources   = stringify_list([file.get_string(for_signature) for file in source], ' ')
    classpath = stringify_list(env['CLASSPATH'],':')
    try:
        command = 'javac -d %s -classpath %s -sourcepath %s %s' % \
            (env['CLASSINSTALLDIR'], classpath, env['SOURCEDIR'], sources)
    except KeyError:
        command = 'javac -d %s -classpath %s %s' % (env['CLASSINSTALLDIR'], classpath, sources)
    listCmd += [command]
    return listCmd

JavaAlternativeBuilder = SCons.Builder.Builder(generator = generate_javac_actions,
                                               emitter = emit_java_classes,
                                               target_factory = SCons.Node.FS.File,
                                               source_factory = SCons.Node.FS.File)

def generate(env):
    """Add Builders and construction variables for javac to an Environment."""
    env['BUILDERS']['JavacBld'] = JavaAlternativeBuilder
    env['CLASSPATH'] = ['.']
    env['JAVAC']            = 'javac'
    env['JAVACFLAGS']       = SCons.Util.CLVar('')
    #env['JAVACCOM']         = '$JAVAC $JAVACFLAGS -d ${TARGET.attributes.java_classdir} -sourcepath ${SOURCE.dir.rdir()} $SOURCES'
    #env['JAVACCOM']         = '$JAVAC $JAVACFLAGS -sourcepath $SOURCEPATH $SOURCES'
    env['JAVACLASSSUFFIX']  = '.class'
    env['JAVASUFFIX']       = '.java'

    
def exists(env):
    return env.Detect('javac')

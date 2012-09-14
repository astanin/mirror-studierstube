import os.path
import SCons.Builder
import SCons.Node.FS
import SCons.Util

csccom = "$CSC $CSCFLAGS -out:${TARGET.abspath} $SOURCES"
csclibcom = "$CSC -t:library $CSCLIBFLAGS $_CSCLIBPATH $_CSCLIBS -out:${TARGET.abspath} $SOURCES"

def generate_mcs_action(source, target, env, for_signature):
    cs_files  = [_file for _file in source if str(_file).endswith('.cs')]
    dll_files =  [_file for _file in source if str(_file).endswith('.dll')]
    references = '-r:%s '*len(dll_files) % tuple(dll_files)
    command = '$CSC ' + references + \
	' -out:${TARGET.abspath} ' + \
	"%s "*len(cs_files) % tuple(cs_files)
    return command

McsBuilder = SCons.Builder.Builder(generator = generate_mcs_action,
                                   source_factory = SCons.Node.FS.default_fs.Entry,
				   src_suffix = '.cs',
                                   suffix = '.exe')

McsLibBuilder = SCons.Builder.Builder(action = '$CSCLIBCOM',
                                   source_factory = SCons.Node.FS.default_fs.Entry,
                                   suffix = '.dll')

def generate(env):
    env['BUILDERS']['CLIProgram'] = McsBuilder
    env['BUILDERS']['CLILibrary'] = McsLibBuilder

    env['CSC']        = 'mcs'
    env['_CSCLIBS']    = "${_stripixes('-r:', CILLIBS, '', '-r', '', __env__)}"
    env['_CSCLIBPATH'] = "${_stripixes('-lib:', CILLIBPATH, '', '-r', '', __env__)}"
    env['CSCFLAGS']   = SCons.Util.CLVar('')
    env['CSCCOM']     = SCons.Action.Action(csccom)
    env['CSCLIBCOM']  = SCons.Action.Action(csclibcom)

def exists(env):
    return internal_zip or env.Detect('gmcs')

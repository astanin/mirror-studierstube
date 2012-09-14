from os.path import join, dirname, basename, splitext
from os import environ

import SCons.Node.FS
import SCons.Environment
import SCons.Builder
import SCons.Scanner

#def clsidl_emitter(target, source, env):
#    return join(env['MONO_LIB_INSTALL'], env['CLS_ASSEMBLY']+'.dll'), source

def generate_clsidl_actions(source, target, env, for_signature):
    idl_files = "%s "*len(source) % tuple(source)
    output_directory = dirname(str(target[0]))
    assembly_file = basename(str(target[0]))
    assembly_stem = splitext(assembly_file)[0]
    command = 'mono %s -o %s %s %s' % (join(environ['IIOPNET_HOME'], 'IDLToCLSCompiler', 'IDLCompiler', 'bin', 'IDLToCLSCompiler.exe'), output_directory, assembly_stem, idl_files)
    return command

def generate(env):
    env['BUILDERS']['IDL2CLS'] = \
        SCons.Builder.Builder(generator = generate_clsidl_actions,
                              src_suffix = '.idl',
                              suffix = '.dll')#,
#                              emitter=clsidl_emitter)

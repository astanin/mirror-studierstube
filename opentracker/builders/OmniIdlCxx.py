import os, re
from itertools import izip, count

import SCons.Node.FS
import SCons.Environment
import SCons.Builder
import SCons.Scanner

def omniidl_emitter(target, source, env):
    tlist, slist = [], source # source list is unchanged
    for src in source:
        trgs = []
        idl_file = os.path.basename(str(src))
        stem = idl_file[:-4]
        tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], stem + 'SK.cc'))        
        tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], stem + 'DynSK.cc'))
        tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], stem + '.hh'))
    #tlist = [os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], idl) for idl in tlist]
    return tlist, slist

def filterOutComments(contents):
    return re.sub(r'(//[^\n]*)', '', contents)

def generate_omniidl_actions(source, target, env, for_signature):
    include_args = ''
    listCmd = []
    for directory in env['CPPPATH']:
        include_args += "-I%s " % (directory)
    defines = "-D%s "*len(env['CPPDEFINES']) % tuple(env['CPPDEFINES'])
    for src in source:
        idl_file = src.get_string(for_signature)
        command = 'omniidl -bcxx -Wba -Wbtp %s %s -C %s %s' % (defines, include_args, env['OMNIIDL_INSTALL_DIRECTORY'], idl_file)
        listCmd.append(command)
    return listCmd

def generate(env):
    env['OMNIIDL_INSTALL_DIRECTORY'] = '.'
    #idlscan = SCons.Scanner.Scanner(function = idlfile_scan,
                                    #skeys = ['.idl'])
    #env.Append(SCANNERS = idlscan)
    try:
        if '.' not in env['CPPPATH']:
            env.Append(CPPPATH = '.')
    except KeyError:
        env.Append(CPPPATH = '.')
    env['BUILDERS']['OMNIIDLCXX'] = \
        SCons.Builder.Builder(generator = generate_omniidl_actions,
                              src_suffix = '.idl',
                              emitter=omniidl_emitter)
        
def test():
    env = SCons.Environment.Environment()
    generate(env)
    target, source = idlj_emitter([], [env.File('graph.idl')], env)

import os, re
from itertools import izip, count

import SCons.Node.FS
import SCons.Environment
import SCons.Builder
import SCons.Scanner

#import _omniidl
#from omniidl.main import *
import popen2, re
modules_re = re.compile(r'_exported_modules\s\=\s\(\s([\d\s\w\",]*)\)', re.M)

def getModules(env, idl_file):
    include_args = ''
    listCmd = []
    for directory in env['CPPPATH']:
        include_args += "-I%s " % (directory)
    try:
        defines = "-D%s "*len(env['CPPDEFINES']) % tuple(env['CPPDEFINES'])
    except KeyError:
        defines = ""
    command = 'omniidl -bpython -Wbstdout %s %s %s' % (defines, include_args, idl_file)
    (output, input) = popen2.popen4(command)
    text, t = '', None
    while t != '':
        t = output.read()
        text += t
    output.close()
    input.close()
    try:
        recovered_modules = modules_re.findall(text)[0]
        parsed_modules = recovered_modules.replace('"','').replace(' ', '').split(',')
        parsed_modules.remove('')
        print "modules extracted from idl file " + str(idl_file) + " = ", parsed_modules
        return parsed_modules
    except IndexError:
        print "could not extract any modules from idl file", str(idl_file)
        return []

def omniidl_emitter(target, source, env):
    print "omniidl_emitter start"
    tlist, slist = [], source # source list is unchanged
    for src in source:
        trgs = []
        idl_file = str(src)
        include_args = ['-I' + directory for directory in env['CPPPATH']]
        print include_args + [idl_file]
        #idl = parseOmniIdlArgs(include_args + [idl_file])
        #modules = [instance for instance in idl.declarations() if (instance.__class__==idlast.Module) and (instance.file()==idl_file)]
        modules = getModules(env, idl_file)
        tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], os.path.basename(idl_file)[:-4] + '_idl.py'))
        for module_name in modules:
            #module_name = module.scopedName()[0]
            tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], module_name, '__init__.py'))
            tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], module_name + '__POA', '__init__.py'))
        #idlast.clear()
        #idltype.clear()
        #_omniidl.clear()
    print "omniidl_emitter end"
    return tlist, slist

def filterOutComments(contents):
    return re.sub(r'(//[^\n]*)', '', contents)


########################################################################
#                                                                      #
# Build problem is here with the "for_signature" argument              #
# Need to generate a Build Signature                                   #
########################################################################
def generate_omniidl_actions(source, target, env, for_signature):
    include_args = ''
    listCmd = []
    for directory in env['CPPPATH']:
        include_args += "-I%s " % (directory)
    try:
        defines = "-D%s "*len(env['CPPDEFINES']) % tuple(env['CPPDEFINES'])
    except KeyError:
        defines = ""
    for src in source:
        idl_file = src.get_string(for_signature)
        command = 'omniidl -bpython %s %s -C %s %s' % (defines, include_args, env['OMNIIDL_INSTALL_DIRECTORY'], idl_file)
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
    env['BUILDERS']['OMNIIDLPY'] = \
        SCons.Builder.Builder(generator = generate_omniidl_actions,
                              src_suffix = '.idl',
                              suffix = '.py',
                              emitter=omniidl_emitter)
        
def test():
    env = SCons.Environment.Environment()
    generate(env)
    target, source = idlj_emitter([], [env.File('graph.idl')], env)

import os, re
from itertools import izip, count

import SCons.Node.FS
import SCons.Environment
import SCons.Builder
import SCons.Scanner

import _omniidl
from omniidl.main import *

def idlj_emitter(target, source, env):
    tlist, slist = [], source # source list is unchanged
    for src in source:
        trgs = []
        idl_file = str(src)
        include_args = ['-I' + directory for directory in env['IDL_INCLUDE_PATH']]
        idl = parseOmniIdlArgs(include_args + [idl_file])
        modules = [instance for instance in idl.declarations() if (instance.__class__==idlast.Module) and (instance.file()==idl_file)]
        ffixes = [('', 'Helper.java'),
                  ('', 'Holder.java')]
        single_ffix = [('', '.java')]
        if_ffixes = [('', 'Operations.java'),
                  ('', 'POA.java'),
                  ('', 'POATie.java'),
                  ('_', 'Stub.java')]
        for module in modules:
            module_name = module.scopedName()[0]
            typedefs = [definition for definition in module.definitions() if definition.__class__==idlast.Typedef]
            interfaces = [definition for definition in module.definitions() if definition.__class__==idlast.Interface]
            enums = [definition for definition in module.definitions() if definition.__class__==idlast.Enum]
            exceptions = [definition for definition in module.definitions() if definition.__class__==idlast.Exception]
            structs = [definition for definition in module.definitions() if definition.__class__==idlast.Struct]
	    consts = [definition for definition in module.definitions() if definition.__class__==idlast.Const]
	    #print [definition.__class__ for definition in module.definitions()]

            for interface in interfaces:
                interface_label = interface.scopedName()[-1]
                trgs += [os.path.join(module_name, prefix + interface_label + suffix) for prefix, suffix in ffixes + single_ffix + if_ffixes]

            for typedef in typedefs:
                typedef_label   = typedef.declarators()[0].scopedName()[-1]
                scannage = scanTypedef(src.get_contents(), typedef)[0]
                if  scannage.startswith('sequence'):
		    #print "scannage starts with sequence", scannage
                    trgs += [os.path.join(module_name, prefix + typedef_label + suffix) for prefix, suffix in ffixes]
		elif typedefIsArray(src.get_contents(), typedef):
                    trgs += [os.path.join(module_name, prefix + typedef_label + suffix) for prefix, suffix in ffixes]
                else:
                    trgs += [os.path.join(module_name, typedef_label + 'Helper.java')]
                    struct_label = scanTypedefForStruct(src.get_contents(), typedef)
                    if struct_label:
                        trgs += [os.path.join(module_name, prefix + struct_label + suffix) for prefix, suffix in ffixes + single_ffix]
            for enum in enums:
                enum_label      = enum.scopedName()[-1]
                trgs += [os.path.join(module_name, prefix + enum_label + suffix) for prefix, suffix in ffixes + single_ffix]
            for exception in exceptions:
                exception_label = exception.scopedName()[-1]
                trgs += [os.path.join(module_name, prefix + exception_label + suffix) for prefix, suffix in ffixes + single_ffix]
            for struct in structs:
                struct_label = struct.scopedName()[-1]
                trgs += [os.path.join(module_name, prefix + struct_label + suffix) for prefix, suffix in ffixes + single_ffix]
	    for const in consts:
                const_label = const.scopedName()[-1]
                trgs += [os.path.join(module_name, const_label + '.java')]
        idlast.clear()
        idltype.clear()
        _omniidl.clear()
        try:
            package_name = os.path.basename(idl_file)[:-4]
            package_path = os.path.join(*tuple(env['PKGPREFIX'].split('.')))
            trgs = [os.path.join(env['IDL_INSTALL_DIRECTORY'],
                                 package_path,
                                 file) for file in trgs]
        except KeyError:
            trgs = [os.path.join(env['IDL_INSTALL_DIRECTORY'], file) for file in trgs]
        tlist += trgs
    print 'target:', tlist, '\nsource:', [str(file) for file in source]
#    return tlist, slist
    return tlist, source

def idlfile_scan(node, env, path):
    include_args = ['-I' + directory for directory in env['IDL_INCLUDE_PATH']]
    try:
        idl = parseOmniIdlArgs(include_args + [str(node)])
        idl_dependencies = []
        for module in idl.declarations():
            if (module.file() != idl.file()) and (module.file() not in idl_dependencies):
                idl_dependencies.append(module.file())
        idlast.clear()
        idltype.clear()
        _omniidl.clear()
        search_dirs = tuple([env.Dir(dirname) for dirname in env['IDL_INCLUDE_PATH']])
        idl_dependencies = [SCons.Node.FS.find_file(idlname, search_dirs) for idlname in idl_dependencies]
        return idl_dependencies
    except Exception:
        return []

def scanTypedefForStruct(contents, typedef):
    contents = filterOutComments(contents)
    text = ''
    reversed_text =  [line + '\n' for line in contents.split('\n')][:typedef.line()]
    reversed_text.reverse()
    for line in reversed_text:
        text = line + text
        if 'typedef' in line:
            break
    typedef_re = re.compile(r'typedef\s\s*struct\s\s*(\w*)', re.M)
    iterator = typedef_re.finditer(text)
    typedef_search = [match.group(1) for match in iterator]
    try:
        return typedef_search[0]
    except IndexError:
        return None

def getTypedef(contents, typedef):
    contents = filterOutComments(contents)
    text = ''
    reversed_text =  [line + '\n' for line in contents.split('\n')][:typedef.line()]
    reversed_text.reverse()
    for line in reversed_text:
        text = line + text
        if 'typedef' in line:
            break
    return text

def typedefIsArray(contents, typedef):
    text = getTypedef(contents, typedef)
    typedef_re = re.compile(r'typedef\s\s*([^\s]*)\s\s*(\w*)(\[\d*\])', re.M)
    iterator = typedef_re.finditer(text)
    typedef_search = [(match.group(1), match.group(2), match.group(3)) for match in iterator]
    try:
        return typedef_search[0][2]
    except IndexError:
        return None

def scanTypedef(contents, typedef):
    text = getTypedef(contents, typedef)
    typedef_re = re.compile(r'typedef\s\s*([^\s]*)\s\s*(\w*)', re.M)
    iterator = typedef_re.finditer(text)
    typedef_search = [(match.group(1), match.group(2)) for match in iterator]
    try:
        return typedef_search[0]
    except IndexError:
        return None

def filterOutComments(contents):
    return re.sub(r'(//[^\n]*)', '', contents)

def parseOmniIdlArgs(args):
    global preprocessor_args, preprocessor_only, preprocessor_cmd
    global no_preprocessor, backend, backend_args, dump_only, cd_to
    global verbose, quiet, print_usage, interactive, temp_file

    files = parseArgs(args)

    if _omniidl.alwaysTempFile():
        temp_file = genTempFileName()

    #if len(backends) == 0 and \
    #   not (quiet or dump_only or preprocessor_only or interactive):
    #    sys.stderr.write(cmdname + ": Warning: No back-ends specified; " \
    #                     "checking IDL for validity\n")

    for name in files:
        if name != "-" and not os.path.isfile(name):
            if not quiet:
                sys.stderr.write(cmdname + ": '" + name + "' does not exist\n")
            raise Exception

        if sys.platform != 'OpenVMS' or len(preprocessor_args)==0:
            preproc_cmd = '%s %s "%s"' % (preprocessor_cmd,
                                          string.join(preprocessor_args, ' '),
                                          name)
        else:
            preproc_cmd = '%s "%s" %s' % (preprocessor_cmd,
                                          string.join(preprocessor_args,'" "'),
                                          name)
        if not no_preprocessor:
            if verbose:
                sys.stderr.write(cmdname + ": Preprocessing '" +\
                                 name + "' with '" + preproc_cmd + "'\n")

            if preprocessor_only:
                err = os.system(preproc_cmd)
                if err:
                    if not quiet:
                        sys.stderr.write(cmdname + \
                                         ": Error running preprocessor\n")
                    raise Exception
                raise Exception

            if temp_file:
                if verbose:
                    sys.stderr.write(cmdname + \
                                     ": cpp output to temporary file '" + \
                                     temp_file + "'\n")
                err = os.system(preproc_cmd + " >" + temp_file)
                if err:
                    if not quiet:
                        sys.stderr.write(cmdname + \
                                         ": Error running preprocessor\n")
                    try:
                        os.remove(temp_file)
                    except:
                        pass
                    sys.exit(1)
                file = temp_file

            else:
                file = os.popen(preproc_cmd, "r")

        else: # No preprocessor
            file = name

        if verbose: sys.stderr.write(cmdname + ": Running front end\n")

        tree = _omniidl.compile(file)

        if not isinstance(file, StringType):
            if file.close():
                if not quiet:
                    sys.stderr.write(cmdname + \
                                     ": Error running preprocessor\n")
                sys.exit(1)

        if temp_file and not no_preprocessor:
            os.remove(temp_file)
    
            #idlast.clear()
            #idltype.clear()
            #_omniidl.clear()
    return tree

def generate_idlj_actions(source, target, env, for_signature):
    include_args = ''
    listCmd = []
    for directory in env['IDL_INCLUDE_PATH']:
        include_args += "-i %s " % (directory)
    for src in source:
        idl_file = src.get_string(for_signature)
        #idl_file = str(src)
        #print "GENERATE_IDLJ_ACTIONS:: generating files from %s to be installed in %s" % (idl_file, env['IDL_INSTALL_DIRECTORY'])
        defines = "-d %s "*len(env['CPPDEFINES']) % tuple(env['CPPDEFINES'])
        try:
            command = '-pkgPrefix %s %s -td %s %s -fallTie %s' % (os.path.splitext(os.path.basename(idl_file))[0], env['PKGPREFIX'], env['IDL_INSTALL_DIRECTORY'], include_args, idl_file)
	    try:
		for transtype, transpkg in env['PKGTRANSLATE']:
		    command = '-pkgTranslate %s %s ' % (transtype, transpkg) + command
		#command = '-pkgTranslate %s %s ' % (env['TRANSTYPE'], env['TRANSPKG']) + command
	    except KeyError:
		print "PKGTRANSLATE is not set. Carrying on..."
	    command = 'idlj %s %s' % (defines, command)
        except KeyError:
            command = 'idlj %s -td %s %s -fallTie %s' % (defines, env['IDL_INSTALL_DIRECTORY'], include_args, idl_file)
        print "command = ", command
        listCmd.append(command)
    return listCmd

def generate(env):
    env['IDL_INSTALL_DIRECTORY'] = '.'
    idlscan = SCons.Scanner.Scanner(function = idlfile_scan,
                                    skeys = ['.idl'])
    env.Append(SCANNERS = idlscan)
    try:
        if '.' not in env['IDL_INCLUDE_PATH']:
            env.Append(IDL_INCLUDE_PATH = '.')
    except KeyError:
        env.Append(IDL_INCLUDE_PATH = '.')
    env['BUILDERS']['IDLJ'] = \
        SCons.Builder.Builder(generator = generate_idlj_actions,
                              src_suffix = '.idl',
                              suffix = '.java',
                              emitter=idlj_emitter)
def test():
    env = SCons.Environment.Environment()
    generate(env)
    target, source = idlj_emitter([], [env.File('graph.idl')], env)

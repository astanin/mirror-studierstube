import SCons.Action
import SCons.Builder
import SCons.Util
import SCons.Script
import re

class NonSubstJarAction(SCons.Action.CommandAction):
    def __init__(self, cmd, cmdstr=None, *args, **kw):
	SCons.Action.CommandAction.__init__(self, cmd, cmdstr=None, *args, **kw)

    def process(self, target, source, env):
	result = [[self.cmd_list]]
        silent = None
        ignore = None
	return result, ignore, silent

    def strfunction(self, target, source, env):
        cmd_list, ignore, silent = self.process(target, source, env)
	_ret =  SCons.Action._string_from_cmd_list(cmd_list[0])
	return _ret

    def execute(self, target, source, env):
	SCons.Action.CommandAction.execute(self, target, source, env)

    def get_contents(self, target, source, env):
        cmd = self.cmd_list
        if SCons.Util.is_List(cmd):
            cmd = string.join(map(str, cmd))
        else:
            cmd = str(cmd)
	return cmd

def jarSources(target, source, env, for_signature):
    """Only include sources that are not a manifest file."""
    jarchdir = env.subst('$JARCHDIR')
    result = []
    for src in source:
        contents = src.get_contents()
        if contents[:16] != "Manifest-Version":
            if jarchdir:
                # If we are changing the dir with -C, then sources should
                # be relative to that directory.
                #src = src.get_path(src.fs.Dir(jarchdir))
		src = str(src)[len(jarchdir)+1:]
                result.append('-C')
                result.append(jarchdir)
            result.append(src)
    return result

def jarManifest(target, source, env, for_signature):
    """Look in sources for a manifest file, if any."""
    for src in source:
        contents = src.get_contents()
        if contents[:16] == "Manifest-Version":
            return src
    return ''

def jarFlags(target, source, env, for_signature):
    """If we have a manifest, make sure that the 'm'
    flag is specified."""
    jarflags = env.subst('$JARFLAGS')
    for src in source:
        contents = src.get_contents()
        if contents[:16] == "Manifest-Version":
            if not 'm' in jarflags:
                return jarflags + 'm'
            break
    return jarflags

JarAction = SCons.Action.Action('$JARCOM', '$JARCOMSTR')

def generate_jar_actions(source, target, env, for_signature):
    command = 'jar -cf '
    command += str(target[0]) + ' '
    jarchdir = env.subst('$JARCHDIR')
    for src in source:
	nested_class = re.compile(r'(\$((\w*).class))')
	_src = str(src).replace("$","\$")
	if jarchdir:
	    command += " -C %s %s" % (jarchdir, _src[len(jarchdir)+1:])
	else:
	    command += str(src) + ' '
    action = NonSubstJarAction(command)
    return action

JarBuilder = SCons.Builder.Builder(generator = generate_jar_actions,
#JarBuilder = SCons.Builder.Builder(action = JarAction,
                                   source_factory = SCons.Node.FS.Entry,
                                   suffix = '$JARSUFFIX')

def generate(env):
    """Add Builders and construction variables for jar to an Environment."""
    try:
        env['BUILDERS']['JarBld']
    except KeyError:
        env['BUILDERS']['JarBld'] = JarBuilder
    #SCons.Script.AllowSubstExceptions(SyntaxError)
    env['JAR']        = 'jar'
    env['JARFLAGS']   = SCons.Util.CLVar('cf')
    env['_JARFLAGS']  = jarFlags
    env['_JARMANIFEST'] = jarManifest
    env['_JARSOURCES'] = jarSources
    env['JARCOM']     = '$JAR $_JARFLAGS $TARGET $_JARMANIFEST $_JARSOURCES'
    env['JARSUFFIX']  = '.jar'

def exists(env):
    return env.Detect('jar')

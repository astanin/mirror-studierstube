from os.path import basename
from os import chmod
from SCons.Builder import Builder

def generateShellScript(filename, class_name, jars, env):
    f=open(filename, 'w')
    f.write("#!/bin/bash\n")
    classpath = "-cp " + ("%s:"*len(jars))[:-1] % tuple([str(jar) for jar in jars])
    java_command = "java -Djava.net.preferIPv4Stack=true"
    try:
	f.write(java_command + " " + classpath + " " + env['PKGPREFIX'] + "." + class_name + " $*" + "\n")
    except NameError:
	print "PKGPREFIX not set"
	f.write(java_command + " " + classpath + " " + class_name + " $*" + "\n")	
    f.close()
    chmod(filename, 0x0755)

def build_function(target, source, env):
    # Code to build "target" from "source"
    # target is the executable shell script
    #     e.g. '/usr/local/bin/Jmap'
    # source is a list of the jars in the classpath
    filename = str(target[0])
    class_name = basename(str(filename))
    jars = [jar for jar in source]
    generateShellScript(filename, class_name, jars, env)

def generate(env):
    bld = Builder(action = build_function)
    env.Append(BUILDERS = {'JavaShellScript' : bld})
               

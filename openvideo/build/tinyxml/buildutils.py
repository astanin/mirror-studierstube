from SCons.Script import *
from SCons.Builder import *


###########################################################################
# Substitutes symbols between '[at]' and '[at]' in a string. That is,
# recognizes the TOKEN part of '[at]TOKEN[at]' and replaces the expression
# with another one.
###########################################################################
def substitutefile(infile, outfile, at, substfunction, env):
    input  = open(infile)
    output = open(outfile, 'w')
    
    for line in input.readlines():
        while True:
            idx = line.find(at)
            if idx >= 0:
                output.write(line[0:idx])
                line = line[idx+len(at):len(line)]
                idx = line.find(at)
            
                if idx >= 0:
                    symbol = line[0:idx]
                    line = line[idx+len(at):len(line)]
                    output.write(substfunction(symbol, at, env))
                else:
                    output.write(line)
            
            else:
                output.write(line)
                break

    input.close()
    output.close()

###########################################################################
# Attempts to recognize a token and replaces the token with a string
###########################################################################
def envsubstitutefunction(symbol, at, env):
    """The function substitutes a symbol by looking it up in the environment dictionary"""
    
    output = '[]'
    try:
        output = str(env[symbol])
    except:
        print "Failed to substitute '" + symbol + "' using SCons build environment"

    return output

###########################################################################
# Substitutes a file based on the environment and writes the resulting file
# to disk.
###########################################################################
def substitute(target = None, source = None, env = None):
    substitutefile(str(source[0]), str(target[0]), '@@', envsubstitutefunction, env)
    return 0
    

###########################################################################
# Appends custom builders to the environment
###########################################################################
def appendbuilders(env):
    subbuilder = Builder(action = substitute, single_source = True)
    env.Append(BUILDERS = {'Substitute':subbuilder})

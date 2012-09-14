from SCons.Script import *
from SCons.Builder import *

import re


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
# Define a function that recursively lists directory contents
###########################################################################
def listtrav(directory, removebase = []):
    # like os.listdir, but traverses directory trees
    stack = [directory]
    files = []

    # Find the part of the absolute path name to be removed before
    # adding the file to the returned list.
    absbase = ''
    if removebase:
        absbase = os.path.abspath(removebase) + os.sep
    
    while stack:
        directory = stack.pop()
        for file in os.listdir(directory):
            fullname = os.path.join(directory, file)
            absname = os.path.abspath(fullname)
            usename = absname

            if removebase and absname.startswith(absbase):
                usename = absname[len(absbase):len(absname)]
                
            files.append(usename)
            # print usename
            
            if os.path.isdir(fullname) and not os.path.islink(fullname):
                stack.append(fullname)
                
    return files

###########################################################################
# Define a function that returns a list of strings found using a regexp
###########################################################################
def regexpfilter(stringarray, filter, keepmatch = True):
    expression = re.compile(filter)
    matches = []
    
    for string in stringarray:
        if expression.search(string):
            if keepmatch:
                matches.append(string)
                #print string
        else:
            if not keepmatch:
                matches.append(string)
                #print string

    return matches

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

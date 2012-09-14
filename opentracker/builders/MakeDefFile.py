import os, re
from os.path import basename
import itertools

import SCons.Node.FS
import SCons.Environment
import SCons.Builder
import SCons.Scanner

def generate_makedeffile_actions(source, target, env, for_signature):
    listCmd = []
    for s,t in itertools.izip(source, target):
        _lib, _def = str(s), str(t)
        listCmd += ['makedeffile.py %s %s 2.0 %s' %(_lib, basename(_def).split('.def')[0], _def)]
    return listCmd

def generate(env):
    env['BUILDERS']['MAKEDEFFILE'] = \
        SCons.Builder.Builder(generator = generate_makedeffile_actions,
                              src_suffix = '.lib',
                              suffix = '.def')
        

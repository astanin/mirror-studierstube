import SCons.Builder
import re
from os.path import join
from epydoc.docbuilder import build_doc_index
from epydoc.docwriter.html import HTMLWriter

apiurl_re = re.compile(r'([\S]*)\t([\S]*.html)\n')

class UrlList(list):
    def addUrlMapping(self, arg):
	mapping = apiurl_re.findall(arg)
	if mapping:
	    self.append(mapping[0][1])

def out(arg):
    print arg
	    
def emit_pydocs(target, source, env):
    urls = UrlList()
    slist = [str(s) for s in source]
    api = build_doc_index(slist)
    writer = HTMLWriter(api)
    writer.write_api_list(urls.addUrlMapping)
    other_targets = ['api-objects.txt', 'class-tree.html', 'crarr.png', 'epydoc.css', 'epydoc.js', 'frames.html', 'help.html', 'identifier-index.html','index.html','module-tree.html','redirect.html', 'toc.html', 'toc-everything.html']
    print [m.filename for m in writer.module_list]
    pysrcs = [m.canonical_name.__str__() + '-pysrc.html' for m in writer.module_list]
    tocs   = ['toc-' + m.canonical_name.__str__() + '-module.html' for m in writer.module_list]
    tlist  = [join('html', url) for url in urls + other_targets + pysrcs + tocs]
    return tlist, slist

def generate_pydoc_actions(source, target, env, for_signature):
    s = "epydoc " + len(source)*"%s "
    command =  s % tuple([str(f) for f in source])
    return command

EPyDocBuilder = SCons.Builder.Builder(generator = generate_pydoc_actions,
				      src_suffix = '.py',
				      emitter   = emit_pydocs)

def generate(env):
    """Add Builders and construction variables for epydoc to an Environment"""
    env['BUILDERS']['EPyDoc'] = EPyDocBuilder

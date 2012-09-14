import sys
if sys.platform.startswith('linux'):
    import dl
    sys.setdlopenflags(dl.RTLD_NOW|dl.RTLD_GLOBAL)

from pivy.coin import *
from pivy.sogui import *

def readFile(filename):
    # Open the input file
    mySceneInput = SoInput()
    if not mySceneInput.openFile(filename):
        print >> sys.stderr, "Cannot open file %s" % (filename)
        return None

    # Read the whole file into the database
    myGraph = SoDB.readAll(mySceneInput)
    if myGraph == None:
        print >> sys.stderr, "Problem reading file"
        return None
    
    mySceneInput.closeFile()
    return myGraph

def getGate(output_list, type, index):
    engine  = SoSelectOne(SoMFString.getClassTypeId())
    engine.input.connectFrom(output_list[0])
    engine.index.setValue(index)
    new_output_list = SoEngineOutputList()
    engine.getOutputs(new_output_list)

    gate = SoGate(type)
    gate.input.connectFrom(new_output_list[0])
    gate.enable = TRUE

    return gate

def getSharedEngine(channel_name):
    sharedEngineType = SoType.fromName('SharedEngine')
    shared_engine = sharedEngineType.createInstance()
    channel_setting = "channel \"%s\"" % (channel_name)
    print "channel_setting = " ,channel_setting
    shared_engine.set(channel_setting)
    
    output_list = SoEngineOutputList()
    shared_engine.getOutputs(output_list)

    trans_gate = getGate(output_list, SoMFVec3f.getClassTypeId(), 0)
    ori_gate   = getGate(output_list, SoMFRotation.getClassTypeId(), 1)

    return trans_gate, ori_gate

def getSharedEngineTransform(channel_name):
    trans_gate, ori_gate = getSharedEngine(channel_name)
    transform = SoTransform()
    transform.translation.connectFrom(trans_gate.output)
    transform.rotation.connectFrom(ori_gate.output)
    return transform

def getAxis(scale):
    sep = SoAnnotation()
    scaling = SoScale()
    setting = "scaleFactor %f %f %f" % (scale, scale, scale)
    scaling.set(setting)
    sep.addChild(scaling)
    
    sep.addChild(readFile('iv/axis.iv'))
    return sep

def getSharedAxis(scale, channel_name):
    sep = SoSeparator()
    transform = getSharedEngineTransform(channel_name)
    sep.addChild(transform)
    axis = getAxis(scale)
    sep.addChild(axis)
    return sep

def main():
    # Initialize Inventor and Qt
    myWindow = SoGui.init(sys.argv[0])  
    if myWindow == None: sys.exit(1)
    root = SoSeparator()
    root.addChild(getAxis(1.5))
    root.addChild(getSharedAxis(1.5, "corbaname:rir:#TestChannel"))
    
    # Display them in a viewer
    myViewer = SoGuiExaminerViewer(myWindow)
    myViewer.setSceneGraph(root)
    myViewer.setTitle("Simple Shared Test")
    myViewer.setTransparencyType(SoGLRenderAction.SORTED_OBJECT_BLEND)
    myViewer.viewAll()
    myViewer.show()

    # write examiner scene graph to file
    #fd  = open('examiner.iv', 'w')
    #out = SoOutput()
    #out.setFilePointer(fd)
    #w   = SoWriteAction(out)
    #w.apply(myViewer.getSceneManager().getSceneGraph())
    #fd.close()

    SoGui.show(myWindow)
    SoGui.mainLoop()
    
if __name__ == '__main__':
    main()

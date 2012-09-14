<?php require "header.php"; ?>
<h1>Opentracker Reconfiguration Manual</h1>

      <p>Opentracker now supports reconfiguration which means it is possible to change the dataflow graph during runtime. This document explains how to use the reconfiguration facility both from the command line and programmatically. It also explains the mechanisms involved in opentracker reconfiguration for version 2.0.

      <h2> How to use it</h2>
      <p>This section explains how to use the reconfiguration facilities of OpenTracker both from the command line, and from Studierstube. Other posibilities for using reconfiguration will be added here.</p>
      
      <h3>The Command Line</h3>
      <p>    OpenTracker provides a runtime that can be used to test configurations of tracking systems. The new reconfigurable version of opentracker provides the same runtime as previous version, without support for reconfiguration. It also provides a new runtime, named opentracker2c, which has reconfiguration enabled by default. In order to start the runtime on the command line: 
	</p>
      <div class="fragment"><pre>
		WINDOWS
		&gt opentracker2c my_config_file.xml
		LINUX
		# .\opentracker2c my_config_file.xml
      </pre></div>
<p> 
   This command, beside initializing opentracker, starts a separate thread which monitors my_config_file.xml, the reconfiguration thread. At startup the reconfiguration thread reads the last modification timestamp of my_config_file.xml. The thread checks each time whether the file has been modified, by comparing timestamps. If the file has not been changed the thread sleeps for a while and tries again. Otherwise, the thread passes this new configuration to opentracker which then proceeds to change the configuration.
</p>
<p>
    If for some reason one wishes to use the configuration facility but not change the main configuration file. Suppose one wishes to start the session with a file, and then change some parameters, but not on the main file. For this purpose, opentracker2c can be called with 2 parameters, first the main configuration file, and second the file for reconfiguration. This late file can be non-existent at startup.
</p>	
      <div class="fragment"><pre>
		WINDOWS
		&gt opentracker2c my_config_file.xml reconfig.xml
		LINUX
		# .\opentracker2c my_config_file.xml reconfig.xml
      </pre></div>
<p>
    In this case the reconfiguration thread will monitor the file reconfig.xml instead of the main configuration file. Changing my_config_file.xml will accomplish nothing, since the runtime just uses it for startup in this case.
</p>

    <h3>Studierstube</h3>
<p>
    Studierstube, as one of the main users of OpenTracker, can also make use of the reconfiguration facility, and has been extended to use it. By default, however, the reconfiguration facility is not built in studierstube. It is necessary to set the preprocessor flag OT_ENABLE_RECONFIGURATION in order to compile the event component of studierstube with reconfiguration. Once reconfiguration is enabled, it is possible to change the opentracker configuration file passed to studierstube to use a new dataflow graph. At the moment it is not possible to monitor a file different from the main configuration from within studierstube.
</p>
<p>
    One has to be carefull though when changing the configuration for studierstube. The stb event system will rely on EventSinks with the right id to exist in opentracker.
      <div class="fragment"><pre>
		&lt EventSink tracking="otTrack"&gt	 
			&lt TestSource position="0 0.9 0" noise="0.5" frequency="1"/&gt
		&lt/EventSink&gt
      </pre></div>
    If these sinks suddenly dissapear, i.e. changing the name otTrack to whatever in the example above ,the stb event system will not be able to get data from them. This is a minor problem though, and it can be fixed at runtime by adding the appropriate sinks again.

</p>

    
<h2> How to extend/program it</h2>
<p>
    It is not absolutely necessary to run a separate thread to be able to use reconfiguration. This solution has been provided because it is simple to use in an experimental setup, one does not need to use external libraries. However, more advanced situations that need to use different mechanisms can be imagined. The reconfiguration API from OpenTracker supports reconfiguration different levels for such situations. 
</p>
<h3>   Configurator: higher level reconfiguration </h3>
<p>
       The Configurator API provides access to high level reconfiguration operations. The Configurator is a singleton, that contains all information necessary to create a new dataflow graph. This information includes a list of all available Modules, together with their initialization functions. It also keeps a reference to the currently running Context (see below for more information about the Context).
</p>
       

<div class="fragment"><pre>
  class OPENTRACKER_API Configurator {

	static Configurator * instance();
	virtual ~Configurator();

	Context & getContext();

	void changeConfiguration(std::string configString);
	void changeConfigurationFile(const char* xmlstring);
	void changeConfigurationString(const char* xmlstring);

	void runConfigurationThread(const char * filename);

	static void addModuleInit(const char * name, ModuleInitFunc func, void * data);
	static void loadModule(Context & newctx, const char * module);

   };
</pre></div>
<p>
   The method getContext provides a reference to the running Context. In order to configure such Context, the Configurator provides the methods changeConfiguration takes a string argument, if the string is finished with ".xml" then it passes it to the changeConfigurationFile method, otherwise it passes it to the changeConfigurationString method.
</p>
<p>
   Both of these methods behave in a similar way. They create a new (temporary) Context, configure it using the new xml, and then, the running context is asked to copy the configuration from the new one, releasing all references to its previous configuration. 
</p>
<p>
   The advantages of using this approach are:
   <ul>  
     <li>It is not necessary to know which parts of the configuration were changed. The Configurator replaces everything.</li>
     <li>It is safe, if the new configuration can not be parsed, the old one should keep running.</li>
   </ul>
</p><p>
   The disadvantages:
    <ul>
     <li>A small change in a large configuration file still requires to change the whole configuration.</li>
     <li> It reliess on modules being able to cleanup resources correctly (which sometimes is not the case).</li>
    </ul>
</p>
<p>
   The main drives for using this approach are lazyness (or simplicity) and ignorance. Lazyness, often dubbed the naive approach, means it is not necessary to compare two possibly complex datastructures to find out where they differ. Ignorance means, it is not necessary to know the details of each module. Assuming that one knows a certain part of the graph has to be reconfigured, it is difficult to know whether changing the configuration requires actually reseting the driver for such part of the graph or just passing a parameter will do. For example changing a serial port requires to actually reset a driver. These details are module specific and can not be dealt with from a general approach without either knowing the details beforehand or adding complexity to each driver to handle the situation.
</p>
   
<h3>   Context: runtime information + lower level reconfiguration </h3>

<p>       The Context manages the runtime, keeps references to the dataflow graph and all the drivers used in that graph. The part of the API that concerns reconfiguration is the following:

<div class="fragment"><pre>
     Context{
	void addNode(std::string parentid, Node * newnode);
	void removeSubtree(std::string nodeid);
	void removeNode(std::string nodeid);
	void replaceNode(std::string nodeid, Node * newnode);
     };
</pre></div>
</p>
<p>       These methods allow adding nodes to the currently runtime graph, removing or replacing parts of the graph. These methods act on the graph, but cannot change the configuration of the drivers responsible for communicating with external devices. By being able to change the dataflow graph, it is possible to perform a fein-grained reconfiguration without the need to reset the whole runtime. 
</p><p>       Adding a node (addNode) requires an id for the parent of such node. It also actually allows adding a whole tree under the parent. The method searches for the node in the graph, using the provided parentid, and then calls its addChild method to add the node.
</p><p>
       Similarly the removeSubTree method searches a node from an id, and then calls removeChild on all its parents. 
       removeNode also uses the id to locate the node to be removed, it adds all children of this node to its parent(s) and then removes the node. This is usefull in case one needs to remove a transitional node, which is not a source or a sink.
</p>




<h2> Requirements: Module cleanup and registration</h2>
<p>
  Any reconfiguration approach would require some cooperation from the device drivers, in OpenTracker called Modules. In the current case the modules could be stoped, deleted and restarted many times during an execution, as opposed to OpenTracker in previous versions, where the drivers were created and initialized once at the beginning, and onle stopped and deleted at the end. 
</p>
<p> 
 One requirement for modules is that they must correctly clean up all their allocated resources when being deleted. Another requirement, introduced by opentracker reconfiguration, is that Modules must register themselves with the Configurator in order to be instantiated.
</p>
<p>
  The Configurator keeps a dictionary of module initialization functions that will be called whenever a module appears in a configuration file. For module creators this means that Modules must provide the initialization function of the form:
<div class="fragment"><pre>
  	void registerModuleMODNAME( Context * context , void * data) 
</pre></div>
</p>
<p>
  The name is non important, and the function can be a static function inside a class or a C function. When executed, this function is expected to create an instance of the Module, and add it to the provided Context, for example by:
<div class="fragment"><pre>
       		MyModule * mod = new MyModule();
		context->addFactory (*mod);
		context->addModule ("MyModuleConfig", *mod);
</pre></div>
</p>
<p>
  The void * data is meant to pass any information needed by the initialization function. As already introduced above an initialization function will be called by the Configurator upon demand (when a configuration file requires instantiating a Module), so it also becomes necessary to add the initialization functions to the Configurator before reading some configuration.
<div class="fragment"><pre>      
      	Configurator::addModuleInit(MyModule, registerModuleMODNAME, (void*)mydata)
</pre></div>
</p>
<p>
  The Configurator will then store a functor with the (optionally) provided data, and will execute the function upon demand. 
</p>
<h3>    Module Creation Helpers  </h3>
<p>
  OpenTracker provides a set of macros intended to simplify the module registration process, including:

<ol>
    <li> OT_MODULE(MODNAME) This macro is intended to be used in the header of each module, it defines the registration function void registerModule##MODNAME (Context * context, void * data) </li>
    <li>OT_MODULE_REGISTRATION_DEFAULT(MODNAME, REGISTRATIONSTRING) This macro is intended to be used in the body of a module, it declares the default initialization that most modules will follow: </br>
<div class="fragment"><pre>      
		MODNAME * mod = new MODNAME();
		context->addFactory (*mod);
		context->addModule (REGISTRATIONSTRING, *mod);
</pre></div>
		</li>
    <li> OT_REGISTER_MODULE(MODNAME, VOIDPARAMETER) This macro adds the initialization function to the Configurators dictionary, within OpenTracker it is normally used inside OpenTracker.cxx during the module registration process. 	(Configurator::addModuleInit(#MODNAME, registerModule##MODNAME, VOIDPARAMETER))</li>
</ol>
</p>

<h3>    Module Creation Example</h3>
<p>
     A code example of creating a Module is as follows:
</p>
<p>
In the header file
<div class="fragment"><pre>      
namespace ot{

  class ExampleModule : public Module, public NodeFactory{

  public:
    ExampleModule();
    virtual ~ExampleModule();
    
    virtual void init(StringTable & attributes, ConfigNode * localTree);


    // NodeFactory interface
    virtual Node * createNode( const std::string & name, StringTable & attributes);
	virtual void pushEvent();
  };

  OT_MODULE(ExampleModule);

}; // namespace ot
</pre></div>    
</p>
<p>
In the .cxx file
<div class="fragment"><pre>      
namespace ot{

  OT_MODULE_REGISTER_FUNC(ExampleModule){
    OT_MODULE_REGISTRATION_DEFAULT(ExampleModule, "ExampleConfig");
  }

  ExampleModule::ExampleModule(){

  };

 ExampleModule::~ExampleModule(){
    // dont forget to clear all created nodes
    cleanupExampleNodes();
    // and also to release all acquired resources
    releaseAllResources();
  };
    
  void ExampleModule::init(StringTable & attributes, ConfigNode * localTree){
    //Called if the Module has a config section
  };
  
  
  // NodeFactory interface
  Node * ExampleModule::createNode( const std::string & name, StringTable & attributes){
    Node * result = 0;

    if (name.compare ("ExampleSource") == 0){

	  ExampleSource * node = new ExampleSource(device);
	  this->addNode(node);
	  result = node;

    } else if(name.compare ("ExampleSink") == 0){

	ExampleSink * node = new ExampleSink;
	this->addNode(node);
	result = node;

    }

    return result;
  };
  
  void ExampleModule::pushEvent(){
	  for (ExampleSrcDict::iterator i = srcDict.begin(); i != srcDict.end(); i++){
		ExampleSource * src = (*i);
		src->pushEvent();
	  }
};
}; // namespace ot
</pre></div>
</p>
<p>
For Modules within OpenTracker, the OpenTracker.cxx has to be edited to add the line
<div class="fragment"><pre>      
#ifdef USE_EXAMPLE_MODULE
        OT_REGISTER_MODULE(ExampleModule, NULL);
#endif
</pre></div>
</p>
<p>
Other libraries that use OpenTracker and define their own Modules to communicate with opentracker can either use this macro or call the Configurator::addModuleInit(#MODNAME, registerModule##MODNAME, VOIDPARAMETER); directly. This must be done before parsing a configuration, otherwise OpenTracker will not be able to find the module.
</p>

<?php require "../footer.php"; ?>
</body>
</html>

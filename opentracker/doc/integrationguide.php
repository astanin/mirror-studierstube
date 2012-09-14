<?php require "header.php"; ?>

  <h1>Integration Guide</h1>
	<p>This document is a short guide to how to use and integrate <!-- #BeginLibraryItem "/Library/OpenTracker.lbi" --><font color="#FF0000">Open</font><font color="#000000">Tracker</font><!-- #EndLibraryItem --> 
          in your project.</p>        
        <h2>Before Integration</h2>      <p>We assume that you already downloaded and compiled <!-- #BeginLibraryItem "/Library/OpenTracker.lbi" --><font color="#FF0000">Open</font><font color="#000000">Tracker</font><!-- #EndLibraryItem -->. 
          If you need help on that, look at the <a href="install.php">Install HowTo</a>. 
	</p>      <h2>Integration</h2>      <p>The typical use of <!-- #BeginLibraryItem "/Library/OpenTracker.lbi" --><font color="#FF0000">Open</font><font color="#000000">Tracker</font><!-- #EndLibraryItem --> 
          is as input library. You would specify a configuration file for the handling 
          and transformation of the tracking data and then read it at some specified 
          places out of the tracker tree. This involves setting up and running the 
          <!-- #BeginLibraryItem "/Library/OpenTracker.lbi" --><font color="#FF0000">Open</font><font color="#000000">Tracker</font><!-- #EndLibraryItem --> 
          main loop and getting at the actual data.</p>        
	<h3>Context Class</h3>      <p>The class <span class="name">Context</span> provides all you need to 
          work with the <!-- #BeginLibraryItem "/Library/OpenTracker.lbi" --><font color="#FF0000">Open</font><font color="#000000">Tracker</font><!-- #EndLibraryItem --> 
          library. It manages the modules, uses a parser to build the run time data 
          structure from a configuration file and implements the main loop. One 
          object of type <span class="name">Context</span> always manages exactly 
          the data flow graph specified in one configuration file.</p>      <p>To be able to parse a configuration file a <span class="name">Context</span> 
          needs node factories that instantiate the different node types. These 
          can be added using the member function <span class="name">addFactory</span>. 
          Moreover modules need to be added with the member function <span class="name">addModule</span> 
          which takes also the name of the configuration element. Typically actual 
          modules implement both the <span class="name">Module</span> and the <span class="name">NodeFactory</span> 
          interface and need to be added as both. The default constructor instantiates 
          a clean <span class="name">Context</span> without any modules or factories. 
          To get a <span class="name">Context</span> object with the default modules 
          and node factories call the constructor with the value 1. In this case 
          the object will also clean up the instantiated modules.</p>      <p>Now the object is ready to parse a configuration file. The member <span class="name">parseConfiguration</span> 
          takes a string with the path and filename of an XML configuration file. 
          See the <a href="userguide.php">User's Guide</a> for the syntax and semantics 
          of such a file. It builds the run time data structure and the <span class="name">Context</span> 
          is now ready to perform any computations.</p>      <p>Next the member function <span class="name">start</span> tells all modules 
          that the processing is about to begin. Then the following member functions 
          are called in order in a loop to process the data flow.</p>      <ol>
          <li><span class="name">pushEvent</span> - calls pushEvent on all modules 
            to create new events and process them. </li>
          <li><span class="name">pullEvent</span> - calls pullEvent on all modules 
            to read data out of the data flow graph and process any Queue an Timedependend 
            nodes.</li>
          <li><span class="name">stop</span> - queries all modules, if they encountered 
            a reason to stop. If one or more modules return <b>1</b>, the member 
            returns <b>1</b> itself telling the application to quit.</li>
	</ol>      <p>After the the application decides to quit and leaves the main loop the 
          member function <span class="name">close</span> should be called to give 
          the Context and the module a chance to clean up resources.</p>      <p>An application can either call these members in its own main loop or 
          execute the member function <span class="name">run</span> instead, which 
          implements initialization, the loop and cleanup. See also the source code 
          for the standalone tracking program as an example.</p>      <p>The following code example shows how to use the Context node in your 
          own main loop :</p>
        <div class="fragment"><pre>Context context( 1 );                                       // create a new context
	    context.parseConfiguration(&quot;mypath/myconfig.xml&quot;);     // parse a configuration file
	    
	    ...  your own initialization
	    context.start();                                       // start the context when you are ready
	    
	    while( context.stop() != 1 &amp;&amp; quit != 1 )              // loop until context our your code quits
	    {
	    ... do your own processing
	    context.pushEvent();                               // push event and
	    context.pullEvent();                               // pull event
	    }
	    ... your own cleanup
	    context.close();                                       // clean up finally</pre></div>
	<p>Using the Context main loop is even simpler, as the following example 
          shows :</p>      
        <div class="fragment"><pre>Context context( 1 );                                       // create a new context
	    context.parseConfiguration(&quot;mypath/myconfig.xml&quot;);     // parse a configuration file

	    context.run();                                         // does everything for you !
	</pre></div>
        <h3>Using Callback Nodes</h3>      <p>The simplest way to get data out of the data flow graph is to use <span class="name">Callback</span> 
          nodes. These nodes execute callback functions whenever they receive an 
          event. To use such nodes the application needs access to the <span class="name">CallbackModule</span> 
          which implements these nodes. Therefore the application needs to retrieve 
          a pointer to the callback module from the context. The module is stored 
          under the name &quot;CallbackConfig&quot;. The following code example 
          shows how to do this :</p>
        <div class="fragment"><pre>void callbackF( Event &amp; event );                    // definition of the callback function 
	    
	    Context context( 1 );                               // create a context
            // get callback module from the context
	    CallbackModule * callbackMod = (CallbackModule *)context.getModule(&quot;CallbackConfig&quot;);

	    context.parseConfiguration(&quot;mypath/myconfig.xml&quot;);  // parse the configuration file

	    callbackMod-&gt;setCallback( &quot;cb1&quot;, &amp;callbackF );    // sets the callback function</pre></div>
	<p>The context is created. Then the <span class="name">CallbackModule</span> 
          is retrieved via getModule<span class="name"></span>. Now the configuration 
          file is parsed and any nodes of type <span class="name">Callback</span> 
          are created. Every <span class="name">Callback</span> node has a unique 
          name that is used to set a callback function afterwards. The next line 
          uses the member function <span class="name">setCallback</span> to set 
          the function <span class="name">callbackF</span> as the callback of the 
          node with the name <b>cb1</b>. </p>      <h3>Using your own Node Type</h3>      <p>You can also define and implement a new node type derived from Node to 
          get data out of the tracker tree. Please refer to the <a href="progguide.php">Programmer's 
	    Guide</a> for detailed informatio
	  t how to do that.</p>
  

<?php require "../footer.php"; ?>

</body>
</html>

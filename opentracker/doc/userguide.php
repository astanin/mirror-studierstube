<?php require "header.php"; ?>

<h1>User Guide to OpenTracker</h1>
<p>This is a user's guide to essential features and of OpenTracker. It describes how configuration files work and how to use the standalone program.</p> 
<h2>Introduction ( or What is XML ? )</h2> 
<p>OpenTracker uses a data flow graph to process tracking data. This graph is specified by a textual configuration file. The format of this file is defined in the <a href="http://www.w3c.org/">eXtensible Markup Language</a> ( XML ). XML is a language to specify the syntax of other languages. It allows us to specify hierarchical structures that are mapped onto our data flow graphs. The language itself is defined in Document Type Description stored in opentracker.dtd. It uses element tags ( like HTML ) and attributes of these tags ( again like HTML ) but no interim textual parts ( unlike HTML ). So it typically looks like a very dry HTML file using strange tags.</p> 
<h2>Structure of an OpenTracker Configuration File</h2> 
<p>The simplest configuration file looks like this :</p> 
<div class="fragment"><pre>&lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;
&lt;!DOCTYPE OpenTracker SYSTEM <font color="#FF0000">&quot;opentracker.dtd&quot;</font>&gt;
&lt;OpenTracker&gt;
   &lt;configuration/&gt;
&lt;/OpenTracker&gt;
</pre> </div>
<p>The first two lines are XML stuff and need to appear in every file. The important thing is that the file <span class="filename">opentracker.dtd</span> can actually be found where it says in the <span class="element">DOCTYPE</span> element. Then there is a general <span class="element">OpenTracker</span> element, framing the whole config file and then an empty element <span class="element">configuration</span>. General configuration options for modules are children of this element. These parts have to appear in every config file ! </p> 
<p>A more interesting example :</p> 
<table width="100%" border="0" cellspacing="2" cellpadding="5"> 
  <tr> 
    <td valign="top"> <div class="fragment"><pre>&lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;
&lt;!DOCTYPE OpenTracker SYSTEM &quot;opentracker.dtd&quot;&gt;
&lt;OpenTracker&gt;
  &lt;configuration&gt;
    &lt;ConsoleConfig headerline=&quot;Sample Output&quot; interval=&quot;10&quot;/&gt;
  &lt;/configuration&gt;
  &lt;ConsoleSink comment=&quot;Test Source 1&quot;&gt;
    &lt;EventTransform translation=&quot;1 1 1&quot;&gt;
      &lt;TestSource/&gt;
    &lt;/EventTransform&gt;
  &lt;/ConsoleSink&gt;
&lt;/OpenTracker&gt;</pre></div> </td> 
    <td rowspan="2" align="center" valign="middle"><img src="images/example1.gif" width="150" alt="Data Flow Graph"></td> 
  </tr> 
  <tr> 
    <td valign="middle" align="center"><b>Fig 1 : Simple config file with corresponding data flow graph</b> </td> 
  </tr> 
</table> 
<p>Here anything coming after the <span class="element">configuration</span> element is part of the data flow description. Each element is mapped to a node used by OpenTracker. The element names (tag names) are used to perform this mapping. See the node reference for details on which element corresponds to which node. The attributes of an element specifiy configuration parameters of the node, such as the parameters of a transformation or the network address of a multicast group.</p> 
<p>The direction of the flow is from children elements to parent elements. Therefore sources (generators of new events) are typically found at the core of a series of nested elements. Sinks (nodes that take events and do something outside of OpenTracker with it) are typically found close to the <span class="element">OpenTracker</span> root element.</p> 
<p>In this example, the <span class="element">configuration</span> element has now a child <span class="element">ConsoleConfig</span>. This element sets general parameters of the console input/output module, in this case the header line to display and the sample intervals for displaying updates.Then a simple linear graph follows, expressed as a series of nested elements. The top-most element is a <span class="element">ConsoleSink</span>, corresponding to a <span class="element">ConsoleSink</span> node, that displays data on the console. It's child is an <span class="element">EventTransform</span> node, that translates the incoming data by the vector <tt>(1,1,1)</tt>. Any finally a <span class="element">TestSource</span> generating events in regular intervals.</p> 
<h3>References</h3> 
<p>The output of one node, can also be reused as input for a second parent node. This is accomplished with reference nodes. They simply link to the whole subgraph referenced and do not have any children of their own. To use a reference node specify a <span class="element">DEF</span> attribute on the node you want to reference. The value must be unique. Then use a <span class="element">Ref</span> element and set it's <span class="element">USE</span> attribute to the same name. A name that is used in a <span class="element">USE</span> attribute has to be specified first in a <span class="element">DEF</span> attribute. The following table shows an example :</p> 
<table width="100%" border="0" cellspacing="2" cellpadding="5"> 
  <tr> 
    <td valign="top"> <div class="fragment"><pre>&lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;
&lt;!DOCTYPE OpenTracker SYSTEM &quot;opentracker.dtd&quot;&gt;
&lt;OpenTracker&gt;
  &lt;configuration&gt;
    &lt;ConsoleConfig headerline=&quot;Sample Output&quot; interval=&quot;10&quot;/&gt;
  &lt;/configuration&gt;
  &lt;ConsoleSink comment=&quot;Test Source 1&quot;&gt;
    &lt;EventTransform translation=&quot;1 1 1&quot; <font color="#FF0000">DEF=&quot;transform&quot;</font>&gt;
      &lt;TestSource/&gt;
	  &lt;/EventTransform&gt;
  &lt;/ConsoleSink&gt;
  &lt;NetworkSink name=&quot;Test&quot; number=&quot;1&quot; <font color="#666666">...</font>&gt;
    <font color="#FF8040">&lt;Ref <font color="#FF0000">USE=&quot;transform&quot;</font>/&gt;</font>
  &lt;/NetworkSink&gt;
&lt;/OpenTracker&gt;</pre> </div></td> 
    <td rowspan="2" align="center" valign="middle"><img src="images/example2.gif" width="300" alt="Data Flow Graph" height="255"></td> 
  </tr> 
  <tr> 
    <td valign="middle" align="center"><b>Fig 2 : Config file using References and resulting Data Flow Graph</b></td> 
  </tr> 
</table> 
<p>Here the necessary attributes have been highlighted in red. The reference element is shown in orange.</p> 
<p>There are some issues that should be keep in mind. <span class="element">Ref</span> elements are not type safe, i.e. they do not enforce the usuall interface restrictions ! And moreover it is possible to define loops using <span class="element">Ref</span> elements. This would put OpenTracker in an endless loop and should be avoided !</p> 
<h3>Wrapper Elements</h3> 
<p>Nodes may have several children grouped corresponding to input ports. For example a Merge node has input ports for position, orientation, button, confidence, time and default and each of these ports may link to several children. To specify such a structure we use wrapper elements as children of the Merge element that in turn contain the actual children. The Merge node can then distinguish the input by distinguishing between the wrapper elements.This is an example of how to use a Merge node :</p> 
<table width="100%" border="0" cellspacing="2" cellpadding="5"> 
  <tr> 
    <td valign="top"><div class="fragment"> <pre>&lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;
&lt;!DOCTYPE OpenTracker SYSTEM &quot;opentracker.dtd&quot;&gt;
&lt;OpenTracker&gt;
  &lt;configuration&gt;
    &lt;ConsoleConfig headerline=&quot;Sample Output&quot;/&gt;
  &lt;/configuration&gt;
  &lt;ConsoleSink comment=&quot;Test Source 1&quot;&gt;
    <font color="#FF8040">&lt;Merge&gt;</font>
      <font color="#FF0000">&lt;MergeDefault&gt;</font>
        &lt;NetworkSource number=&quot;2&quot; .../&gt;
      <font color="#FF0000">&lt;/MergeDefault&gt;
      &lt;MergeAttribute attributeName=&quot;position&quot;&gt;</font>
        &lt;EventTransform translation=&quot;1 1 1&quot;&gt;
          &lt;TestSource/&gt;
        &lt;/EventTransform&gt;
      <font color="#FF0000">&lt;/MergeAttribute&gt;
      &lt;MergeAttribute attributeName=&quot;button&quot;&gt;</font>
        &lt;ConsoleSource number=&quot;0&quot;/&gt;
      <font color="#FF0000">&lt;/MergeAttribute&gt;</font>
    <font color="#FF8040">&lt;/Merge&gt;</font>
  &lt;/ConsoleSink&gt;
&lt;/OpenTracker&gt;</pre> </div></td> 
    <td rowspan="2" align="center" valign="middle"><img src="images/example3.gif" width="400" alt="Data Flow Graph" height="302"></td> 
  </tr> 
  <tr> 
    <td valign="middle" align="center"><b>Fig 3 : Using a Merge node and the Data Flow Graph</b></td> 
  </tr> 
</table> 
<p>Here the wrapper elements are highlighted in red and the element using them is highlighted in orange. The graph shows how the different children elements are connected to the different ports of the node.</p> 
<h2>Standalone Server</h2> 
<p>Besides the default use as a library, OpenTracker also provides a simple standalone program. called <span class="filename">opentracker</span>. It accepts a configuration file as input and then executes the data flow specified in the configuration file until stopped. The program has only one parameter, the filename of the configuration file. It outputs errors during parsing and building the data structure. An example execution looks like :</p> 
<pre>opentracker test.xml</pre> 
<p>Thats it, Have Fun !</p>

<?php require "../footer.php"; ?>

</body></html>

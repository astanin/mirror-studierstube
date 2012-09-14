<?php require "header.php"; ?>
<h1>Multi-Modal Events</h1>
      <p>This document gives you a short overview over the new concept of multi-modal events in OpenTracker and shows you how to use your own data types for event attributes.</p>
      

      <h2>Concept of Multi-Modal Events</h2>
      <p>Since version 1.2, OpenTracker events are not limited to tracking related data any more. Instead, events consist of a number of attributes that can be created dynamically and can hold data of arbitrary, generic types. Each node creating or transforming an event can dynamically add new attributes or access them by name. Combination of multiple event attributes of different types within the event class allows grouping of attributes that belong together (such as simultaneous measurements) in a single event instance.</p>
      
      <h2>Event Handling</h2>
      <p>Nodes operate on events by looking for a known attribute name, the semantic of which is established by naming conventions. The set of attributes in an event is introduced dynamically. Whenever a node attempts to access an attribute by a name not known to the event, this attribute can be created transparently inside the event.</p>
      
      <ul>
	<li>The first node to operate on an event (source) will start with an empty event with no attributes.</li>
	<li>Attributes can be accessed by generic methods provided by the Event class (see below).</li>
	<li>If the name does not exist, but a default value is provided within the method call, a new attribute is created transparently and initialized to the given value.</li>
	<li>If the name does not exist and the access method is called without a default value, an invalid argument exception is thrown.</li>
	<li>If the name exists already, but the type does not match, a runtime exception is thrown.</li>
      </ul>

      <h2>How to Access Event Attributes</h2>
      <p>A simplified class diagram of the Event class is shown in Figure 1. The Event class was realized as a map matching names (strings) to according attributes (typed data values). It provides the only public interface to access the information stored in the event's attributes.</p>
      
      <table border="1" align="center">
	<tr><th>ot::Event</th></tr>
	<tr><td>#attributes: map&lt;string, EventAttributeBase*&gt;<br/>
	    + Event(rv: Event&)<br/>
	    + operator=(rv: Event&): Event&<br/>
	    + hasAttribute(name: string&): bool<br/>
	    + delAttribute(name: string&): bool<br/>
	    + renAttribute(name: string&, newName: string&): bool<br/>
	    + getAttribute&lt;T&gt;(name: string&): T&<br/>
	    + getAttribute&lt;T&gt;(name: string&, defValue: T&): T&<br/>
	    + setAttribute&lt;T&gt;(name: string&, value: T&): bool<br/>
	    + addAttribute&lt;T&gt;(name: string&, value: T&): bool<br/>
      </td></tr></table>
      
      <p>Example usage:</p>
      <div class="fragment"><pre>
	  Event myEvent;
	  myEvent.addAttribute&lt;float&gt;("myFloatAttribute", (float)0.3);
	  myEvent.setAttibute&lt;float&gt;("myFloatAttribute", (float)0.4);
	  std::cout &lt;&lt; myEvent.getAttribute&lt;float&gt;("myFloatAttribute") &lt;&lt; std::endl;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// output is "0.4"
	  std::cout &lt;&lt; myEvent.getAttribute&lt;long&gt;("myLongAttribute", (long)5) &lt;&lt; std::endl;&nbsp;// output is "5"
	  std::cout &lt;&lt; myEvent.getAttribute&lt;short&gt;("myShortAttribute") &lt;&lt; std::endl;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// invalid argument exception
      </pre></div>
      
      <h2>Event Utility Node</h2>
      <p>A node called EventUtilityNode was introduced to allow modification of events at any point in the configuration tree. According to the node settings provided by the configuration file, event attributes can be renamed, created, or deleted. The renaming capabilities can be used to resolve naming incompatibilities between several nodes.</p>
      
      <p>Example usage:</p>
      <div class="fragment"><pre>
	  &lt;EventUtilityNode rename="myFloatAttribute floatAttr" create="short myShortAttribute 6" discard="myLongAttribute"&gt;
	  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;any event generator ...&gt;
	  &lt;/EventUtilityNode&gt;
      </pre></div>

      <h2>Basic Data Types For Event-Attributes</h2>
      <p>A large variety of different data types for event attributes is supported and this set of types can be further extended. The currently supported types and their generic names are:</p>
      
      <table border="1" align="center">
	<tr><th>Type</th><th>Generic Type Name</th></tr>
	<tr><td>bool</td><td>bool</td></tr>
	<tr><td>char</td><td>char</td></tr>
	<tr><td>signed char</td><td>signed_char</td></tr>
	<tr><td>unsigned char</td><td>unsigned_char</td></tr>
	<tr><td>int</td><td>int</td></tr>
	<tr><td>long</td><td>long</td></tr>
	<tr><td>short</td><td>short</td></tr>
	<tr><td>unsigned int</td><td>unsigned_int</td></tr>
	<tr><td>unsigned long</td><td>unsigned_long</td></tr>
	<tr><td>unsigned short</td><td>unsigned_short</td></tr>
	<tr><td>double</td><td>double</td></tr>
	<tr><td>long double</td><td>long_double</td></tr>
	<tr><td>float</td><td>float</td></tr>
	<tr><td>std::string</td><td>string</td></tr>
	<tr><td>std::vector&lt;float&gt;</td><td>vector&lt;float&gt;</td></tr>
      </table>
      
      <h2>Add New Data Types</h2>
      <p>Support for other basic data types as well as higher level types defined by the user can easily be added. Assuming that you have written your own class MyClass and want to use it with OpenTracker now, you just need to follow three simple steps:</p>
      
      <ol>
	<li>
	  Provide a standard constructor for the new class, which does not take any arguments:
	  <div class="fragment"><pre>MyClass() { ... }</pre></div>
	</li>
	
	<li>
	  Provide input and output streaming operators for your class, which will be used for serialization and de-serialization for file logging and network support:
	  <div class="fragment"><pre>
	      istream& operator>>(istream &amp;in, MyClass &amp;obj);
	      ostream& operator<<(ostream &amp;out, const MyClass &amp;obj);
					  </pre></div>
	  <p>Streaming operators are to be implemented outside the class, but preferably declared within the same header file, so that they are known wherever the new type is used. Streaming operators for the most important STL container classes (i.e., vectors and lists) were implemented as a part of OpenTracker and can be found in src/core/iostream_ext.h. This is also a good starting point to see how to implement streaming operators.</p>
	</li>
	
	<li>
	  Make your class known to the OpenTracker library:
	  <div class="fragment"><pre>
	      Event::registerGenericTypeName((MyClass*)NULL, "GenericNameOfMyClass");
	  </pre></div>
	  <p>This method must be called before the type is used by your code for the first time. Usually, it is a good idea to register type names in some kind of initialization method of your code. The generic type name can be chosen arbitrarily but must be unique.</p>
	</li>
      </ol>

<?php require "../footer.php"; ?>
</body>
</html>

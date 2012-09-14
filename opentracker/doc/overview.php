<?php require "header.php"; ?>


<h1 align="right">OpenTracker</h1>

<p align="left">Tracking is an indispensable requirement for all kinds of virtual 
  reality (VR) and augmented reality (AR) systems. While the quality of tracking, 
  in particular the need for high performance and fidelity have led to a large 
  body of past and current research, little attention is typically paid to software 
  engineering aspects of tracking software.</p>
<h2 align="left">What current tracker software provides </h2>
<ul>
  <li>Device Abstraction - Interfaces are defined to provide the same interface 
    for different devices</li>
  <li>Linear Extensibility - It is typically very simple to provide new implementations 
    of the same interfaces to support new devices.</li>
</ul>
<h2>What current tracker software lacks</h2>
<ul>
  <li> support for complex or experimental configurations - non-trivial setups, 
    e.g. requiring combination of data from different sources, have to be coded 
  </li>
  <li> non-trivial geometric modifications - e.g. interpreting the data of one 
    device relative to another</li>
  <li>transparent network access - simple reconfiguration of server devices</li>
  <li>filters for data transformations 
    <ul>
      <li>geometric - relocate tracker origin</li>
      <li>semantic - fuse data from two sources</li>
    </ul>
  </li>
  <li>object-oriented hierarchical extensibility - allowing to add stuff on every 
    level </li>
  <li>simple end user reconfiguration - specification of configuration via graphical 
    tools</li>
</ul>
<h2>Solution - OpenTracker provides ...</h2>
<p>OpenTracker is developed to be generic solution to the different tasks involved 
  in tracking input devices and processing tracking data for virtual environments. 
  It provides an open software architecture based on a highly modular design and 
  a configuration syntax based on <a href="http://www.w3c.org/XML/">XML</a>, thus 
  taking full advantage of this new technology. OpenTracker is a first attempt 
  towards a &quot;write once, track anywhere&quot; approach to virtual reality 
  application development. Some features of OpenTracker include :</p>
<ul>
  <li>a generic data flow network - to describe complex manipulations of data 
    as a series of simple transformations</li>
  <li>based on an object-oriented graph structure</li>
  <li>an extensible C++ class library</li>
  <li>transparent network access</li>
  <li>XML-based configuration</li>
  <li>Flexible event model 
    <ul>
      <li>event based : event data is passed through a graph of nodes</li>
      <li>time based : continous functions in time can be configurated and evaluated</li>
    </ul>
  </li>
</ul>
<h3>Why XML ?</h3>
<ul>
  <li>XML is a meta language for any kind of hierarchical ( graph like ) information</li>
  <li>OpenTracker is based on a regular XML document type definition</li>
  <li>XML standard tools are available for : 
    <ul>
      <li>Editing : end users employ a visual editor for creating tracker configurations</li>
      <li>Parsing &amp; Type Checking : a standard XML parser reads the configuration 
        file, checks its consistency, and builds a runtime data structure.</li>
      <li>Documentation : Automatic generation of graphical documentation is easily 
        possible. </li>
    </ul>
  </li>
</ul>
<p align="center"><img src="Images/bigconfig.gif" width="886" height="456"><br>
  <b>A large data flow graph. This figure was automatically created from the XML 
  configuration file.</b> </p>
<p align="center">&nbsp;</p>
<p align="center">&nbsp;</p>
<h1>Applications</h1>
<p align="left">We use OpenTracker in several different setups as our main software 
  for dealing with tracking data. Some Examples (click on the pictures for larger 
  versions).</p>
<table width="100%" border="0" cellspacing="2" cellpadding="2">
  <tr>
    <td valign="top"><h4>Fusion of different tracking devices</h4>
        <p>In this setup the pen is tracked with a magnetic tracker, whereas the 
          tablet is tracked using an optical tracking system relying on markers. 
          The border of the marker is visible below the overlayed graphic of the 
          tablet. </p></td>
    <td><a href="Images/pip&amp;pen_cropped.jpg"><img src="Images/pip&amp;pen_cropped.jpg" width="125" height="90" lowsrc="Images/pip&amp;pen_cropped.jpg" border="0" /></a></td>
  </tr>
  <tr>
    <td valign="top"><h4>Mobile platform for collaborative Augmented Reality</h4>
        <p>This figure shows our developing mobile platform for Augmented Reality. 
          It poses demanding requirements to the tracking software. Different input 
          devices such as a web camera for optical tracking, a graphics tablet providing 
          2D information and orientation sensors are combined to provide a full 
          3D interactive user interface. </p>
      <p>The data flow graph shown in the figure before is a visualization of 
        an early tracking software configuration for this platform.</p></td>
    <td><a href="Images/ivan_pipaction_side.jpg"><img src="Images/ivan_pipaction_side.jpg" width="125" border="0" /></a></td>
  </tr>
  <tr>
    <td valign="top"><h4>Collaboration between mobile and stationary user</h4></td>
    <td><a href="Images/chessImage3.jpg"><img src="Images/chessImage3.jpg" width="125" border="0" /></a></td>
  </tr>
</table>
<h2 align="left"></h1>
      </h2>
<p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
      <p></p>
</td></tr></table>
</div>

<?php require "footer.php"; ?>

</body></html>

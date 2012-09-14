<?php require "header.php"; ?>

<h1>Installation Guide</h1>
  <p>This is a step-by-step guide to configuring and compiling
    OpenTracker. After the General section, the guide splits into two parts
    - for a Windows and for a Unix environment. </p>
  <h2>General</h2>
  <ol>
    <li>You will need the following libraries to compile OpenTracker.
      Download the libraries form the sites linked to and install them on
      your system. You might need to compile them, depending on the version
      you chose to download. Please refer to documentation of the respective
      library on how to do that. Follow also the following guide lines to
      build these libraries correctly.</li>
  </ol>
  <ul>
    <li><a href="http://xml.apache.org/">Xerces</a> - A validating XML
      parser library for C++ and Java. You can use the binary distribution
      for your system, if you want to avoid building it yourself.</li>
    <li><a href="http://www.cs.wustl.edu/%7Eschmidt/ACE.html">ACE</a> -
      The Adaptive Communication Environment, a wrapper library for system
      services. You will have to build this library for your system. Get one
      of the recent beta versions. In the platform dependend configuration
      header file, define the definition for compiling ACE with the Standard
      C++ library. Currently the define is called <span class="envars">ACE_HAS_STANDARD_CPP_LIBRARY</span>.</li>
  </ul>
  <ol start="2">
    <li>Then download OpenTracker and unpack the distribution file into a
      directory.</li>
  </ol>
  <h2><a name="windows"></a>Windows</h2>
  <ol start="3">
    <li>
      <p>We recommend that you set up the following environment variables
        for your OpenTracker environment (under Windows 2000/NT, use the System
        control panel, Advanced/Environment variables section to do this):</p>
      <ul type="disc">
        <li>Set <span class="envars">OTROOT</span> to the root directory
          of your OpenTracker installation (<span class="filename">c:\dev\opentracker</span>
          in our case). </li>
        <li>Set <span class="envars">ACEROOT</span> to the root
          directory of your ACE_wrappers installation (<span class="filename">c:\dev\ace</span>
          in our case).</li>
        <li>Set <span class="envars">XERCESCROOT</span> to the root
          directory of your Xerces installation (<span class="filename">c:\dev\xerces</span>
          in our case).</li>
        <li>Make yourself sure that your <span class="envars">PATH</span>
          environment variable contains <span class="filename">%ACEROOT%\bin</span>
          and <span class="filename">%XERCESCROOT%\bin</span>.</li>
        <li>Set any additional evironment variables for additional device
          drivers:</li>
        <ul>
          <li>ARTOOLKITROOT for the ARToolkit library</li>
          <li>VRPNROOT for VRPN inclusion-<br>
          </li>
        </ul>
      </ul>
      <p><br>
        Of course you can work without this setup and edit your project files
        to fit your particular needs. However, we do not recommend this as we
        will continue to distribute pre-configured Visual C++ workspace files
        that you would need to manually edit for every new version of
        OpenTracker. </p>
    </li>
    <li>
      <p>Open the file config_win.h and uncomment the defines for any
        additional code you want to compile. This may include device drivers or
        other nodes. Typically these classes depend on special libraries that
        need to be present. Look at the documentation of those classes to see
        how to setup the build process. Also make sure the include files and
        libraries can be found by the compiler. Either add them to the project
        files or better still to the following environment variables :</p>
      <ul type="disc">
        <li>Set <span class="envars">INCLUDE</span> to any include
          directories you will need to compile in for support of additional
          libraries.</li>
        <li>Set <span class="envars">LIB</span> to any lib directories
          you will need to link with. </li>
      </ul>
      <p>&nbsp;</p>
    </li>
    <li> Compiling the current version of OpenTracker should be a very
      straightforward task. If you have installed the necessary external
      libraries in proper places as specified above, the only step you need
      would be to open <span class="filename">opentracker.dsw</span> located
      in the topmost directory of the OpenTracker source tree and perform a
      Batch build that builds the OpenTracker library plus the standalone
      program. </li>
  </ol>
  <p>The build process builds a DLL for OpenTracker, either called <span
 class="filename">opentracker.dll</span> or <span class="filename">opentrackerd.dll</span>
    for the debug version and a standalone program called <span
 class="filename">opentracker.exe</span> to use as standalone tracking
    server. All output files will be in <span class="filename">%OTROOT%\bin</span>
    and the library files in <span class="filename">%OTROOT%\lib</span>.</p>
  <h2><a name="unix"></a>Unix</h2>
  <ol start="3">
    <li>
      <p>Go to the root directory of your OpenTracker installation.</p>
    </li>
    <li>
      <p>Run ./configure to configure the build system, check for any
        additional libraries you want to use</p>
    </li>
    <li>
      <p>run make to compile OpenTracker</p>
    </li>
    <li>
      <p>run make install to install it your system.</p>
    </li>
    <li>
      <p>Make sure that any shared libraries OpenTracker needs ( such as
        ACE and Xerces ) are accessible ( in a lib directory or in <span
 class="envars">LD_LIBRARY_PATH</span> ) so that the binaries will be
        able to execute.</p>
    </li>
  </ol>
  <p>The build process generates a static and shared version of the
    library called <span class="filename">libOpenTracker</span> which will
    be in <span class="filename">$OTROOT\lib</span>. It also generates a
    standalone program called <span class="filename">opentracker</span> in
  <span class="filename">$OTROOT\bin</span>.</p>
  <h2>Documentation</h2>
  <p>The source documentation can be generated from the sources using
    doxygen. If you have installed doxygen in your path, you can let the
    build system do that for you.</p>
  <p>On Unix execute "make doc" to build the source documentation. It
will reside in <span class="filename">doc\html</span>.</p>

</div>

<?php require "../footer.php"; ?>

</body></html>

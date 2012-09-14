<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/2000/REC-xhtml1-20000126/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<title>Studierstube Augmented Reality Project</title>
<link rel="stylesheet" href="style.css" type="text/css">
<style type="text/css">
<!--
.style12 {color: #CC0000}
-->
</style>
</head>

<body bgcolor="#FFFFFF" text="#000000">

<?php require "title.php"; ?>

<div id="links">
<?php require "left.php"; ?>
</div>

<div id="inhalt">
<table border="0" cellspacing="0" cellpadding="15">

    <tr> 
        <td width="956"><h1>External Libraries</h1>
            <p>This page contains information about the different libraries that 
                OpenTracker or modules of it depend on.</p>
            <h2>Required Libraries</h2>
            <ul>
                
        <li><a href="http://xml.apache.org/">Xerces</a> - A validating XML parser 
          library for C++ and Java, developed by the Apache project. OpenTracker 
          uses this library to parse it's configuration files. Always use the 
          latest version !</li>
                
        <li><a href="http://www.cs.wustl.edu/%7Eschmidt/ACE.html">ACE</a> - The 
          Adaptive Communication Environment, a wrapper library that provides 
          platform independent access to system resources, such as network sockets, 
          threads, shared memory. Always use the latest version !</li>
            </ul>
            <h2>Additional Libraries for Device Support</h2>
          <ul>
        <li><a
          href="http://www.hitl.washington.edu/research/shared_space/">ARToolkit</a>
          - The ARToolkit is a library to track the location of
          fiducial markers within a space captured by a video
          camera. This library is necessary to make the
          ARToolKitModule work. <br> It uses DirectShow to access any
          kind of video cameras. Make sure to use the
          new <a
          href="download/contrib/arvideolib-2.65-ds.zip">ARVideoLib</a>
          version! <br> Set the environment variable ARTOOLKITROOT to the
          directory so that the build system can find it.</li>
        <li><a
          href="http://www.studierstube.org/handheld_ar/artoolkitplus.php">ARToolkitPlus</a>
          - ARToolkitPlus is a C++ based, optimized and extended
          version on ARToolKitlibrary within a space captured by a
          video camera. This library is necessary to make the
          ARToolKitPlusModule work. <br> For Windows platforms you can
          download the newest version at the OpenTracker
          <a href="download/">download</a> page.  <br>It uses DirectShow
	    to access any kind of video cameras. Make sure to use the
	    new <a
	    href="download/contrib/arvideolib-2.65-ds.zip">ARVideoLib</a>
	    version! <br> Set the environment variable ARTOOLKITPLUSROOT
	    to the directory so that the build system can find
	    it.</li>

        <li><a href="download/contrib/UserPort.zip">UserPort</a> - A WindowsNT/2000 
          device driver that allows access to I/O ports. This is required by the 
          ParButtonModule to access the parallel port. You need to allow access 
          to the I/O ports of the parallel port you want to use, this is the same 
          address you configure in the module.</li>
        <li><a href="http://www.pointing.com/">Wintab</a> - a generic API to graphics 
          tablets, supported by most graphic tablet producers. This is required 
          by the WacomGraphireModule.</li>
        <li><a href="http://www.microsoft.com/speech/download/SDK51/">Microsoft 
          Speech SDK</a> - this is required for the SpeechModule. The currently 
          used version is 5.1.</li>
        <li><a href="http://www.alliancedistributors.com/Alliance_Brand/Products/Developers.php">P5Glove</a> 
          - driver libraries for the Essential Reality P5 Glove. You will have 
          to register with their developer program to download the SDK.</li>
           
  </td></tr></table>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
</div>

<?php require "footer.php"; ?>

</body></html>

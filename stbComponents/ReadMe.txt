============================================================================
==       STUDIERSTUBE COMPONENTS  /  STUDIERSTUBE APPLICATIONS            ==
============================================================================

authors: Michael Kalkusch
created: 17-05-2006


Environment varibels:

  STB4COMPROOT = {absolute path}{stbComponents}
  
  
  Variabels from Studierstube_v4 (minimal set):
    
    COINROOT  
    STB4ROOT  
  
    (see Studierstube 4 for details)
  


----------------------------------------------------------------------------
  Windows settings
----------------------------------------------------------------------------
   
 Default output folder:

   $(STB4COMPROOT)/bin/win32/
   $(STB4COMPROOT)/lib/win32/
 
   MSVC Project file settings:
  
    OutputFile="$(STB4COMPROOT)/bin/win32/{project_name}.dll"
    ImportLibrary="$(STB4COMPROOT)/lib/win32/{project_name}.lib"
  
  
----------------------------------------------------------------------------
  Linux settings
----------------------------------------------------------------------------
  
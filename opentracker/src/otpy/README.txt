Environment variable PYTHONPATH is the module search path for Python. In order 
to successfully execute Python code from within OpenTracker, the minimum 
requirement is that PYTHONPATH points to %OTROOT%\bin\win32;%OTROOT%\src\otpy. 
In addition to this search paths for the absolute necessary modules ot and _ot 
the location of the actual Python code module (as specified by the PythonConfig 
tag in the XML configuration file) has to be communicated in a similar way to 
Python.

/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The header file for the Config class.
*
* @author Denis Kalkofen
*
* $Id: Config.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */


#ifndef _CONFIG_H_
#define _CONFIG_H_

class TiXmlElement;

#include <stb/base/macros.h>
#include <stb/base/string.h>

BEGIN_NAMESPACE_STB

class Kernel;


/**@ingroup kernel
* The Config class reads the kernel's configuration file and 'forwards' 
* the xml-elements depending on their xml-tag to their appropriate object.
* the config supports the following xml tags:
* 
*/
class Config{
public:
    /************************************************************************
     * The Constructor
     ************************************************************************/
    Config();
    
    /************************************************************************
     * The Destructor
     ************************************************************************/
    ~Config();

    /*
    * Read the Kernel's configuration file.
    */
    bool readKernelConfig(stb::string nFileName);

protected:
    /*
     * Parses an specific element in the kernels configuration. This function is called with each element in the kernel's configuration file as argument.
     * Depending on the element's value, the readConfiguration() function for the specific element will be called.
     */
    bool parseXMLElement(TiXmlElement* element);
};


END_NAMESPACE_STB


#endif //

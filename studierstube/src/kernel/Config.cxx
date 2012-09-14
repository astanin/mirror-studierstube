/* ========================================================================
 * Copyright (C) 2004-2005  Graz University of Technology
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
 * Graz University of Technology, Institut für Maschinelles Sehen und Darstellen,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: 
 * ======================================================================== */
/** @author   Denis Kalkofen
 *
 * $Id: Config.cpp 131 2005-11-03 20:53:09Z denis $
 * @file                                                                   */
/* ======================================================================= */


#include <stb/kernel/Config.h>
#ifndef USE_TINYXMLMOD
#include <tinyxml/tinyxml.h>
#else
#include <TinyXML/tinyxml.h>
#endif
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/ComponentManager.h>
BEGIN_NAMESPACE_STB

//================Config===========
Config::Config()
{
}

Config::~Config()
{
}

/// parse the xml file
bool
Config::readKernelConfig(stb::string filename)
{ 
    stb::logPrintD("Stb:: parsing " + filename + " ...\n");
	TiXmlDocument* document = new TiXmlDocument();
	
	if(!document->LoadFile(filename.c_str()))
	    {
		    logPrintE("An error occured during parsing " + filename + "\n");
		    logPrint(" Message: ");
		    logPrint(document->ErrorDesc());
		    logPrint("\n");
		    return false;
	    }
	
	TiXmlElement* root = document->RootElement();
	if(!stb::stricasecmp(root->Value(),"studierstube"))
	{
		////parse file
		TiXmlElement* element = root->FirstChildElement();
		while(element)
		{
			parseXMLElement(element);
			element = element->NextSiblingElement();
		}
	}
	/////////	parsing is done -> clean up 
	document->Clear();
	delete document;
	return true;
}


bool
Config::parseXMLElement(TiXmlElement* element)
{
	///////////////// Kernel /////////////////
	if(!stb::stricasecmp(element->Value(),"Kernel")){
		Kernel::getInstance()->parseConfiguration(element);
	}
	///////////////// Component / Application /////////////////
	else if(!stb::stricasecmp(element->Value(),"Component")
         || !stb::stricasecmp(element->Value(),"Application")
           )
	{
		ComponentInfo *compInfo=new ComponentInfo();
		compInfo->readConfiguration(element);
        Kernel::getInstance()->getComponentManager()->addComponent(compInfo);
	}

	return true;
}

END_NAMESPACE_STB

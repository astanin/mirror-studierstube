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
/* @author Denis Kalkofen
*
* $Id: ComponentRetriever.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/kernel/ComponentRetriever.h>
#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/Kernel.h>
#include <stb/base/OS.h>

BEGIN_NAMESPACE_STB

/**
*     The Constructor	
*/
ComponentRetriever::ComponentRetriever()
{

}

/**
*     The destructor.
*/
ComponentRetriever::~ComponentRetriever()
{
}

Component* 
ComponentRetriever::getComponent(ComponentInfo *compInfo)
{
	////load lib
    stb::string libName=compInfo->getLibName();

//FIX ME: we should remove the gui-binding from the kernel -denis
//#ifdef STB_IS_WINDOWS
//    if(libName=="stbviewer")
//    {
//        switch(stb::Kernel::getInstance()->scheduler->getCurrentGuiBinding())
//        {
//            case SOWIN::
//                break;
//
//        }
//       
//    }
//#endif
///////////

    char padding[4];
    Component* newComponent=NULL;
	if(libName=="")
		return NULL;
	hModule libHandle;
	os_correctModuleName(libName, true,true);

    if (libName.size()<16) strcpy(padding,"\t\t\t");
    else strcpy(padding,"\t\t");

    logPrintS("Loading component %s %s ", libName.c_str(), padding);

	libHandle = os_LoadLibrary(libName);
	if(!libHandle){
        logPrint("[ X ]\n");
		logPrintE("Couldn't load " + libName + "\n");
        return NULL;
	}
	compInfo->setHModule(libHandle);
	Component* (*createComponent)()=(Component*(*)())os_GetProcAddress(libHandle,"createComponent");
    if(!createComponent)
    {
        logPrint("[ X ]\n");
        logPrintE("Can't find createComponent() in " + libName + "\n");
        return NULL;
    }
	newComponent=(*createComponent)();
    newComponent->setComponentInfo(compInfo);
    logPrint("[ OK ]\n");
    //logPrintI("Component %s loaded\n", libName.c_str());
	return newComponent;
}




END_NAMESPACE_STB

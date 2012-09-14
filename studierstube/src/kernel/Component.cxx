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
/** The cxx file for the Component class.
*
* @author Denis Kalkofen
*
* $Id: Component.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/kernel/Component.h>
#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/Kernel.h>

BEGIN_NAMESPACE_STB

Component::Component():
	kernel_(stb::Kernel::getInstance()),
	compInfo_(NULL),
    isInit(false)
{
}

Component::~Component()
{
    printf ("~Component");
    if(compInfo_){
		delete compInfo_;
    }
}

stb::string 
Component::getTypeID()
{
	return compInfo_->typeID;
}


void 
Component::setParameter(stb::string /*key*/, std::string /*value*/)
{

}

void 
Component::setComponentInfo(ComponentInfo* info)
{
    this->compInfo_=info;
}

void 
Component::retrieveParameter()
{
    if(!compInfo_)
        return;

    std::map<stb::string,stb::string>::iterator element;
    element = compInfo_->parameter.begin();
    for (int i=0;i<(int)compInfo_->parameter.size();i++) 
    {
        setParameter((*element).first,(*element).second);
        element++;
    }

}


END_NAMESPACE_STB

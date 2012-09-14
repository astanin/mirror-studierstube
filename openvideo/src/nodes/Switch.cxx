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
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The source file for the Switch class.
  *
  * @author Daniel Kopeinigg
  * 
  * $Id: Switch.cxx 
  * @file                                                                   
  * ======================================================================= */

 #include <openvideo/nodes/Switch.h>
#include <openvideo/openVideo.h>
#include <openvideo/nodes/VideoSinkSubscriber.h>

#ifdef  ENABLE_SWITCH

asdfa
#include <openvideo/State.h>
#include <openvideo/Manager.h>
namespace openvideo {

Switch::Switch()
{
	name = typeName = "Switch";
	isRunning=false;
	internalFormat=0;
}

void 
Switch::initPixelFormats()
{
	//format_r8g8b8	= 0,
	//format_b8g8r8	= 1,
	//format_r8g8b8x8	= 2,
	//format_b8g8r8x8	= 3,
	//format_l8		= 5,
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8X8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8X8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_L8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R5G6B5));
}

Switch::~Switch()
{

}


void
Switch::init()
{
	//
	//////////////////////////////////////////////
	isRunning=true;
	state=inputs[0]->getState();
}

void
Switch::switchToInput(int inputIndex)
{
	//
	//////////////////////////////////////////////
	printf("Switching TO INPUT Number %i \n",inputIndex);
	isRunning=true;
	state=inputs[inputIndex]->getState();
}

 

 

void
Switch::process()
{
	// send update notification to subsrcibers
	//for(int i=0;i<subscribers.size();i++)
	for(int i=subscribers.size()-1; i>=0;i--)
		subscribers[i]->update(state);
}

void
Switch::postProcess()
{
	/*for(int i=0;i<subscribers.size();i++)
	{
	if(subscribers[i]->isResourceInUse())
	{
	//wait till 
	}
	}*/
}

void 
Switch::subscribe(VideoSinkSubscriber* aSubscriber)
{
	subscribers.push_back(aSubscriber);
}

void 
Switch::unsubscribe(const VideoSinkSubscriber* aSubscriber)
{
	std::vector<VideoSinkSubscriber*> subscribersCopy(subscribers);
	for (int i = subscribers.size() - 1;i >= 0;i --)
		if (subscribersCopy[i] == aSubscriber)
			subscribers.erase(subscribers.begin() + i);
}
}
#endif  //ENABLE_Switch


 

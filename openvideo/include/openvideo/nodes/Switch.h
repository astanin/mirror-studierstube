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
/** The header file for the Switch Class
  *
  * @author Daniel Kopeinigg
  *
  * $Id: Switch.h 
  * @file                                                                   */
 /* ======================================================================= */
#ifndef _SWITCH_H
#define _SWITCH_H

 


#include <openvideo/openVideo.h>
#ifdef ENABLE_SWITCH
#include <openvideo/Node.h>


 



namespace openvideo {
	class VideoSinkSubscriber;

	/**
	*@ingroup nodes
	*	This node implements an publish-subsribers pattern to pass the video to any number of registered consumers.
	*	The node can be used to integrate an OpenVideo sink into another programm. 
	*
	*   Supported Pixel Formats:
	*	@li: R8G8B8
	*	@li: B8G8R8
	*	@li: R8G8B8X8
	*	@li: B8G8R8X8
	*	@li: L8
	*/
	class OPENVIDEO_API Switch : 
		public openvideo::Node
	{
	public:
		/**
		*	constructor
		*/
		Switch();

		/**
		*	destructor
		*/
		~Switch();

		/**
		*	
		*/
		virtual void init();

		/**
		*	
		*/   
		virtual void process();

		virtual void postProcess();

		virtual void initPixelFormats();

		void subscribe(VideoSinkSubscriber* aSubscriber);
		
		
		void switchToInput( int inputIndex );
	 

		void unsubscribe(const VideoSinkSubscriber* aSubscriber);

		/**
		*	indicates whether the sink is started or not.
		*/
		bool isStarted() const { return isRunning; }

		/**
		*	wait synchronously until sink is started.
		*/
		void waitStarted() const { while (! isRunning); }

	protected:
		volatile bool isRunning;

		/**
		*	Internal texture format. defines the number of color components in the texture. valid values = 1,2,3, or 4.
		*/
		int internalFormat;

		std::vector<VideoSinkSubscriber*> subscribers;
	};

} //namespace openvideo {

#endif // 

#endif

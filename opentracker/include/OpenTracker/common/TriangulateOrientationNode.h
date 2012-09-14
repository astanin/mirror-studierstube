/* ========================================================================
* TODO Uni-blabla
* ========================================================================
* PROJECT: OpenTracker
* ======================================================================== */
/** header file for Merge Tracker Node.
*
* @author Gratl/Werlberger VRVU
*
* $Id:$
* @file                                                                   */
/* ======================================================================= */

/**
* @page Nodes Node Reference
* @section TriangulateOrientation TriangulateOrientation
* TODO description
*
* inputs :
* 
* TODO
* 
* An example element using all wrapper elements looks like this :
* @verbatim
TODO example


Order of elements on the triangle:
First element ist top element of wooden triangle
Second element ist left element of wooden triangle
Third element ist right element of wooden triangle


<TriangulateOrientation>
  <EventTransform DEF="top" scale="-1 1 -1" >
      <UbisenseSource DEF="testar1" object="Tag 016-000-002-104"/>
  </EventTransform>
  <EventTransform DEF="bLeft" scale="-1 1 -1" >
      <UbisenseSource DEF="testar6" object="Tag 016-000-002-111"/>
  </EventTransform>
  <EventTransform DEF="bRight" scale="-1 1 -1" >
      <UbisenseSource DEF="testar9" object="Tag 016-000-002-112"/>
  </EventTransform>
</TriangulateOrientation>







*/

#ifndef _TRIANGULATEORIENTATIONNODE_H
#define _TRIANGULATEORIENTATIONNODE_H

#include "../OpenTracker.h"

/**
*
*/

namespace ot
{

	class OPENTRACKER_API TriangulateOrientationNode : public Node   
	{

	protected:
		//! constructor method
		TriangulateOrientationNode();

		//! states of top tracking device
	    Event topState;

		//! states of top tracking device
		Event bLeftState;

		//! states of top tracking device
		Event bRightState;

		//! merged state
		Event mergedState;

	public: 

		/** tests for EventGenerator interface being present and returns
		* 1, if present.
		* @return always 1 */
		virtual int isEventGenerator()
		{
			return 1;
		}

		/**
		* This method is called by any child node. It updates the local
		* state with the right part of the passed event and notifies
		* it's parent and references in turn.
		* @param event the event value passed
		* @param generator the node generating the event
		*/
		virtual void onEventGenerated( Event& event, Node & generator); 
		void pushEvent();
		void pullEvent();

		friend class CommonNodeFactory;
	};

} // namespace ot

#endif

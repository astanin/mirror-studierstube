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
/** The header file for the SoViewport.h class.
*
* @author Denis Kalkofen
*
* $Id: SoViewport.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */


#ifndef SOVIEWPORT_H
#define SOVIEWPORT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/actions/SoGLRenderAction.h> 
#include <Inventor/SbLinear.h>
#include <Inventor/fields/SoSFVec2s.h> 
#include <stb/base/macros.h>
#include <stb/components/viewer/ViewerDefines.h>


BEGIN_NAMESPACE_STB


class VIEWER_API SoViewport : public SoNode
{
SO_NODE_HEADER(SoViewport);

public:
	SoViewport(void);

	static void initClass();


	/** Sets the size and the origin of the viewport region */
	void setViewportRegion(SbVec2s origin, SbVec2s size);

	SoSFVec2s origin;
	SoSFVec2s size;


	/** Returns the region's origin*/
	SbVec2s getOrigin();

	/** Returns the region's size*/
	SbVec2s getSize();

protected:
	~SoViewport(void);
	/** Overrides the inherited method to implement a new
        SoGLRenderAction behavior. 
        @param action the SoGLRenderAction calling this method
    */
    virtual void GLRender(SoGLRenderAction* action);
	
	/**
		reads an instance from soinput and sets up the viewport's parameters.
	*/
	virtual SbBool readInstance(SoInput *in, unsigned short flags);


private:
	/** The viewport's region to be used*/
	SbViewportRegion viewportRegion;

	/** Indicates whether the node is going to change the viewport's settings or not.
		The default value is set to false. By calling 'setViewportRegion(...)' the value changes to true and enables setting the viewport.
		*/
	SbBool changeViewportRegion;
	
};


END_NAMESPACE_STB


#endif

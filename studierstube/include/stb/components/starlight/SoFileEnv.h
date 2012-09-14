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
/** The header file for the SoFileEnv
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoFileEnv.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOFILEENV_H_
#define _SOFILEENV_H_


/*
* --------------------------------------------------------------------------------
* Includes
* --------------------------------------------------------------------------------
*/
#include <Inventor/nodes/SoFile.h>
#include <Inventor/fields/SoSFString.h>
#include "starlight.h"

/** 
*
*
* @author Erick Mendez
* @ingroup starlight
* 
*/

class STARLIGHT_API SoFileEnv: public SoFile
{
    SO_NODE_HEADER(SoFileEnv);
public:
    /// Initializes the node kit
    static void initClass();

    /// The constructor of the class
    SoFileEnv();

    /// Destructor
    ~SoFileEnv();

    SoSFString envPath;
    SoSFString fileName;

protected:

    virtual SbBool readNamedFile(SoInput * in);

};


#endif //_SOFILEENV_H_

/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
/** Implementation of class MultRotRot
  *
  * @author Rainer Splechtna
  *
  * $Id: MultRotRot.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SbLinear.h>
#include <stb/components/viewer/controlmode/math2.h>
#include <stb/components/viewer/controlmode/MultRotRot.h>

BEGIN_NAMESPACE_STB


SO_ENGINE_SOURCE(MultRotRot);

void
MultRotRot::initClass() 
{
    SO_ENGINE_INIT_CLASS(MultRotRot, SoEngine, "Engine");
}

MultRotRot::MultRotRot() 
{
    SO_ENGINE_CONSTRUCTOR(MultRotRot);

    // define input fields and their default values
    SO_ENGINE_ADD_INPUT(rotationA, (SbRotation::identity()));
    SO_ENGINE_ADD_INPUT(rotationB, (SbRotation::identity()));

    // define the output
    SO_ENGINE_ADD_OUTPUT(product, SoMFRotation);
    SO_ENGINE_ADD_OUTPUT(inverse, SoMFRotation);
}



MultRotRot:: ~MultRotRot()
{
    // NIL
}

void MultRotRot::evaluate()
{
    int rotANum=rotationA.getNum();
    int rotBNum=rotationB.getNum();
    int maxNum=(rotANum && rotBNum) ? m2max(rotANum,rotBNum) : 0;

    SO_ENGINE_OUTPUT(product,SoMFRotation,setNum(maxNum));
    SO_ENGINE_OUTPUT(inverse,SoMFRotation,setNum(maxNum));

    if (product.getNumConnections() || inverse.getNumConnections()){
        for (int i=0;i<maxNum;i++){
            SbRotation prod = rotationA[m2min(i,rotANum-1)]*rotationB[m2min(i,rotBNum-1)];
            SbRotation inv = prod.inverse();
            SO_ENGINE_OUTPUT(product, SoMFRotation, set1Value(i,prod));
            SO_ENGINE_OUTPUT(inverse, SoMFRotation, set1Value(i,inv));
        }
    }
}

END_NAMESPACE_STB

/* ===========================================================================
    End of MultRotRot.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */

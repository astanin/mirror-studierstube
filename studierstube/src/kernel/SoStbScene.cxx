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
/** The header file for the SoStbScene class.
  *
  * @author Denis Kalkofen
  *
  * $Id: SoStbScene.cxx 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/base/fixWinCEIssues.h>
#include <stb/kernel/SoStbScene.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/SceneManager.h>

#include <Inventor/nodes/SoSeparator.h>

BEGIN_NAMESPACE_STB

SO_NODE_SOURCE(SoStbScene);


void
SoStbScene::initClass()
{ 
   SO_NODE_INIT_CLASS(SoStbScene, SoGroup, "SoGroup");

}

SoStbScene::SoStbScene()
{
   SO_NODE_CONSTRUCTOR(SoStbScene);
   SoSeparator* stbSceneRoot=stb::Kernel::getInstance()->getSceneManager()->getSceneRoot();  
   addChild(stbSceneRoot);
}

END_NAMESPACE_STB

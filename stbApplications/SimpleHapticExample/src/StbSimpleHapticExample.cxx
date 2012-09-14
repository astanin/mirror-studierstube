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
/* @author Christian Fast
 *
 * @file                                                                   */
/* ======================================================================= */

#include <SimpleHapticExample/StbSimpleHapticExample.h>

CREATE_COMPONENT_FUNC(SimpleHapticExample)

  using namespace stb;

/******************************************************************************
 Event callback triggered when styus button is depressed.  Called in collision
 thread to avoid kick.  (If the application instead waits to handle the button
 down in the client thread, the user can get a kick when the constraint is
 set if the user's hand moved from the position that the button down was
 originally recorded at.)
******************************************************************************/


SimpleHapticExample::SimpleHapticExample() {

  isInit=false;
  needEvent=false;
  needViewer=false;
  needStarlight=false;
  needHaptic=false;

}

SimpleHapticExample::~SimpleHapticExample() {
  //nil
}



void
SimpleHapticExample::setParameter(stb::string key, std::string value) {

  if(key=="sceneFile") {
    sceneFile=value;
  }
  else if(key=="needEvent" && value=="true") {
    needEvent=true;
  }
  else if(key=="needViewer" && value=="true") {
    needViewer=true;
  }
  else if(key=="needStarlight" && value=="true") {
    needStarlight=true;
  }
  else if(key=="needHaptic" && value=="true") {
    needHaptic=true;
  }
}

/// Called before the application is destructed.
bool
SimpleHapticExample::init() {

  if(isInit)
    return true;

  //get viewer's parameter
  retrieveParameter();

  if(needHaptic) {
    Haptic* haptic=(Haptic*)(Kernel::getInstance()->getComponentManager()->load("Haptic"));
    if(!haptic)
      return false;
  }

  if(needStarlight) {

    Starlight* starlight=
      (Starlight*)(stb::Kernel::getInstance()->getComponentManager()->load("Starlight"));
    if(!starlight)
      return false;
  }

  if(needEvent) {
    Event* event=(Event*)(Kernel::getInstance()->getComponentManager()->load("Event"));
    if(!event)
      return false;
  }

  if(needViewer) {
    Viewer* viewer=(Viewer*)(Kernel::getInstance()->getComponentManager()->load("Viewer"));
    if(!viewer)
      return false;
  }

  //load iv file
  if(sceneFile.size()<1)
    return false;

  SoInput::addDirectoryFirst("./");
  SoInput myinput;

  if (!myinput.openFile(sceneFile.c_str())) {

    SoInput::removeDirectory("./");
    return false;
  }
  root=SoDB::readAll(&myinput);
  myinput.closeFile();
  SoInput::removeDirectory("./");

  if (root==NULL) {
    return false;
  }

  //register scene
  registerScene();
  isInit=true;

  std::cerr << "Application initialized" << std::endl;

  return isInit;
}

/// Called before the application is destructed.
void
SimpleHapticExample::shutDown() {

  root->unref();
}

void
SimpleHapticExample::update() {

}

void
SimpleHapticExample::registerScene() {

  stb::Kernel::getInstance()->getSceneManager()->registerApp(root);
}

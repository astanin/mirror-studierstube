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
/** The header file for the SimpleHapticExample class.  
 *  
 * @author Christian Fast
 *  
 * @file                                                                   */  
/* ======================================================================= */  

#ifndef _SimpleHapticExample_H_
#define _SimpleHapticExample_H_

#include <stb/kernel/Kernel.h>
#include <stb/kernel/Application.h>
#include <stb/kernel/SceneManager.h>
#include <stb/components/event/event.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/starlight/starlight.h>
#include <stb/components/haptics/Haptic.h>

#include <Inventor/SbTime.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/errors/SoDebugError.h>

#include <cstdio>
#include <iostream>

#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>
#include <HL/hl.h>
#include <HLU/hlu.h>

namespace stb {
  /**
   *	
   */
  class SimpleHapticExample : public stb::Application
    {
    public:
      /**
       *     The Constructor	
       */
      SimpleHapticExample();

      /**
       *     The destructor.
       */
      virtual ~SimpleHapticExample();

      /// Called before the application is destructed.
      virtual bool init();

      /// Called before the application is destructed.
      virtual void shutDown();

      virtual void update();

      virtual void registerScene();
    
      virtual void setParameter(stb::string key, std::string value);

    protected:	

      SoSeparator* root;
      stb::string sceneFile;
      bool needEvent;
      bool needViewer;
      bool needStarlight;
      bool needHaptic;

	
    };// class 

} //namespace



#endif // _SimpleHapticExample_H_

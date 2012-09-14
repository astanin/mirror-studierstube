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
/** The source file for the SoVBOMesh.
*
* @author Denis Kalkofen
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoVBOMesh.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef  _SO_VBOMESH_
#define  _SO_VBOMESH_

#define NO_VBOMESH
#ifndef NO_VBOMESH // This node is optional since it depends on GLEW

#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <stb/kernel/StbLogger.h>

#include "starlight.h"

#include <vector>
#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif
#include <gl/glew.h>
#include <gl/GL.h>

#    if defined(_DEBUG) || defined(DEBUG)
#      pragma message("SoVBOMesh: linking against glew32d.lib")
#      pragma comment( lib, "glew32d.lib" )
#    else
#      pragma message("SoVBOMesh: linking against glew32.lib")
#      pragma comment( lib, "glew32.lib" )
#    endif 

/** 
 * This class is capable of taking an IndexedFaceSet and/or an SoCoordinate3 
 * and convert it to vertex buffer objects.
 * This class is mainly useful if the vertices are changed during runtime
 * or if we desire to render a high number of polygons (30,000+)
 *
 */

class STARLIGHT_API SoVBOMesh : public SoIndexedFaceSet
{
   SO_NODE_HEADER(SoVBOMesh);
public:
    SoVBOMesh();
    ~SoVBOMesh();

   static void initClass();

   // The IndexedFaceSet should be given here
   SoSFNode faceset;

   // And the SoCoordinate3 should be given here
   SoSFNode coords;

   // Invert the direction of the normals if true
   SoSFBool invertNormals;

protected:
   virtual void GLRender(SoGLRenderAction * action);

   bool isInit;
   virtual void init();
   virtual void buildVBO();

   GLuint vboVertices;
   GLuint vboNormals;

   struct Vertex {float x,y,z;};
   int      vertexCount;
   Vertex*  vertexList;

   struct Normal{float nx,ny,nz;};
   Normal* normalList; 

   long  polygonCount;
   int** indexedPolygonList;    //indexedPolygonList[k][i] = index of vertex i of polygon k
   int*  polygonSizeList;       //polygonSizeList[i]= number of vertices of polygon i

};

#else // If this node is not desired

#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/fields/SoSFBool.h>
#include "starlight.h"

class STARLIGHT_API SoVBOMesh : public SoIndexedFaceSet
{
    SO_NODE_HEADER(SoVBOMesh);
public:
    SoVBOMesh();
    ~SoVBOMesh();
    SoSFNode faceset;
    SoSFNode coords;
    SoSFBool invertNormals;
    static void initClass();
protected:

};

#endif

#endif

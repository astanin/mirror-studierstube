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
/** The header file for the SoVBOMesh.
*
* @author Denis Kalkofen
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoVBOMesh.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/starlight/SoVBOMesh.h>

#ifndef NO_VBOMESH

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoCoordinate3.h> 
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoCacheElement.h>

SO_NODE_SOURCE(SoVBOMesh);

void SoVBOMesh::initClass()
{
   SO_NODE_INIT_CLASS(SoVBOMesh, SoIndexedFaceSet, "IndexedFaceSet");
}

SoVBOMesh::SoVBOMesh()
{   
    SO_NODE_CONSTRUCTOR(SoVBOMesh); 
    SO_NODE_ADD_FIELD(faceset,          (NULL));
    SO_NODE_ADD_FIELD(coords,           (NULL));
    SO_NODE_ADD_FIELD(invertNormals,    (FALSE));
    isInit=false;
}


SoVBOMesh::~SoVBOMesh()
{   
    delete[] vertexList;
    delete[] normalList;
}


void SoVBOMesh::init()
{
    if (faceset.getValue()==NULL)
    {
        stb::logPrintE("SoVBOMesh doesn't have a defined faceset\n");
        return;
    }

    vertexProperty.setValue(new SoVertexProperty());
    this->coordIndex=((SoIndexedFaceSet *)faceset.getValue())->coordIndex;
    SoCoordinate3* coord3=(SoCoordinate3*)coords.getValue();

    // Check if the faceset has a vertexProperty
    if(((SoIndexedFaceSet *)faceset.getValue())->vertexProperty.getValue())
        ((SoVertexProperty*)vertexProperty.getValue())->vertex=((SoVertexProperty*)((SoIndexedFaceSet *)faceset.getValue())->vertexProperty.getValue())->vertex;
    // If not, check if 'coords' were given
    else if(coord3)
    {
            coord3->ref();
            ((SoVertexProperty*)vertexProperty.getValue())->vertex=coord3->point;
            coord3->unref();
    }
    // if nothing was found then abort
    else
    {
        stb::logPrintE("SoVBOMesh doesn't have input vertices\n");
        return;
    }

    //////////////////////////////////////////////////////////
    // setup vertexList & colorList
    vertexCount   =((SoVertexProperty*)vertexProperty.getValue())->vertex.getNum();
    vertexList    = new Vertex[vertexCount];						// Allocate Vertex Data
    normalList    = new Normal[vertexCount];
    stb::logPrint("vertexCount %i \n",vertexCount);

    // fill vertList
    for(int i=0;i<vertexCount;i++)
        ((SoVertexProperty*)vertexProperty.getValue())->vertex[i].getValue(vertexList[i].x, vertexList[i].y, vertexList[i].z);

    // Build the VBO
    buildVBO();

    isInit=true;
}

bool isExtensionSupported(const char *extension)
{
    const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;

    where=(GLubyte *) strchr(extension, ' ');
    if (where || *extension == '\0') return false;
    extensions=glGetString(GL_EXTENSIONS);
    start=extensions;
    for (;;) 
    {
        where=(GLubyte *) strstr((const char *) start, extension);
        if (!where) break;
        terminator=where+strlen(extension);
        if (where==start || *(where - 1)==' ')
            if (*terminator==' ' || *terminator=='\0')
                return true;
        start=terminator;
    }
    return false;
}


void SoVBOMesh::buildVBO()
{
    if (!isExtensionSupported("GL_ARB_vertex_buffer_object"))
        stb::logPrintEAbort("GL_ARB_vertex_buffer_object not available.\n");

    // ----------------------------- Generate And Bind The Vertex Buffer
    // Vertices
    glEnableClientState(GL_VERTEX_ARRAY);	
        glGenBuffersARB(1, &vboVertices);			            // Get A Valid Name
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboVertices);	    // Bind The Buffer   
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
                        vertexCount*3*sizeof(float), 
                        vertexList, GL_STATIC_DRAW_ARB);        // Load Vertex Data Into The Graphics Card Memory
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);			    // Bind The Buffer   
    glDisableClientState(GL_VERTEX_ARRAY);	

    // ----------------------------- Set up polygon index lists

    struct IndexedFace{std::vector<int> indices;};
    std::vector<IndexedFace*> faceList;
    IndexedFace* curFace=new IndexedFace;
    for(int i=coordIndex.getNum()-1;i>=0;i--)
    {
        if(coordIndex[i]==-1)
        {//push face            
            faceList.push_back(curFace);
            curFace=new IndexedFace;
        }
        else
            curFace->indices.push_back(coordIndex[i]);
    }
    delete curFace;
    polygonCount=(int)faceList.size();    
    stb::logPrint("SoVBOMesh::polygonCount=%i \n",polygonCount);
    polygonSizeList   =new int[polygonCount];                   //polygonSizeList[i]= number of vertices of polygon i
    indexedPolygonList=new int*[polygonCount];                  //indexedPolygonList[k][i] = index of vertex i of polygon k    
    for(int i=polygonCount-1;i>=0;i--)
    {
        int numOfVert=(int)(faceList[i]->indices.size());       //number of vertices of face i
        polygonSizeList[i]=numOfVert;
        indexedPolygonList[i]=new int[numOfVert];
        int k=0;
        for(int j=(int)(faceList[i]->indices.size())-1;j>=0;j--,k++)
            indexedPolygonList[i][k]=faceList[i]->indices[j];
    }

    // ----------------------------- Compute normals per vertex
    for(int i=0;i<vertexCount;i++)
    {
        normalList[i].nx=1.0;
        normalList[i].ny=0.0;
        normalList[i].nz=0.0;           
    }

    for(int i=(int)faceList.size()-1;i>=0;i--)
    {//per face i
        int numOfVertPerFace=(int)(faceList[i]->indices.size());    //number of vertices of face i
        if(numOfVertPerFace<3)
            stb::logPrintD("Less than 3 vertices found -- setting normal to (1,0,0)\n");
        else
        {
            int vertexIdx0=faceList[i]->indices[0];
            int vertexIdx1=faceList[i]->indices[1];
            int vertexIdx2=faceList[i]->indices[2];

            SbVec3f a=SbVec3f(vertexList[vertexIdx1].x,vertexList[vertexIdx1].y,vertexList[vertexIdx1].z)
                     -SbVec3f(vertexList[vertexIdx0].x,vertexList[vertexIdx0].y,vertexList[vertexIdx0].z);
            SbVec3f b=SbVec3f(vertexList[vertexIdx2].x,vertexList[vertexIdx2].y,vertexList[vertexIdx2].z)
                     -SbVec3f(vertexList[vertexIdx0].x,vertexList[vertexIdx0].y,vertexList[vertexIdx0].z);

            SbVec3f n;
            if (invertNormals.getValue())
                n=a.cross(b);
            else
                n=b.cross(a);

            n.normalize();
            for (int j=0;j<numOfVertPerFace;j++)
            {
                n.getValue( normalList[faceList[i]->indices[j]].nx,
                            normalList[faceList[i]->indices[j]].ny,
                            normalList[faceList[i]->indices[j]].nz
                          );
            }
        }
        
        delete faceList[i];
    }

    ////delete faces 
    faceList.empty();

    glEnableClientState(GL_NORMAL_ARRAY);
        glGenBuffersARB(1, &vboNormals);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormals);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,vertexCount*3*sizeof(float),  normalList,  GL_STATIC_DRAW_ARB);
    glDisableClientState(GL_NORMAL_ARRAY);

}

void SoVBOMesh::GLRender(SoGLRenderAction *action)
{
    SoState* state=action->getState();
    if(!isInit)
    {
        // Initializing GLEW
        GLenum error=glewInit();
        if (error!=GLEW_OK)
            stb::logPrintEAbort("GLEW could not be initialized [ %s ]. \nAborting.\n\n", glewGetErrorString(error));

        // Initializing the Mesh
        init();           
    }

    SoIndexedFaceSet::GLRender(action);

    // ----------------------------- Vertices 
    glEnableClientState( GL_VERTEX_ARRAY );            
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboVertices);    
    glVertexPointer( 3, GL_FLOAT, 0, (char*)NULL);
    
    // ----------------------------- Normals 
    glEnableClientState( GL_NORMAL_ARRAY );						    // Enable Vertex Arrays
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboNormals );
    glNormalPointer( GL_FLOAT, 0, (char*)NULL  );


    glMultiDrawElements(    GL_POLYGON,		                        // GLenum mode
                            (GLsizei*)polygonSizeList,              // const GLsizei *count 
                            GL_UNSIGNED_INT,                        // GLenum type
                            (const GLvoid **)indexedPolygonList,    // const GLvoid* *indices
                            (GLsizei)polygonCount);                 // GLsizei primcount
            
    //Disable VBO
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    glDisableClientState(GL_VERTEX_ARRAY );
    glDisableClientState(GL_NORMAL_ARRAY  );	

}

#else

SO_NODE_SOURCE(SoVBOMesh);

void SoVBOMesh::initClass()
{
    SO_NODE_INIT_CLASS(SoVBOMesh, SoIndexedFaceSet, "IndexedFaceSet");
}

SoVBOMesh::SoVBOMesh()
{   
    SO_NODE_CONSTRUCTOR(SoVBOMesh); 
    SO_NODE_ADD_FIELD(faceset,          (NULL));
    SO_NODE_ADD_FIELD(coords,           (NULL));
    SO_NODE_ADD_FIELD(invertNormals,    (FALSE));
}

SoVBOMesh::~SoVBOMesh()
{   
}

#endif

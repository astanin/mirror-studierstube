/*
 * scene.cpp
 *
 * Create the arena, floor and other bits of the scene using opensg primitives
 * 
 * Chris Christou 28/08/2006
 */
/*
#ifdef WIN32
#include <windows.h>
#endif
*/
#include "opensg_dep.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// the Switch node core
#include <OpenSG/OSGSwitch.h>
//#include <OpenSG/OSGFTGLFont.h>
//#include <OpenSG/OSGFTGLText.h>

#include "lights.h"
//#include "Pong.h"
//#include "Paddle.h"
#include "Puck.h"
#include "scene.h"
//#include "Player.h"
#include "utilities.h"

PerspectiveCameraPtr	scenecamera;
ViewportPtr				sceneviewport;
NodePtr					sceneCamBeacon;
NodePtr					makeBarriers();
NodePtr					makeGoals();
NodePtr					makeBanners();
// the Switch Core
SwitchPtr sw;

//extern NodePtr	arenaNodePtr;
//extern Player	*player;
extern Puck		*puck;

void addSceneCamera(NodePtr root);
NodePtr createScoreBoards();

NodePtr createScene()
{
    //NodePtr scene = makeCoredNode<Group>();

	// create the root Switch node
    NodePtr  scene = Node::create();
    
    sw = Switch::create();
    beginEditCP(sw, Switch::ChoiceFieldMask);
    sw->setChoice(-2);  // Note: this will be Switch::ALL in 1.2
    endEditCP(sw, Switch::ChoiceFieldMask);

    beginEditCP(scene, Node::CoreFieldMask | Node::ChildrenFieldMask);
    scene->setCore(sw);
    
 
	/////////////////////////////ARENA/////////////////////////
	// create the arena
	NodePtr arenaNodePtr = createArena();

	////////////////////////////LIGHTS/////////////////////////
	// Make the lights and add to scene
	// lights is the last node in a string of lights
	NodePtr lights = createArenaLights(scene);  
	

	//NodePtr sb = createScoreBoards();
	
	
	/////////////////////////////PUCK//////////////////////////
	// Create the puck 
	NodePtr _puck_node_ptr = puck->init();
	beginEditCP(lights,Node::ChildrenFieldMask);
		lights->addChild(_puck_node_ptr);
		lights->addChild(arenaNodePtr);
		//		lights->addChild(sb);
	endEditCP(lights,Node::ChildrenFieldMask);
	
	endEditCP  (scene, Node::CoreFieldMask | Node::ChildrenFieldMask);

	return scene;
}

NodePtr createArena( void )
{	
	// Make the Arena Surround - Just a cylinder
	NodePtr surround = makeSurround();
	// make ground - just a plane
	NodePtr ground = makeGround();
	
	NodePtr barriers =makeBarriers();

	NodePtr goals =makeGoals();

	NodePtr banners = makeBanners();

    beginEditCP(surround);
	{
		surround->addChild(ground);
		surround->addChild(barriers);
		surround->addChild(goals);
		surround->addChild(banners);
	}
    endEditCP(surround);

	return surround;
}


NodePtr makeSurround()
{
	MaterialPtr matPtr;
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr trans;
    NodePtr trans_node = makeCoredNode<Transform>(&trans);
    beginEditCP(trans);
		trans->getMatrix().setTranslate(0.0,2.5,0.0);
    endEditCP(trans);

	// MAKE THE SURROUND (CYLINDER) AND ADD IT TO ITS TRANSFORM
	NodePtr surround = OSG::makeCylinder(5.0,12.0, 32, true, false,false);
	beginEditCP(trans_node);
		trans_node->addChild(surround);
	endEditCP(trans_node);

	// SET A TEXTURE OF A CROWD
	ImagePtr image = Image::create();
	Char8 name[64];
	strcpy(name, "./data/crowd.jpg");
	image->read(name);

	ChunkMaterialPtr  texMatPtr     = ChunkMaterial::create();
	TextureChunkPtr   texChunkPtr   = TextureChunk::create();
	//BlendChunkPtr     blendChunkPtr = BlendChunk::create();
	beginEditCP (texChunkPtr);
	{
          texChunkPtr->setImage     ( image);				  
          texChunkPtr->setWrapS     ( GL_REPEAT );
          texChunkPtr->setWrapT     ( GL_REPEAT );
          texChunkPtr->setWrapR     ( GL_REPEAT);

          texChunkPtr->setMinFilter ( GL_LINEAR_MIPMAP_LINEAR );
          texChunkPtr->setMagFilter ( GL_LINEAR );
	}
	endEditCP   (texChunkPtr);

	beginEditCP (texMatPtr);
	{
          texMatPtr->addChunk(texChunkPtr);
         // texMatPtr->addChunk(blendChunkPtr);
	}
	endEditCP   (texMatPtr);
	matPtr = texMatPtr;


	// now create the texture coordinates
	GeoTexCoords2fPtr texcoords = GeoTexCoords2f::create();
	beginEditCP(texcoords);
	Real32 h,A;
	Real32 u,v;
	Real32 txScale=8.0;
	for(h=0.0;h<=1.0;h+=0.2)
	{
		v= txScale*h;
		for(A=0;A<=2*PI;A+=PI/16.0)
		{
			u =txScale*A / (2.0*PI);
			texcoords->addValue(Vec2f(u,v));
		}

		u=txScale;
		v=txScale*h;
		texcoords->addValue(Vec2f(u,v));
	}
	endEditCP(texcoords);


	GeometryPtr geo = GeometryPtr::dcast(surround->getCore());
    beginEditCP(geo);
		geo->setTexCoords(texcoords);
		geo->setMaterial(matPtr);
    beginEditCP(geo);

	// TOP of the arena
	SimpleTexturedMaterialPtr m2 = SimpleTexturedMaterial::create();
    beginEditCP(m2);
    {		
        m2->setAmbient      (Color3f(0.0,0.0,0.0));
        m2->setDiffuse      (Color3f(0.4,0.4,0.7));
    }
    endEditCP  (m2);  
	TransformPtr top_trans;
    NodePtr top_trans_node = makeCoredNode<Transform>(&top_trans);
    beginEditCP(top_trans);
		top_trans->getMatrix().setTransform(Vec3f(0,2.75,0),Quaternion( Vec3f(1,0,0),deg2rad(90)));
    endEditCP(top_trans);
	NodePtr top = makeTorus( .5, 10.5, 16, 32 );
	geo = GeometryPtr::dcast(top->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m2);
    beginEditCP(geo);
	beginEditCP(top_trans_node);
		top_trans_node->addChild(top);
	endEditCP(top_trans_node);
	beginEditCP(trans_node,Node::ChildrenFieldMask);
		trans_node->addChild(top_trans_node);
	endEditCP(trans_node,Node::ChildrenFieldMask);

	return trans_node;
}


NodePtr makeGround()
{
	MaterialPtr matPtr;
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr trans;
    NodePtr trans_node = makeCoredNode<Transform>(&trans);
    beginEditCP(trans);
		trans->getMatrix().setTransform(Vec3f(0,-2.5,0),Quaternion( Vec3f(1,0,0),deg2rad(90)));
    endEditCP(trans);

	// MAKE THE GROUND
	NodePtr bottom = makePlane(25.0, 25.0, 128, 128);


	beginEditCP(trans_node);
		trans_node->addChild(bottom);
	endEditCP(trans_node);


	ImagePtr image = Image::create();
   
	// chequered ground markings - surely easier with a for loop
	UChar8 data[] = {	
			0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
			0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 
			0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
			0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff,0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
			0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 
			0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
			0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff,0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
			0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
			0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff,0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
			0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 
			0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,0xff, 0xff, 0xff,  0x80, 0x00, 0x00, 0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
			0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff,0x80, 0x00, 0x00,  0xff, 0xff, 0xff, 0x80, 0x00, 0x00,  0xff, 0xff, 0xff
	};  

	beginEditCP(image);
		
	image->set( Image::OSG_RGB_PF, 8, 8, 1, 1, 1, 0, data );
	endEditCP(image);
  
    
    SimpleTexturedMaterialPtr m2 = SimpleTexturedMaterial::create();
    beginEditCP(m2);
    {		
        m2->setAmbient      (Color3f(0.1,0.1,0.1));
        m2->setDiffuse      (Color3f(0.2,0.8,0.8));
        m2->setEmission     (Color3f(0.0,0.0,0.0));
        m2->setSpecular     (Color3f(0.0,0.0,0.0));
        m2->setColorMaterial(GL_NONE);
        
        m2->setImage        (image);
        m2->setMinFilter    (GL_LINEAR_MIPMAP_LINEAR);
        m2->setMagFilter    (GL_NEAREST);
       // m2->setEnvMode      (GL_MODULATE);
        m2->setEnvMap       (false);
    }
    endEditCP  (m2);

	GeometryPtr geo = GeometryPtr::dcast(bottom->getCore());  

    beginEditCP(geo);
		geo->setMaterial(m2);
    beginEditCP(geo);

	return trans_node;
}


NodePtr makeBarriers()
{
	GeometryPtr geo;
	// MAKE THE MATERIALS
    SimpleTexturedMaterialPtr m2 = SimpleTexturedMaterial::create();
    beginEditCP(m2);
    {		
        m2->setAmbient      (Color3f(0,0,0));
        m2->setDiffuse      (Color3f(0.1,0.1,0.9));
    }
    endEditCP  (m2);  

	//////////LEFT /////////
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr left_trans;
    NodePtr left_trans_node = makeCoredNode<Transform>(&left_trans);
    beginEditCP(left_trans);
		left_trans->getMatrix().setTransform(Vec3f(-WALLX-0.25,-2.375,0),Quaternion( Vec3f(0,0,0),deg2rad(0)));
    endEditCP(left_trans);
	NodePtr left = makeBox(0.5, 0.25, 11, 1,1,1);
	beginEditCP(left_trans_node);
		left_trans_node->addChild(left);
	endEditCP(left_trans_node);
	geo = GeometryPtr::dcast(left->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m2);
    beginEditCP(geo);

	//////////RIGHT/////////
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr right_trans;
    NodePtr right_trans_node = makeCoredNode<Transform>(&right_trans);
    beginEditCP(right_trans);
		right_trans->getMatrix().setTransform(Vec3f(WALLX+0.25,-2.375,0),Quaternion( Vec3f(0,0,0),deg2rad(0)));
    endEditCP(right_trans);
	NodePtr right = makeBox(0.5, 0.25, 11, 1,1,1);
	beginEditCP(right_trans_node);
		right_trans_node->addChild(right);
	endEditCP(right_trans_node);
	geo = GeometryPtr::dcast(right->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m2);
    beginEditCP(geo);

	NodePtr barriers = makeCoredNode<Group>();
	beginEditCP(barriers,Node::ChildrenFieldMask);
		barriers->addChild(left_trans_node);
		barriers->addChild(right_trans_node);
	endEditCP(barriers,Node::ChildrenFieldMask);


	return barriers;
}



NodePtr makeGoals()
{
	GeometryPtr geo;
	// MAKE THE MATERIALS
	ImagePtr image = Image::create();
	Char8 name[64];
	strcpy(name, "./data/goalp.png");
	image->read(name);

    SimpleTexturedMaterialPtr m2 = SimpleTexturedMaterial::create();
    beginEditCP(m2);
    {		
        m2->setAmbient      (Color3f(0.0,0.0,0.0));
        m2->setDiffuse      (Color3f(1.0,1.0,1.0));
		m2->setImage		(image);
    }
    endEditCP  (m2);  

	////////// 1 /////////
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr goal1_trans;
    NodePtr goal1_trans_node = makeCoredNode<Transform>(&goal1_trans);
    beginEditCP(goal1_trans);
		goal1_trans->getMatrix().setTransform(Vec3f(0,-1.5,9.65),Quaternion( Vec3f(0,1,0),deg2rad(180)));
    endEditCP(goal1_trans);

	// make geometry
	NodePtr goal1 = makePlane(5, 2, 16,16);
	beginEditCP(goal1_trans_node);
		goal1_trans_node->addChild(goal1);
	endEditCP(goal1_trans_node);
	geo = GeometryPtr::dcast(goal1->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m2);
    beginEditCP(geo);

	////////// 2
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr goal2_trans;
    NodePtr goal2_trans_node = makeCoredNode<Transform>(&goal2_trans);
    beginEditCP(goal2_trans);
		goal2_trans->getMatrix().setTransform(Vec3f(0,-1.5,-9.65),Quaternion( Vec3f(0,0,0),deg2rad(0)));
    endEditCP(goal2_trans);

	// make geometry
	NodePtr goal2 = makePlane(5, 2, 16,16);
	beginEditCP(goal2_trans_node);
		goal2_trans_node->addChild(goal2);
	endEditCP(goal2_trans_node);
	geo = GeometryPtr::dcast(goal2->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m2);
    beginEditCP(geo);

	NodePtr goals = makeCoredNode<Group>();
	beginEditCP(goals,Node::ChildrenFieldMask);
		goals->addChild(goal1_trans_node);
		goals->addChild(goal2_trans_node);
	endEditCP(goals,Node::ChildrenFieldMask);


	return goals;
}


/* under constructionae
NodePtr makeSplash()
{
	GeometryPtr geo;
	ImagePtr image = Image::create();
	Char8 name[64];
	strcpy(name, "./data/splash.png");
	image->read(name);

    SimpleTexturedMaterialPtr m2 = SimpleTexturedMaterial::create();
    beginEditCP(m2);
    {		
        m2->setAmbient      (Color3f(0.0,0.0,0.0));
        m2->setDiffuse      (Color3f(0.3,0.7,0.1));
		m2->setImage		(image);
    }
    endEditCP  (m2);  

	////////// 1 /////////
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr splash1_trans;
    NodePtr splash1_trans_node = makeCoredNode<Transform>(&splash1_trans);
    beginEditCP(splash1_trans);
		splash1_trans->getMatrix().setTransform(Vec3f(0,-1.5,9.6),Quaternion( Vec3f(0,1,0),deg2rad(180)));
    endEditCP(splash1_trans);

	// make geometry
	NodePtr splash1 = makePlane(5, 2, 16,16);
	beginEditCP(splash1_trans_node);
		splash1_trans_node->addChild(splash1);
	endEditCP(splash1_trans_node);
	geo = GeometryPtr::dcast(splash1->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m2);
    beginEditCP(geo);


	NodePtr splashs = makeCoredNode<Group>();
	beginEditCP(splashs,Node::ChildrenFieldMask);
		splashs->addChild(splash1_trans_node);
	endEditCP(splashs,Node::ChildrenFieldMask);


	return splash1; 
}
*/


NodePtr makeBanners()
{
	Char8 name[64];
	GeometryPtr geo;

	////////// 1 /////////
	ImagePtr image1 = Image::create();
	strcpy(name, "./data/pirelli.png");
	image1->read(name);

    SimpleTexturedMaterialPtr m1 = SimpleTexturedMaterial::create();
    beginEditCP(m1);
    {		
        m1->setAmbient      (Color3f(0.1,0.1,0.1));
        m1->setDiffuse      (Color3f(1.0,1.0,1.0));
		m1->setImage		(image1);
    }
    endEditCP  (m1); 

	
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr banner1_trans;
    NodePtr banner1_trans_node = makeCoredNode<Transform>(&banner1_trans);
    beginEditCP(banner1_trans);
		banner1_trans->getMatrix().setTransform(Vec3f(-8.3,-2.124,-4.0),Quaternion( Vec3f(0,1,0),deg2rad(90)));
    endEditCP(banner1_trans);

	// make geometry
	NodePtr banner1 = makePlane(2.2, 0.75, 8,1);
	beginEditCP(banner1_trans_node);
		banner1_trans_node->addChild(banner1);
	endEditCP(banner1_trans_node);
	geo = GeometryPtr::dcast(banner1->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m1);
    beginEditCP(geo);


	////////// 2 /////////
	ImagePtr image2 = Image::create();
	strcpy(name, "./data/ucl.png");
	image2->read(name);

    SimpleTexturedMaterialPtr m2 = SimpleTexturedMaterial::create();
    beginEditCP(m2);
    {		
        m2->setAmbient      (Color3f(0.1,0.1,0.1));
        m2->setDiffuse      (Color3f(1.0,1.0,1.0));
		m2->setImage		(image2);
    }
    endEditCP  (m2); 

	
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr banner2_trans;
    NodePtr banner2_trans_node = makeCoredNode<Transform>(&banner2_trans);
    beginEditCP(banner2_trans);
		banner2_trans->getMatrix().setTransform(Vec3f(-8.3,-2.124,4.0),Quaternion( Vec3f(0,1,0),deg2rad(90)));
    endEditCP(banner2_trans);

	// make geometry
	NodePtr banner2 = makePlane(2.2, 0.75, 8,1);
	beginEditCP(banner2_trans_node);
		banner2_trans_node->addChild(banner2);
	endEditCP(banner2_trans_node);
	geo = GeometryPtr::dcast(banner2->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m2);
    beginEditCP(geo);

		////////// 3 /////////
	ImagePtr image3 = Image::create();
	strcpy(name, "./data/marlboro.png");
	image3->read(name);

    SimpleTexturedMaterialPtr m3 = SimpleTexturedMaterial::create();
    beginEditCP(m3);
    {		
        m3->setAmbient      (Color3f(0.1,0.1,0.1));
        m3->setDiffuse      (Color3f(1.0,1.0,1.0));
		m3->setImage		(image3);
    }
    endEditCP  (m3); 
	
	// MAKE ITS TRANSFORM 
	TransformPtr banner3_trans;
    NodePtr banner3_trans_node = makeCoredNode<Transform>(&banner3_trans);
    beginEditCP(banner3_trans);
		banner3_trans->getMatrix().setTransform(Vec3f(8.3,-2.124,4.0),Quaternion( Vec3f(0,1,0),deg2rad(-90)));
    endEditCP(banner3_trans);

	// make geometry
	NodePtr banner3 = makePlane(2.2, 0.75, 8,1);
	beginEditCP(banner3_trans_node);
		banner3_trans_node->addChild(banner3);
	endEditCP(banner3_trans_node);
	geo = GeometryPtr::dcast(banner3->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m3);
    beginEditCP(geo);


	////////// 4 /////////
	ImagePtr image4 = Image::create();
	strcpy(name, "./data/google.png");
	image4->read(name);

    SimpleTexturedMaterialPtr m4 = SimpleTexturedMaterial::create();
    beginEditCP(m4);
    {		
        m4->setAmbient      (Color3f(0.1,0.1,0.1));
        m4->setDiffuse      (Color3f(1.0,1.0,1.0));
		m4->setImage		(image4);
    }
    endEditCP  (m4); 
	
	// MAKE ITS TRANSFORM 
	TransformPtr banner4_trans;
    NodePtr banner4_trans_node = makeCoredNode<Transform>(&banner4_trans);
    beginEditCP(banner4_trans);
		banner4_trans->getMatrix().setTransform(Vec3f(8.3,-2.124,-4.0),Quaternion( Vec3f(0,1,0),deg2rad(-90)));
    endEditCP(banner4_trans);

	// make geometry
	NodePtr banner4 = makePlane(2.2, 0.75, 8,1);
	beginEditCP(banner4_trans_node);
		banner4_trans_node->addChild(banner4);
	endEditCP(banner4_trans_node);
	geo = GeometryPtr::dcast(banner4->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m4);
    beginEditCP(geo);


	////////// 5 /////////
	ImagePtr image5 = Image::create();
	strcpy(name, "./data/carlsberg.png");
	image5->read(name);

    SimpleTexturedMaterialPtr m5 = SimpleTexturedMaterial::create();
    beginEditCP(m5);
    {		
        m5->setAmbient      (Color3f(0.1,0.1,0.1));
        m5->setDiffuse      (Color3f(0.8,0.8,0.9));
		m5->setImage		(image5);
    }
    endEditCP  (m5); 

	
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr banner5_trans;
    NodePtr banner5_trans_node = makeCoredNode<Transform>(&banner5_trans);
    beginEditCP(banner5_trans);
		banner5_trans->getMatrix().setTransform(Vec3f(-9,-2.124,0.0),Quaternion( Vec3f(0,1,0),deg2rad(90)));
    endEditCP(banner5_trans);

	// make geometry
	NodePtr banner5 = makePlane(2.2, 0.75, 8,1);
	beginEditCP(banner5_trans_node);
		banner5_trans_node->addChild(banner5);
	endEditCP(banner5_trans_node);
	geo = GeometryPtr::dcast(banner5->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m5);
    beginEditCP(geo);

		////////// 6 /////////
	ImagePtr image6 = Image::create();
	strcpy(name, "./data/fosters.png");
	image6->read(name);

    SimpleTexturedMaterialPtr m6 = SimpleTexturedMaterial::create();
    beginEditCP(m6);
    {		
        m6->setAmbient      (Color3f(0.1,0.1,0.1));
        m6->setDiffuse      (Color3f(0.8,0.8,0.9));
		m6->setImage		(image6);
    }
    endEditCP  (m6); 

	
	// FIRST MAKE ITS TRANSFORM 
	TransformPtr banner6_trans;
    NodePtr banner6_trans_node = makeCoredNode<Transform>(&banner6_trans);
    beginEditCP(banner6_trans);
		banner6_trans->getMatrix().setTransform(Vec3f(9,-2.124,0.0),Quaternion( Vec3f(0,1,0),deg2rad(-90)));
    endEditCP(banner6_trans);

	// make geometry
	NodePtr banner6 = makePlane(2.2, 0.75, 8,1);
	beginEditCP(banner6_trans_node);
		banner6_trans_node->addChild(banner6);
	endEditCP(banner6_trans_node);
	geo = GeometryPtr::dcast(banner6->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m6);
    beginEditCP(geo);



	NodePtr banners = makeCoredNode<Group>();
	beginEditCP(banners,Node::ChildrenFieldMask);
		banners->addChild(banner1_trans_node);
		banners->addChild(banner2_trans_node);
		banners->addChild(banner3_trans_node);
		banners->addChild(banner4_trans_node);
		banners->addChild(banner5_trans_node);
		banners->addChild(banner6_trans_node);
	endEditCP(banners,Node::ChildrenFieldMask);


	return banners;
}


void addSceneCamera(NodePtr root)
{
	sceneCamBeacon = Node::create();
	beginEditCP(sceneCamBeacon);
	{    
		TransformPtr sceneCamTrans;    
		sceneCamTrans = Transform::create();
		beginEditCP(sceneCamTrans);            
			Matrix M;
			M.setIdentity();
			M.setTransform(Vec3f(0,20,0),Quaternion( Vec3f(-1,0,0),deg2rad(90)));
			sceneCamTrans->setMatrix(M);            
		endEditCP(sceneCamTrans);        
		sceneCamBeacon->setCore(sceneCamTrans);
	}
	endEditCP(sceneCamBeacon);

	scenecamera=PerspectiveCamera::create();

	beginEditCP(scenecamera);
                scenecamera->setBeacon(sceneCamBeacon);
                scenecamera->setFov(deg2rad(70));
                scenecamera->setNear(0.1);
                scenecamera->setFar(100000);
	endEditCP(scenecamera);
        
	GradientBackgroundPtr  bg=GradientBackground::create();
	beginEditCP(bg);
		 bg->addLine(Color3f(0,0,0),0);
		 bg->addLine(Color3f(1,1,1),1);
	endEditCP(bg);
        GradientBackgroundPtr bg1=GradientBackground::create();
	beginEditCP(bg1);
		 bg1->addLine(Color3f(0,0,0),0);
		 bg1->addLine(Color3f(1,1,1),1);
	endEditCP(bg1);

	sceneviewport=Viewport::create();
	beginEditCP(sceneviewport);
		sceneviewport->setBackground(bg);
		sceneviewport->setCamera(scenecamera);
		sceneviewport->setRoot(root);
		sceneviewport->setSize(0,0,1,1);   //(0,0,0.5,1);
	endEditCP(sceneviewport);

		beginEditCP(root);
		root->addChild(sceneCamBeacon);
	endEditCP(root);
}

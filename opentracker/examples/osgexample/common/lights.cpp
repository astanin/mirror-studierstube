/*
 * lights.cpp
 *
 *	CREATE THE LIGHTS AS A BRANCH OF THE MAIN SCENE GRAPH.
 *	LIGHTS ARE LINKED TO EACH OTHER WITH A LOCAL TRANSLATION to
 *  SPACE THEM OUT AND POSITION THEM HIGH ABOVE THE FLOOR
 * 
 * Chris Christou 28/08/2006
 */

// Headers
#include "opensg_dep.h"
#include "scene.h"
#include "lights.h"

LightPtr lightsList[10];              // pointers to the lights (to turn them on or off)
UInt32	 nLights = 0;

NodePtr makeLight(NodePtr scene, float x, float y, float z);

NodePtr makeLightGeometry()
{
		//we add a little sphere that will represent the light source
	GeometryPtr sphere = makeSphereGeo(1,1);

	// make sphere material
	SimpleMaterialPtr sm = SimpleMaterial::create();
	beginEditCP(sm, SimpleMaterial::DiffuseFieldMask |SimpleMaterial::LitFieldMask);
	{
		sm->setLit(false);
		sm->setDiffuse(Color3f(0.9,0.9,0.9));
		sm->setAmbient(Color3f(0.2,0.2,0.2));
	}
	endEditCP (sm, SimpleMaterial::DiffuseFieldMask | SimpleMaterial::LitFieldMask);
	// set sphere material
	beginEditCP(sphere, Geometry::MaterialFieldMask);
	{
		sphere->setMaterial(sm);
	}
	endEditCP (sphere, Geometry::MaterialFieldMask);

	NodePtr sphereNode = Node::create();
	beginEditCP(sphereNode);
		sphereNode->setCore(sphere);
	endEditCP(sphereNode);


	return sphereNode;
}

NodePtr makeLight(NodePtr scene, float x, float y, float z)
{
	TransformPtr light_trans;
    NodePtr light_beacon = makeCoredNode<Transform>(&light_trans);
    beginEditCP(light_trans);
        light_trans->getMatrix().setTranslate(x,y,z);
    endEditCP(light_trans);

    // light.
    PointLightPtr light_core;
    NodePtr light = makeCoredNode<PointLight>(&light_core);
    beginEditCP(light_core);
        light_core->setAmbient(1.0,1.0,1.0,1);
        light_core->setDiffuse(1.0,1.0,1.0,1);
        light_core->setSpecular(0.0,0.0,0.0,1);
        light_core->setBeacon(light_beacon);
        light_core->setOn(true);
    endEditCP(light_core);

    // create spheres geometry
    NodePtr sphere = makeLatLongSphere(50, 50, 0.3);
    TransformPtr sphere_trans_core;
    NodePtr sphere_trans = makeCoredNode<Transform>(&sphere_trans_core);
    beginEditCP(sphere_trans_core);
        sphere_trans_core->getMatrix().setTranslate(x,y,z);
    endEditCP(sphere_trans_core);


    beginEditCP(sphere_trans);
        sphere_trans->addChild(sphere);
    endEditCP(sphere_trans);
    
    beginEditCP(light);
        light->addChild(sphere_trans);
    endEditCP(light);

	beginEditCP(scene);
     //   scene->addChild(light_beacon);
    endEditCP(scene);

	return light;
}


NodePtr createArenaLights(NodePtr scene)
{
	Real32 x,z;
	Real32 theta;
	
	theta = 0.0;
	x = 6.0 * cos(theta);
	z = 6.0 * sin(theta);
	NodePtr light1 = makeLight(scene,x,LIGHTHEIGHT,z);
	theta = deg2rad(45.0);
	x = 6.0 * cos(theta);
	z = 6.0 * sin(theta);
	NodePtr light2 = makeLight(scene,x,LIGHTHEIGHT,z);
	theta = deg2rad(90.0);
	x = 6.0 * cos(theta);
	z = 6.0 * sin(theta);
	NodePtr light3 = makeLight(scene,x,LIGHTHEIGHT,z);
	theta = deg2rad(135.0);
	x = 6.0 * cos(theta);
	z = 6.0 * sin(theta);
	NodePtr light4 = makeLight(scene,x,LIGHTHEIGHT,z);
	theta = deg2rad(180.0);
	x = 6.0 * cos(theta);
	z = 6.0 * sin(theta);
	NodePtr light5 = makeLight(scene,x,LIGHTHEIGHT,z);
	theta = deg2rad(225.0);
	x = 6.0 * cos(theta);
	z = 6.0 * sin(theta);
	NodePtr light6 = makeLight(scene,x,LIGHTHEIGHT,z);
	theta = deg2rad(270.0);
	x = 6.0 * cos(theta);
	z = 6.0 * sin(theta);
	NodePtr light7 = makeLight(scene,x,LIGHTHEIGHT,z);
	theta = deg2rad(315.0);
	x = 6.0 * cos(theta);
	z = 6.0 * sin(theta);
	NodePtr light8 = makeLight(scene,x,LIGHTHEIGHT,z);

	beginEditCP(light1);
		light1->addChild(light2);
    endEditCP(light1);
	beginEditCP(light2);
		light2->addChild(light3);
    endEditCP(light2);
	beginEditCP(light3);
		light3->addChild(light4);
    endEditCP(light3);	
	beginEditCP(light4);
		light4->addChild(light5);
    endEditCP(light4);
	beginEditCP(light5);
		light5->addChild(light6);
    endEditCP(light5);
	beginEditCP(light6);
		light6->addChild(light7);
    endEditCP(light6);
	beginEditCP(light7);
		light7->addChild(light8);
    endEditCP(light7);

    beginEditCP(scene);
        scene->addChild(light1);  // attach first light node to the scene node
    endEditCP(scene); 

	return light8;   // return last node in the light chain
}




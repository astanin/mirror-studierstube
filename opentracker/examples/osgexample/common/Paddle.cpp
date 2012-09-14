/*
 * Paddle.cpp
 * A set of functions to implement the paddle class
 * Every player has a paddle
 * 
 * Chris Christou 30/08/2006
 */

#include "opensg_dep.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif

#include "Player.h"
#include "Paddle.h"
#include "Puck.h"

/* Constructor */
Paddle::Paddle():otCallbackObj()
{

}

Paddle::~Paddle(){

}

	//otCallbackObj interface
void Paddle::_register(){
    registerCallback( &Paddle::callback, (void*)this);	
};

void Paddle::callback( ot::CallbackNode * node,  ot::Event & event, void * data ) {
  Paddle * self = reinterpret_cast<Paddle*>(data);
  try{
    printf("Paddle  %s received event\n", (self->objname).c_str());    
    std::vector<float> & pos = event.getPosition();
    self->positionWC = Vec3f(pos[0],pos[1],pos[2]);
    self->updatePosition();

  } catch(...){
    printf("PADDLE::Error unpacking event\n");
  }
};



void Paddle::sendData(){
  //  printf("paddle sending data\n");
  ot::Event event;
  event.timeStamp();
  float pos[3];
  pos[0]=positionWC[0];
  pos[1]=positionWC[1];
  pos[2]=positionWC[2];
  event.setPosition(pos);

  otCallforwardModule ->callForward(objname.c_str(), event);


}

void Paddle::init(NodePtr root)
{
	r_radius = PADDLE_RADIUS;
	r_halfheight = PADDLE_HALF_HEIGHT;

	origin = Vec3f(0,r_halfheight-PLAYER_HEIGHT,-2.0); // local (player) coord space looking along -z
	positionLC = origin;

	NodePtr paddle_trans_node = makeCoredNode<Transform>(&paddleTransPtr);
    beginEditCP(paddleTransPtr);
	{
		paddleTransPtr->getMatrix().setTranslate(origin);
	}
    endEditCP(paddleTransPtr);

    NodePtr cyl = OSG::makeCylinder(r_halfheight*2.0,r_radius, 32, true, true ,true);
    beginEditCP(paddle_trans_node);
	{
		paddle_trans_node->addChild(cyl);
	}
    endEditCP(paddle_trans_node);

    SimpleMaterialPtr mat = SimpleMaterial::create();
    beginEditCP(mat);
	{
		mat->setAmbient(Color3f(0.0,0.0,0.0));
		mat->setDiffuse(Color3f(0.0,0.0,1.0));
	}
    endEditCP(mat);

    GeometryPtr geo = GeometryPtr::dcast(cyl->getCore());
    beginEditCP(geo);
		geo->setMaterial(mat);
    beginEditCP(geo);

	// Add the PADDLE to the player node (root)
	beginEditCP(root,Node::ChildrenFieldMask);
		root->addChild(paddle_trans_node);
	endEditCP(root,Node::ChildrenFieldMask);

}

void Paddle::translate(Real32 x,Real32 y, Real32 z, Matrix playerMatrix)
{
	Matrix paddleMatrix;
	Quaternion rot,scaleOr;
	Vec3f scale;
	Vec3f newtrans;

	// TRANSFORM device coords TO PLAYER COORD SPACE
	positionLC[0] = origin[0]+x;
	positionLC[1] = origin[1]-y;
	positionLC[2] = origin[2]-z;



	paddleMatrix.identity();


	// ADD TO MATRIX
	// BECAUSE THE PADDLE IS BELOW THE PLAYER NODE IN THE SCENE
	// GRAPH THE PLAYER TRANSFORM WILL BE applied TO THE PADDLE as well
	// SO ALL WE NEED HERE IS A LOCAL COORD SPACE TRANSFORM
	// HOWEVER, WE ALSO NEED A RECORD OF THE ACTUAL WORLD COORD SPACE
	// LOCATION OF THE PADDLE SO THA IT CAN BE BROADCAST BY ELVIN TO THE
	// GAME SERVER FOR PADDLE-PUCK COLLISION DETECTION
    beginEditCP(paddleTransPtr, Transform::MatrixFieldMask);
	{
		paddleMatrix = paddleTransPtr->getMatrix();
		paddleMatrix.setTranslate(positionLC);
		paddleTransPtr->setMatrix(paddleMatrix);
	}
    endEditCP(paddleTransPtr, Transform::MatrixFieldMask);

	// 
	playerMatrix.mult(paddleMatrix);
	playerMatrix.getTransform(positionWC, rot, scale, scaleOr);
	//printf("%f %f %f\n",positionWC[0],positionWC[1],positionWC[2]);


}

// take position of the paddle (this is a local coord system position)
// transform it by the player matrix transform 
// then apply the position test/
// 
bool Paddle::hitTest(Puck *puck, TransformPtr playerTransPtr, bool ps)
{
	Vec3d con;
	Quaternion rot;
	Quaternion scaleOr;
	Vec3f scale;
	Matrix playerMatrix,paddleMatrix;

	// get the matrix for player
    beginEditCP(playerTransPtr, Transform::MatrixFieldMask);
	{
		playerMatrix = playerTransPtr->getMatrix();
	}
    endEditCP(playerTransPtr, Transform::MatrixFieldMask);

	playerMatrix.getTransform(positionWC, rot, scale, scaleOr);

	//printf("pplayer wc %f %f %f\n",positionWC[0],positionWC[1],positionWC[2]);

	// get the matrix for paddle
    beginEditCP(paddleTransPtr, Transform::MatrixFieldMask);
	{
		paddleMatrix = paddleTransPtr->getMatrix();
	}
    endEditCP(paddleTransPtr, Transform::MatrixFieldMask);

	// multiply paddle matrix by player matrix and derive the position
	// of the paddle in world coordinates from the result
	playerMatrix.mult(paddleMatrix);
	playerMatrix.getTransform(positionWC, rot, scale, scaleOr);

	//printf("pplayer wc %f %f %f\n",positionWC[0],positionWC[1],positionWC[2]);

	// derive a vector from the centre of the paddle to the
	// centre pf the puck and calculate the distance
	con[0] = puck->position[0] - positionWC[0];
	con[1] = 0.0;
	con[2] = puck->position[2] - positionWC[2];
	Real32 distance = con.length();
	//printf("%f %f %f %f\n",puck->position[0],puck->position[2] ,positionWC[0],positionWC[2]);

	// The paddle and puck are in contact if the length of con is
	// shorter than the sum of the radii.
	// If a collision has taken place then 
	// we need to normalise con
	// and make this the new direction (velocity) for the puck
	if (distance < puck->radius+r_radius)
	{
		
		Real32 magnitude = puck->velocity.length();
		con.normalize();
		if(ps)
		{
			con = con*magnitude*3;
		}
		else
			con = con*magnitude;

		puck->velocity = con;
		return true;
	}

	return false;
}

void Paddle::updatePosition()
{
    beginEditCP(paddleTransPtr, Transform::MatrixFieldMask);
	{
		paddleTransPtr->getMatrix().setTranslate(positionLC);
	}
    endEditCP(paddleTransPtr, Transform::MatrixFieldMask);
}



/*
 * Player.cpp
 * A set of functions to implement the player class
 * Note: this implementation is for the game server. Players implememnt
 * their own functions
 *
 * 
 * Chris Christou 30/08/2006
 */
#include "Puck.h"
#include "Paddle.h"
#include "Player.h"
#include "utilities.h"
#include "otcb_obj.h"

extern NodePtr *arena;
extern Player *player;
extern int nodeId;
extern int serverId;

NodePtr					beacon,lightBeacon,lightNode;


void Player::callback( ot::CallbackNode * node,  ot::Event & event, void * data ) {
  Player * self = reinterpret_cast<Player*>(data);
  try{
    
    std::vector<float> & pos = event.getPosition();
    //    std::vector<float> & ori = event.getOrientation();
    self->position = Vec3f(pos[0], pos[1], pos[2]);
    self->setPosition(pos[0], pos[1], pos[2]);

  } catch(...){
    printf("Player::Error unpacking event\n");
  }
};

void Player::_register(){
  registerCallback( &Player::callback, (void*)this);	
}



Player::Player():otCallbackObj()
{
//	mouseMoving = false;
//	mouseIncrement = 0.0;
}

Player::~Player(void)
{

}

void Player::init( NodePtr root )
{
	Real32 x,y,z;
	Real32 h,p,r;

	sceneRoot = root;        // add the root ptr to the players sceneroot ptr for later use
	positionIndex = -1;
	score = 0;
	status = INACTIVE;     // becomes active when game server acknowledges presence
						// and assigns it an id and origin etc.
	startButtonPressed = false;
	x = 0;
	y = 100000;			// high above the ground until it is assigned a position by server
	z = 0;
	h = 0.0;
	p = 0.0;
	r = 0.0;

	matrixLocalToWorld.identity();

	// set the player position etc
	setOrientation(h,p,r);
	setPosition(x,y,z);

	NodePtr playerNodePtr = addGeometry(root);
	addCamera(root);
	createPaddle(playerNodePtr);


	beginEditCP(root,Node::ChildrenFieldMask);
	{
		root->addChild(playerNodePtr);
	}
	endEditCP(root,Node::ChildrenFieldMask);
}

void Player::requestToJoin()
{
	

}

void Player::activate(  Real32 x, Real32 y , Real32 z, Real32 h, Real32 p, Real32 r   )
{
	printf("activating player..");

	

	// set the player position etc
	matrixLocalToWorld.identity();
	origin = Vec3f(x,y,z);
	setOrientation(h,p,r);
	setPosition(x,y,z);

	// set the player transform
	 beginEditCP(playerTransPtr);
	{
		playerTransPtr->setMatrix(matrixLocalToWorld);
	}
    endEditCP(playerTransPtr);

	// set the camera transform
	beginEditCP(cameraTrans, Transform::MatrixFieldMask);
	{
			cameraTrans->setMatrix(matrixLocalToWorld);
	} 
	endEditCP(cameraTrans, Transform::MatrixFieldMask);

	//	otSendEntityEvent(nodeId, serverId);

	t_lastKeepAlive = timeGetTime()/1000.0;
	printf("activated and set to %f %f %f \n",origin[0], origin[1],origin[2]);
	status = ACTIVE;
}

void Player::deActivate( void )
{

	Real32 x,y,z;
	Real32 h,p,r;

	positionIndex = -1;
	score = 0;
	status = INACTIVE;     // becomes active when game server acknowledges presence
						// and assigns it an id and origin etc.
	powerShotCountDown=0;
	x = 0;
	y = 100000;			// high above the ground until it is assigned a position by server
	z = 0;
	h = 0.0;
	p = 0.0;
	r = 0.0;

	matrixLocalToWorld.identity();

	// set the player position to be far away
	setOrientation(h,p,r);
	setPosition(x,y,z);
	// adjust paddle appropriately
	paddle->translate(x,y,z,matrixLocalToWorld);
	// set the player transform 
	 beginEditCP(playerTransPtr);
	{
		playerTransPtr->setMatrix(matrixLocalToWorld);
	}
    endEditCP(playerTransPtr);
	// set the camera transform accordingly
	beginEditCP(cameraTrans, Transform::MatrixFieldMask);
	{
			cameraTrans->setMatrix(matrixLocalToWorld);
	} 
	endEditCP(cameraTrans, Transform::MatrixFieldMask);

}

void Player::createPaddle(NodePtr playerNodePtr)
{

	paddle  = new Paddle();
	paddle->init(playerNodePtr);

}

void Player::addCamera(NodePtr root)
{
	// FIRST MAKE THE BEACON
	beacon = Node::create();
	// EDIT THE BEACON, MAKE A CAMERA TRANSFORM, POSITION IT AND ADD TO BEACON
	beginEditCP(beacon);
	{    
		cameraTrans = Transform::create();
		beginEditCP(cameraTrans);
		{
			//Matrix M;
			//M.setIdentity();
			//M.setTransform(Vec3f(0,1.75,0));
			cameraTrans->setMatrix(matrixLocalToWorld);
		} 
		endEditCP(cameraTrans);
        
		beacon->setCore(cameraTrans);
	}
	endEditCP(beacon);

	// NOW MAKE THE CAMERA
	camera=PerspectiveCamera::create();
	// AND SET ITS BEACON TO THE ONE CREATED PREVIOUSLY
	beginEditCP(camera);
                camera->setBeacon(beacon);
                camera->setFov(deg2rad(90));
                camera->setNear(0.1);
                camera->setFar(100000);
	endEditCP(camera);
        
	// MAKE THE BACKGROUND
	GradientBackgroundPtr  bg=GradientBackground::create();
	beginEditCP(bg);
		 bg->addLine(Color3f(0.1,0.14,0.9),0);
		 bg->addLine(Color3f(0,0,0),1);
	endEditCP(bg);

	// CREATE VIEWPORT AND ADD BG AND CAMERA  AND SET THE ROOT AS ROOT
	viewPort=Viewport::create();
	beginEditCP(viewPort);
		viewPort->setBackground(bg);
		viewPort->setCamera(camera);
		viewPort->setRoot(root);
		viewPort->setSize(0,0,1,1);   
	endEditCP(viewPort);

	// ADD BEACON TO THE PLAYER ROOT
	beginEditCP(root);
		root->addChild(beacon);
	endEditCP(root);

}


NodePtr Player::addGeometry(NodePtr root)
{
	// CREATE THE PLAYER TRANS
    NodePtr playerTransNodePtr = makeCoredNode<Transform>(&playerTransPtr);
    beginEditCP(playerTransPtr);
	{
		playerTransPtr->setMatrix(matrixLocalToWorld);
	}
    endEditCP(playerTransPtr);

	/// CREATE GEOMETRY
    NodePtr sphere = makeLatLongSphere(50, 50, 0.1);
    beginEditCP(playerTransNodePtr);
	{
		playerTransNodePtr->addChild(sphere);
	}
    endEditCP(playerTransNodePtr);
	/// CREATE MATERIAL
    SimpleMaterialPtr mat = SimpleMaterial::create();
    beginEditCP(mat);
	{
		mat->setAmbient(Color3f(0.0,0.0,0.0));
		mat->setDiffuse(Color3f(0.0,0.0,1.0));
	}
    endEditCP(mat);

	/// add material to player
    GeometryPtr player_geo = GeometryPtr::dcast(sphere->getCore());
    beginEditCP(player_geo);
		player_geo->setMaterial(mat);
    beginEditCP(player_geo);

	
	beginEditCP(root,Node::ChildrenFieldMask);
		root->addChild(playerTransNodePtr);
	endEditCP(root,Node::ChildrenFieldMask);

	return playerTransNodePtr;
}





//------------------------------------------------: BroadcastData
void Player::localplayer(){
  /* Only for clients
   */
}


void Player::sendData(){
  // this player does not send, because the server does not handle players
}
void Player::broadcastData()
{



}

Vec3f Player::getOrientation()
{
return orientation;
}


void Player::draw()
{


}

void Player::move()
{

}

// --------- position
void Player::setPosition(Real32 x, Real32 y, Real32 z)
{

	position = Vec3f(x,y,z);
	matrixLocalToWorld.setTranslate(x,y,z);
}

void Player::setPosition(Vec3f pos)
{
	position = pos;
	matrixLocalToWorld.setTranslate(position);
}


Vec3f Player::getPosition()
{
return position;
}

/*
void Player::setPosition(Real32 x, Real32 y, Real32 z)
{

	matrixLocalToWorld.setTranslate(x,y,z);
}
*/
void Player::setOrientation(Real32 h, Real32 p, Real32 r)
{

	matrixLocalToWorld.setRotate(Quaternion(Vec3f(0,1,0),deg2rad(h)));

}



void Player::setOrientation(Vec3f orientation)
{
	// only rotation about the y axis is need
	matrixLocalToWorld.setRotate(Quaternion(Vec3f(0,1,0),deg2rad(orientation[0])));
}

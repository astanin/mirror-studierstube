/*
 * Player.cpp
 * A set of functions to implement the player class
 *
 * 
 * Chris Christou 30/08/2006
 */

#include "opensg_dep.h"
#include "Puck.h"
#include "Paddle.h"
#include "Player.h"
#include "utilities.h"
//#include "otinterface.h"

extern NodePtr *arena;
//extern Player *player;
//extern int numPlayers;
//extern int nodeId;
//extern int serverId;

NodePtr 	beacon,lightBeacon,lightNode;
ImageForegroundPtr		goalFrg;

void Player::callback( ot::CallbackNode * node,  ot::Event & event, void * data ) {
  Player * self = reinterpret_cast<Player*>(data);
  try{
    printf("Player:: %s received event\n", ((self->objname).c_str()));
    std::vector<float> & pos = event.getPosition();
    //    std::vector<float> & ori = event.getOrientation();
    //    self->position = Vec3f(pos[0], pos[1], pos[2]);
    self->setPosition(pos[0], pos[1], pos[2]);
    beginEditCP(self->playerTransPtr, Transform::MatrixFieldMask);
    self->playerTransPtr->setMatrix(self->matrixLocalToWorld);
    endEditCP(self->playerTransPtr, Transform::MatrixFieldMask);

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

void Player::init(NodePtr root)
{
	Real32 x,y,z;
	Real32 h,p,r;

	positionIndex = -1;
	score = 0;
	status = INACTIVE;     // becomes active when game server acknowledges presence
	goalTimer = 0;
	hasMovedPaddle = false;
	startButtonPressed=false;   // need to push the spacebar to beginning sending data 

	x = 0;
	y = 1.2;	//sure		// high above the ground until it is assigned a position by server
	z = 0;
	h = 180.0;
	p = 0.0;
	r = 0.0;

	matrixLocalToWorld.identity();

	// set the player position etc
	setOrientation(h,p,r);
	setPosition(x,y,z);

	NodePtr playerNodePtr = addGeometry(root);
	addCamera(root);
	addLogo();
	addStart();
	addWinSplash();
	addGoalSplash();
	addConsolationSplash( );
	createPaddle(playerNodePtr);


	beginEditCP(root,Node::ChildrenFieldMask);
	{
		root->addChild(playerNodePtr);
	}
	endEditCP(root,Node::ChildrenFieldMask);



}

void Player::requestToJoin()
{
  /*
  otSendPlayerRequestToJoin(  );
  */
}



void Player::localplayer(){
  otCallbackObj::deregister();
  paddle->deregister();

  	beginEditCP(viewPort);
	   viewPort->getMFForegrounds()->push_back(logoFrg); 
	   viewPort->getForegrounds().push_back(startForegroundPtr); 
	endEditCP(viewPort);


	// Display the start instruction text with the pong game in the background
	activate(0, 1.2, 0, 0.0, 0 ,0); 

	beginEditCP(viewPort);
	  viewPort->getMFForegrounds()->push_back(logoFrg); 
	endEditCP(viewPort);
	//	      numPlayers=1;
	hasMovedPaddle = true;
	printf("player initialized \n");
  
}

void Player::activate( Real32 x, Real32 y , Real32 z, Real32 h, Real32 p, Real32 r   )
{
	printf("activating player..");
	
	origin = Vec3f(x,y,z);

	// set the player position etc
	matrixLocalToWorld.identity();
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


	//	numPlayers++;
	status = PENDING;   // they now have to press spacebar to be active
	t_lastKeepAlive = timeGetTime()/1000.0;

	printf("activated and set to origin[0]: %f x: %f y: %f z:%f h:%f\n",origin[0], x,y,z,h);
		
}



void Player::deActivate( void )
{
  /*
	printf("deactivating player..");

	Real32 x,y,z;
	Real32 h,p,r;

	positionIndex = -1;
	score = 0;
	status = INACTIVE;     // becomes active when game server acknowledges presence
						// and assigns it an id and origin etc.
	x = 0;
	y = 100000;			// high above the ground until it is assigned a position by server
	z = 0;
	h = 0.0;
	p = 0.0;
	r = 0.0;

	matrixLocalToWorld.identity();

	paddle->translate(x,y,z,matrixLocalToWorld);
	// set the player position etc
	setOrientation(h,p,r);
	setPosition(x,y,z);

		// set the player transform
	 beginEditCP(playerTransPtr, Transform::MatrixFieldMask);
	{
		playerTransPtr->setMatrix(matrixLocalToWorld);
	}
    endEditCP(playerTransPtr, Transform::MatrixFieldMask);
	// set the camera transform
	beginEditCP(cameraTrans, Transform::MatrixFieldMask);
	{
			cameraTrans->setMatrix(matrixLocalToWorld);
	} 
	endEditCP(cameraTrans, Transform::MatrixFieldMask);
  */
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

	// CREATE VIEWPORT AND ADD BG FG AND CAMERA  AND SET THE ROOT AS ROOT
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

void Player::addLogo( void )
{
	//load the logo image file
	ImagePtr logoImage = Image::create();
	beginEditCP(logoImage);
		logoImage->read("./data/logo.png");
	endEditCP(logoImage);

	logoFrg = ImageForeground::create();
	beginEditCP(logoFrg);
		logoFrg->addImage(logoImage,Pnt2f(0.05,0));
	endEditCP(logoFrg);

	beginEditCP(viewPort);
		viewPort->getMFForegrounds()->push_back(logoFrg); 
	endEditCP(viewPort);

}


void Player::addStart( void )
{

	ImagePtr startImage = Image::create();
	beginEditCP(startImage);
		startImage->read("./data/start.png");
	endEditCP(startImage);

	startForegroundPtr = ImageForeground::create();
	beginEditCP(startForegroundPtr);
		startForegroundPtr->addImage(startImage,Pnt2f(0.37,0.5));
	endEditCP(startForegroundPtr);


	// don't add to viewport yet

}

void Player::addWinSplash( void )
{

	ImagePtr image = Image::create();
	beginEditCP(image);
		image->read("./data/win.png");
	endEditCP(image);

	winForegroundPtr = ImageForeground::create();
	beginEditCP(winForegroundPtr);
		winForegroundPtr->addImage(image,Pnt2f(0.37,0.5));
	endEditCP(winForegroundPtr);


	// don't add to viewport yet

}

void Player::addGoalSplash( void )
{
	//load the GOAL image file
	ImagePtr goalImage = Image::create();
	beginEditCP(goalImage);
		goalImage->read("./data/goal.png");
	endEditCP(goalImage);
	goalForegroundPtr = ImageForeground::create();
	beginEditCP(goalForegroundPtr);
		goalForegroundPtr->addImage(goalImage,Pnt2f(0.375,0.55));
	endEditCP(goalForegroundPtr);


	// don't add to viewport - only when goal notification
	// is receivedl
}



void Player::addConsolationSplash( void )
{
	ImagePtr consolationImage;

	
	consolationImage = Image::create();
	beginEditCP(consolationImage);
		consolationImage->read("./data/consolation1.png");
	endEditCP(consolationImage);
	consolationForegroundPtr[0] = ImageForeground::create();
	beginEditCP(consolationForegroundPtr[0]);
		consolationForegroundPtr[0]->addImage(consolationImage,Pnt2f(0.375,0.55));
	endEditCP(consolationForegroundPtr[0]);

	
	consolationImage = Image::create();
	beginEditCP(consolationImage);
		consolationImage->read("./data/consolation2.png");
	endEditCP(consolationImage);
	consolationForegroundPtr[1] = ImageForeground::create();
	beginEditCP(consolationForegroundPtr[1]);
		consolationForegroundPtr[1]->addImage(consolationImage,Pnt2f(0.375,0.55));
	endEditCP(consolationForegroundPtr[1]);

	
	consolationImage = Image::create();
	beginEditCP(consolationImage);
		consolationImage->read("./data/consolation3.png");
	endEditCP(consolationImage);
	consolationForegroundPtr[2] = ImageForeground::create();
	beginEditCP(consolationForegroundPtr[2]);
		consolationForegroundPtr[2]->addImage(consolationImage,Pnt2f(0.375,0.55));
	endEditCP(consolationForegroundPtr[2]);


}



//---work in progress
void Player::addScreenScore()
{

	scoreForegroundPtr = SimpleStatisticsForeground::create();
	//char scorestring[128];
	//sprintf(scorestring,"Player1: %d\nPlayer2: %d",9,2);
    beginEditCP(scoreForegroundPtr);
    scoreForegroundPtr->setSize(40);
    scoreForegroundPtr->setColor(Color4f(0,1,0,0.9));
   // scoreForegroundPtr->addElement(NULL,     scorestring);
    endEditCP(scoreForegroundPtr);

}

//----- work in progress
void Player::addSplash(NodePtr root)
{
	//load the SPLASH image file
	ImagePtr splashImage = Image::create();
	beginEditCP(splashImage);
		splashImage->read("./data/splash.png");
	endEditCP(splashImage);
	//splashForegroundPtr = ImageForeground::create();
	//beginEditCP(splashForegroundPtr);
	//	splashForegroundPtr->addImage(splashImage,Pnt2f(0.0,0.0));
	//endEditCP(splashForegroundPtr);

	ImageBackgroundPtr  bg2=ImageBackground::create();
	beginEditCP(bg2);
		 bg2->setImage(splashImage);
	endEditCP(bg2);

	// CREATE VIEWPORT AND ADD BG FG AND CAMERA  AND SET THE ROOT AS ROOT
	viewPortBG=Viewport::create();
	beginEditCP(viewPortBG);
		viewPortBG->setBackground(bg2);
		//viewPort->getForegrounds().push_back(flashForegroundPtr);
		viewPortBG->setCamera(camera);
		viewPortBG->setRoot(root);
		viewPortBG->setSize(0,0,1,1);   
	endEditCP(viewPortBG);


}


NodePtr Player::addGeometry(NodePtr root)
{
	// MAKE THE MATERIALS
	ImagePtr image = Image::create();
	Char8 name[64];
	strcpy(name, "./data/goalkeeper1.png");
	image->read(name);

    SimpleTexturedMaterialPtr m2 = SimpleTexturedMaterial::create();
    beginEditCP(m2);
    {		
        m2->setAmbient      (Color3f(0.0,0.0,0.0));
        m2->setDiffuse      (Color3f(1.0,1.0,1.0));
		m2->setImage		(image);
    }
    endEditCP  (m2);  

	// CREATE THE PLAYER TRANS
    NodePtr playerTransNodePtr = makeCoredNode<Transform>(&playerTransPtr);
    beginEditCP(playerTransPtr);
	{
		playerTransPtr->setMatrix(matrixLocalToWorld);
	}
    endEditCP(playerTransPtr);

	/// CREATE GEOMETRY
    NodePtr plane = makePlane(2.5,2.5,2,2);
    beginEditCP(playerTransNodePtr);
	{
		playerTransNodePtr->addChild(plane);
	}
    endEditCP(playerTransNodePtr);
	

	/// add material to player
    GeometryPtr player_geo = GeometryPtr::dcast(plane->getCore());
    beginEditCP(player_geo);
		player_geo->setMaterial(m2);
    beginEditCP(player_geo);
	
	beginEditCP(root,Node::ChildrenFieldMask);
		root->addChild(playerTransNodePtr);
	endEditCP(root,Node::ChildrenFieldMask);

	return playerTransNodePtr;
}





//------------------------------------------------: BroadcastData
void Player::broadcastData()
{
	// Timers

	static Real32 t_last=0;
	Real32 t_now;
	Real32 t_elapsed;

	t_now = timeGetTime()/1000.0; 
	t_elapsed = t_now-t_last;

	
	if(hasMovedPaddle)
	{
	  paddle->sendData();

		hasMovedPaddle = false;
	}

	if(t_elapsed>2.0)
	{
	  sendData();
	  t_last = t_now;
	}
	
}

void Player::sendData(){
  //  printf("player is sending data\n");
  ot::Event event;
  event.timeStamp();
  float pos[3];
  pos[0]=position[0];
  pos[1]=position[1];
  pos[2]=position[2];
  event.setPosition(pos);
  // send orientation   
  otCallforwardModule ->callForward(objname.c_str(), event);

}

void Player::draw()
{
 // nit implemented here
}

void Player::move()
{
 //not implemented here 
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

//-----orientation
Vec3f Player::getOrientation()
{
return orientation;
}

void Player::setOrientation(Real32 h, Real32 p, Real32 r)
{
	// only rotation about the y axis is need
	orientation = Vec3f(h,p,r);
	matrixLocalToWorld.setRotate(Quaternion(Vec3f(0,1,0),deg2rad(h)));

}

void Player::setOrientation(Vec3f orientation)
{
	// only rotation about the y axis is need
	matrixLocalToWorld.setRotate(Quaternion(Vec3f(0,1,0),deg2rad(orientation[0])));
}

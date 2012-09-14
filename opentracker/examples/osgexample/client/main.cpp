
/*
 * main.cpp - for player node
 *
 * Note: Elvin stuff is kept modularised  & well separated from the rest of the code for two reasons
 * Firtly, This makes replacing elvin for some other middleware easier and allows for comparisons to
 * be made. Secondly, elvins implementation screws up VS2003 ability to locate errors in compile-time
 * debugging. Errors are reported at the wrong location in the file.
 * 
 * Chris Christou 28/08/2006
 */
#include "opensg_dep.h"
//#include "glut.h"

#include "lights.h"
#include "Pong.h"
#include "Paddle.h"
#include "Puck.h"
#include "scene.h"
#include "Player.h"

//#include "otinterface.h"
#include "ot.h"
#include "otcb_obj.h"
#include "utilities.h"

// local functions
void sleep(osg::UInt32 i); // what i need!
void display( void );
int  setupGLUT( int *argc, char *argv[] );

osg::WindowPtr window;        // One window/viewport used for overview of scene

void pressKey(int key, int x, int y);
void releaseKey(int key, int x, int y);
void showStats(osg::ViewportPtr v,osg::RenderAction *a);
void drawText(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b);

/*

// Timers
Real32 t_last;
Real32 t_last_send;
Real32 t_start;
Real32 t_send_time=0.1f;
*/



// OpenSG declarations
osg::NodePtr	root;	         // Top of the OpenSG graph
osg::RenderAction *renderAct; // For drawing without simplescenemanager
Puck	*puck;       	// Puck is drawn by this node but its position is derived from the server
LightPtr lights[8];      // pointers to the lights (to turn them on or off)

// the Switch Core set in scene.cpp
extern SwitchPtr	sw;

/*
NodePtr	arenaNodePtr;    // Arena and lights




int	nodeId;		//The id for this node. Not the player as there could
// be several entities represented here each with its
// own unique id.
int serverId;           // This is the nodeId of the server just for reference
														

int	numPlayers;

int	registeredplayers[MAXPLAYERS];

*/
void printFontFamilies()
 	{
	/*   	   std::vector<std::string> families;
 	   OSG::TextFaceFactory::the().getFontFamilies(families);  // Get list of all families
 	
 	   std::cout << "--- Font Families ---\n";
 	   for(unsigned i=0; i<families.size(); ++i)
 	   {
 	      std::cout << families[i] << std::endl;
 	   }
 	   std::cout << "---------------------\n";*/
 	}

std::vector<otCallbackObj*> otObjs;
std::vector<Player *>players;     

// Initialize GLUT & OpenSG and set up the scene
int main(int argc, char **argv)
{
  ////////////////////////////OPENTRACKER///////////////////////////
  otInit(argc, argv);
 
  
    //game = new cGame();
  puck = new Puck();
  puck->setName("Puck");
  puck->_register();

  otObjs.push_back(puck);

  randInit();
  /*
  nodeId = (int)rand();     
  printf("Your Player Node id is: %d\n",nodeId);
  playerPositionReceived=true;

  ////////////////////////////////TIMING///////////////////////
  t_start = t_last = t_last_send = timeGetTime()/1000.0;

  */
  ////////////////////////////OSG init/////////////////////////
  osg::osgInit(argc,argv);

  printFontFamilies();

  ///////////////////////////// GLUT init//////////////////////
  int winid = setupGLUT(&argc, argv);
  // the connection between GLUT and OpenSG
  osg::GLUTWindowPtr gwin= osg::GLUTWindow::create();
  renderAct=osg::RenderAction::create();
	
  /////////////////////////////SCENE//////////////////////////
  NodePtr root = createScene();  // creates arena, lights, puck and  initialises player 
  
  ////////////////////////////PLAYER/////////////////////////
  for(int i=0;i<2;i++)  // initialise 2 players
	{
	  Player * p = new Player();
	  p->init( root );
	  players.push_back(p);
	  
//	  registeredplayers[i]=0;
	}
	
  //	numPlayers = 0;    // number of live players 

	////////////////////////////WINDOW/////////////////////////
	//showStats(players[0]->viewPort,renderAct);

  // Initialize local player
  players[0] ->setName("PlayerB");
  players[0] ->paddle->setName("PaddleB");
  players[0] ->localplayer();

  // initialize other players
  players[1] ->setName("PlayerA");
  players[1] ->_register();
  players[1] ->paddle->setName("PaddleA");
  players[1] ->paddle->_register();

  
	gwin->setSize(400,400);
	gwin->setId(winid);
	window=gwin;
	window->addPort(players[0]->viewPort);

	window->init();
	
    // GLUT main loop
    glutMainLoop();

    return 0;
}

// redraw the window
void display( void )
{

  otInterface();  // check for messages
	/*
	UInt32 i;
	static UInt32 numJoinAttempts = 0;
	static UInt32 first=true;
	Real32 t;

	Real32 t_elapsed;
	static Real32 t_last_join_attempt=0;
	static Real32 t_totalelapsed=0.0;
	static Real32 t_startJoinAttempt=0.0;


	char string[128];	
	*/
  

  Player *player = players[0];
  osg::Real32 t_now;
  static osg::Real32 t_lastBackgroundAudio=0;
	t_now = timeGetTime()/1000.0; 
	if((t_now - t_lastBackgroundAudio)>15 && player->goalTimer<0)
	{
		if(t_lastBackgroundAudio>0)
			PlaySound("./data/background2.wav", NULL, SND_ASYNC);
		t_lastBackgroundAudio = t_now;
	}
  
		
	/*// has this player been offered a position slot by the server?
	  // if so, send a confirmation and player node id to the registered players list
	  //	  if(playerPositionReceived)
	  //  {
	      // Player has been acknowledged by server but waiting to be	activated
//	      registeredplayers[0] = nodeId;

	  //    otSendPlayerConfirmation( ); // let the game server know we're here and have accepted position
	*/	  /*  }
	  else
	    {
	      // Otherwise, send a request to join every second until acknowledged by server with an empty
	      //position slot
	      
	      t_elapsed = (t_now -t_last_join_attempt);

	      if(player->positionIndex < 0 && t_elapsed>1000.0)
		{
		  
		  player->requestToJoin();
		  //window->render(renderAct);
		  
		  if(numJoinAttempts>10)
		    {
		      printf("Join game timedout -make sure pongserver is running\n");
		      sleep(10);
		      exit(0);
		    }
		  t_last_join_attempt = t_now; 
		  
		}
	    }
  	}
	else*/
	if(player->status==ACTIVE)
	{
			player->broadcastData();
			if(--player->goalTimer == 0)
			{
				
				beginEditCP(player->viewPort);
				player->viewPort->getForegrounds().clear();
				player->viewPort->getMFForegrounds()->push_back(player->logoFrg); 
				endEditCP(player->viewPort);
				//PlaySound("./data/background2.wav", NULL, SND_ASYNC);
			}
	}
	/*
	//----- UPDATE PLAYERS
	Real32 t_ka, waitTime;
	for(UInt32 i = 0;i<MAXPLAYERS; i++)
	{
		player = players[i];

		// First check the other players are still around
		// by testing their keep-alive status
		// Keep-alives come in the form of 'player-data' tuples sent in a Rep3DEvent message
		if(i!=0 && (player->status == ACTIVE || player->status==PENDING) )  
		{
			t_ka = timeGetTime()/1000.0;
			if(player->status==PENDING)
				waitTime = 60.0;
			else
				waitTime = 30.0;
			//printf("%f %f %f\n",t_ka,player->t_lastKeepAlive,(t_ka - player->t_lastKeepAlive));
			if( (t_ka - player->t_lastKeepAlive) > waitTime)
			{
				player->deActivate();
//				numPlayers--;
//				registeredplayers[i]=0;
				continue;
			}
		}
	}

	 */      
	window->render(renderAct);

}

//
// GLUT callback functions
//
// react to size changes
void reshape(int w, int h)
{
  window->resize(w, h);
  glutPostRedisplay();
}


  
// react to mouse button presses
void mouse(int button, int state, int x, int y)
{
    glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{

	osg::UInt32 w = window->getWidth();
	osg::UInt32 h = window->getHeight();

	osg::Real32 width =osg::Real32(w);
	osg::Real32 height =osg::Real32(h);
	osg::Real32 ydevice =osg::Real32(y);
	osg::Real32 xdevice =osg::Real32(x);
	osg::Real32 scalex = 7.0/width;
	osg::Real32 scaley = 2.0/height;
	osg::Real32 xp,yp,zp;
	osg::Real32 yy;

	// calculate relative adjustments of the paddle 
	// 3D Device coordinate space, Facing along -ve z axis
	xp = scalex*(xdevice-width/2.0);
	yp = 0.0;
	yy = ((height/2.0)-ydevice)+height/2.0;
	zp = yy*scaley;


	// Apply to the paddle
	// This will transform to player coord space
	// and also calculate the world coords of the paddle
	// for broadcasting to game server
	if(players[0]->status == ACTIVE)
	{
		players[0]->hasMovedPaddle = true;
		players[0]->paddle->translate(xp,yp,zp,players[0]->matrixLocalToWorld);
		
	}

	// post redisplay request
    glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27:    
        {
//			players[0]->deActivate();
			
			otExit();
            osgExit();
            exit(1);
        }
        break;

		case ' ':
			if(!players[0]->startButtonPressed)
			{
//				otSendPlayerRequestToStart(  );
				players[0]->startButtonPressed = true;
				players[0]->status = ACTIVE;
				beginEditCP(players[0]->viewPort);
					players[0]->viewPort->getForegrounds().clear();
					players[0]->viewPort->getMFForegrounds()->push_back(players[0]->logoFrg); 
				endEditCP(players[0]->viewPort);
				PlaySound("./data/background1.wav", NULL, SND_ASYNC);
			}
			else{
			  //otSendPowerShotRequest();
			}

			break;

    case 'a':
        {
            beginEditCP(sw, Switch::ChoiceFieldMask);
            sw->setChoice(Switch::ALL); 
            endEditCP(sw, Switch::ChoiceFieldMask);
        }
        break;

        case 'n':
        {
            beginEditCP(sw, Switch::ChoiceFieldMask);
            sw->setChoice(Switch::NONE); 
            endEditCP(sw, Switch::ChoiceFieldMask);
        }
        break;

    case 's':
      //      showStats(players[0]->viewPort,renderAct);
      break;

	//	case 'p':
	//		startGame= true;
	//	break;
    }
}

//------to do - add player movements here
void pressKey(int key, int x, int y) 
{
	
	Player *player;
	player = players[0];
	Vec3f pos = player->getPosition();
	Vec3f ort = player->getOrientation();

	switch (key) {
		case GLUT_KEY_LEFT :  // left
			pos[0] -= 0.1;
			if(pos[0]< -6.0) return;
			player->setPosition(pos);
		break;
		case GLUT_KEY_RIGHT : // right
			pos[0] += 0.1;
			if(pos[0]> 6.0) return;
			player->setPosition(pos);
		break;
		case GLUT_KEY_UP :   // forwards
			pos[2] -= 0.1;
			if(pos[2]< -9.0) return;
			player->setPosition(pos);
					//
			break;
		case GLUT_KEY_DOWN : // backwards
			pos[2] += 0.1;
			if(pos[2]> 9.0) return;

			player->setPosition(pos);
			break;
		case GLUT_KEY_PAGE_UP : 
			
			break;
		case GLUT_KEY_PAGE_DOWN : 
			
			break;	
	}

	beginEditCP(player->cameraTrans, Transform::MatrixFieldMask);
		player->cameraTrans->setMatrix(player->matrixLocalToWorld);
	endEditCP(player->cameraTrans, Transform::MatrixFieldMask);
	
	beginEditCP(player->playerTransPtr, Transform::MatrixFieldMask);
		player->playerTransPtr->setMatrix(player->matrixLocalToWorld);
	endEditCP(player->playerTransPtr, Transform::MatrixFieldMask);

	// broadcast this change in player position
	player->sendData();
	player->hasMovedPaddle = true;
	
}

void releaseKey(int key, int x, int y)
{

	Player *player;
	player = players[0];
	Vec3f pos = player->getPosition();
	Vec3f ort = player->getOrientation();

	switch (key) {
		case GLUT_KEY_LEFT : 
		  //			player->setPosition(player->origin);
			break;
		case GLUT_KEY_RIGHT : 
		  //			player->setPosition(player->origin);
			break;
		case GLUT_KEY_UP : 
		case GLUT_KEY_DOWN : 
			
			break;

		case GLUT_KEY_PAGE_UP : 
		case GLUT_KEY_PAGE_DOWN:
			
			break;
	}

	beginEditCP(player->cameraTrans, Transform::MatrixFieldMask);
		player->cameraTrans->setMatrix(player->matrixLocalToWorld);
	endEditCP(player->cameraTrans, Transform::MatrixFieldMask);
	
	beginEditCP(player->playerTransPtr, Transform::MatrixFieldMask);
		player->playerTransPtr->setMatrix(player->matrixLocalToWorld);
	endEditCP(player->playerTransPtr, Transform::MatrixFieldMask);
	
	// broadcast this change in player position
//	otSendPlayerData( player->score, pos[0],pos[1],pos[2],   ort[0],ort[1],ort[2] );*/
} 

void
drawText(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b)
{
    unsigned int lines;
    char* p;

    glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();
     glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
	    0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
     glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glColor3f(r,g,b);
      glRasterPos2i(x, y);
      for(p = s, lines = 0; *p; p++) {
	  if (*p == '\n') {
	      lines++;
	      glRasterPos2i(x, y-(lines*18));
	  }
	  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
      }
      glPopMatrix();
     glMatrixMode(GL_PROJECTION);
     glPopMatrix();
     glMatrixMode(GL_MODELVIEW);
}



// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
	int winid = glutCreateWindow("Pong Player");
    
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc( motion );
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
   glutKeyboardFunc(keyboard);
    glutIdleFunc(display);
	//glutFullScreen();

    return winid;
}

void sleep(osg::UInt32 i)
{
	osg::Real32 t_now;
	osg::Real32 t_last;
	osg::Real32 t;

	t_last = timeGetTime()/1000.0;
	do {t_now = timeGetTime()/1000.0; 	t = (t_now-t_last);}
	while(t<i);
}
	


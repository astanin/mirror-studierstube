/*
 * main.cpp
 * Initialise the world incl OpenTracker
 * The display callback is located here
 * 
 * Chris Christou 27/08/2006
 * Updated 28-09-06: Adjusted win procedure
 */

// Headers
#include "opensg_dep.h"

#ifdef WIN32
#include <windows.h>										// Header File For Windows
#endif

//#include "cGame.h"
#include "utilities.h"
#include "lights.h"
#include "Pong.h"
#include "Paddle.h"
#include "Puck.h"
#include "scene.h"
#include "Player.h"
#include "ot.h"
#include "otcb_obj.h"


#ifdef WIN32
#include <sys/timeb.h>
#else
#include <time.h>
#endif

#define GAMESERVER



// opensg related externs
extern PerspectiveCameraPtr		scenecamera;
extern ViewportPtr				sceneviewport;
extern PerspectiveCameraPtr		playercamera;
extern ViewportPtr				playerviewport;
// the Switch Core set in scene.cpp
extern SwitchPtr				sw;

// OpenSG global variables
NodePtr 	root;					// Top of the OpenSG graph
NodePtr		arenaNodePtr;           // Arena and lights
//WindowPtr						window;					// One window/viewport used for overview of scene
RenderAction					*renderAct;				// For drawing without simplescenemanager
Puck							*puck;             
std::vector<Player *>players;
//cGame							*game;
SimpleSceneManager *mgr;
extern LightPtr					lightsList[];              // pointers to the lights (to turn them on or off)
extern UInt32					nLights;

// local stuff 
int								numPlayers;
// Timers
Real32							t_last;
Real32							t_last_send;
Real32							t_start;
Real32							t_send_time=0.1f;

// local functions
void							display( void );
int								setupGLUT( int *argc, char *argv[] );
void							sleep(int);
NodePtr							createArena( void );


// The following are hard-coded positions for 4 players in the game
// starting at 6o'clock (looking along -z axis) then 12o'clock then 3 o'clock then 9 o'clock 
double playerposition[4][4] = {{0,PLAYER_HEIGHT,9.6,0},{0,PLAYER_HEIGHT,-9.6,180.0},{9.6,PLAYER_HEIGHT,0,90.0},{-9.6,PLAYER_HEIGHT,0,-90.0}};


// Initialize GLUT & OpenSG and set up the scene
int main(int argc, char **argv)
{
  randInit();
  
  puck = new Puck();
  puck->setName("Puck");
  // no need to register the puck, because this is the server

  //game = new cGame();


  osgInit(argc,argv);

  ///////////////////////////// GLUT init//////////////////////
  int winid = setupGLUT(&argc, argv);
  GLUTWindowPtr gwin= GLUTWindow::create();
  gwin->setId(winid);
  gwin->init();
  
  /////////////////////////////SCENE//////////////////////////
  NodePtr root = createScene();  // creates arena, lights, puck and  initialises player 
  //NodePtr root =createScenegraph();
  ////////////////////////////PLAYER/////////////////////////
  // We assume that a total of 2 players might play. Since these must be represented
  // in the scene graph they are initialised now until i learn how to access the
  // scene graph dynamically
  for(int i=0;i<MAXPLAYERS;i++)  // initialise players
	{
	  Player * p = new Player();
	  
	  p->init( root );
	  p->activate(0,0,0,0,0,0);
	  p->status= ACTIVE;
	  players.push_back(p);


	}
  ///register all players with opentracker (This should be part of the UbiTrack onAddObject)
  players[0]->setName("PlayerA");
  players[0]->paddle->setName("PaddleA");
  players[0]->paddle->_register();

  players[1]->setName("PlayerB");
  players[1]->paddle->setName("PaddleB");
  players[1]->paddle->_register();
  

  //numPlayers = 0;    // number of live players 
  
  //////////////////////SCENE MANAGER - SIMPLE FOR THE SERVER///////////////////////////
  //The game server uses the simple scene manager to utilitise simple
  // navigation of the scene etc.
  mgr = new SimpleSceneManager;
  mgr->setWindow(gwin );
  mgr->setRoot  (root);
  //mgr->setHeadlight(true);
    mgr->turnHeadlightOff();
    // show the whole scene
    //  mgr->showAll();
    
    // enable local lights.
    RenderAction *ract = (RenderAction *) mgr->getAction();
    ract->setLocalLights(true);    
    Navigator * nav = mgr->getNavigator();
    nav->setUp(Vec3f(0,0,-1));
    nav->setFrom(nav->getFrom()+Vec3f(0,20,0));
    
    ////////////////////////////OPENTRACKER//////////////////////////
    otInit(argc, argv);
    printf("Ready to play\n");
    
    ////////////////////////////////TIMING///////////////////////
    t_start = t_last = t_last_send = timeGetTime()/1000.0;
    ////////////////////////////OSG init/////////////////////////
    
    
    //////////////////////// GLUT main loop /////////////////////
    puck->newGame();
    glutMainLoop();
    
    return 0;
}


// This is the glut mainloop and also serves as our OpenTracker main
// loop. The gamer server controls the game here: Player Control and Puck Control
void display( void )
{
	Player *player;
	Real32 t;
    Real32 t_now;
	static int winCountDown=0;

	t_now =timeGetTime(); 	t = (t_now-t_last)/1000.0;
	t_last = t_now;


	// do OpenTracker Loop
	otInterface();


	if(--winCountDown>0)
	{
		mgr->redraw();
		return;
	}


	/*----- PUCK CONTROL LOGIC
	if(numPlayers>0)   /// don't update puck until there are players
	{*/
		puck->update();
		puck->broadcastData();
		//	}


	//----- PLAYER CONTROL LOGIC
	Real32 waitTime;
	for(int i = 0;i<(int)players.size(); i++)
	{
		player = players[i];

		if(player->status==ACTIVE)
		  {
		    if(player->score == 10)
		      {
			/*			otSendPlayerWin(i);
			puck->newGame();
			winCountDown=500;*/
		      }
		    // for each player update their paddles in the servers representation of the scene
		    // Also, this shows up in the server (birds-eye) view of the scene from the scene camera
		    // The server has no control over the individual paddles of course
		    player->paddle->updatePosition();
		    // Check to see if this paddle at its current position makes contact with the puck
		    // If so, this will be used to change the velocity of the puck
		    if(player->paddle->hitTest(puck,player->playerTransPtr, player->powerShot))
		      {
			player->powerShot=false;
			//			otSendPlayerPaddleHit(i);
		      }
		    if(--player->powerShotCountDown<=0)
		      player->powerShot=false;
		    


		  }

	}
	
	
	mgr->redraw();
}



//------------------------------------ GLUT callback functions
// react to size changes
void reshape(int w, int h)
{
		mgr->resize(w, h);
		glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27:    
        {
			otExit();
            osgExit();
            exit(1);
        }
        break;
        
        case '0':   case '1':   case '2':   case '3':   case '4':   
        case '5':   case '6':   case '7':   case '8':   case '9': 
        {   
            beginEditCP(sw, Switch::ChoiceFieldMask);
            sw->setChoice(k - '0');
            endEditCP(sw, Switch::ChoiceFieldMask);
            break;
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

        case 'l':   // activate all lights
        {
            for(UInt16 i = 0; i < nLights; ++i)
            {
                lightsList[i]->setOn(true);
            }
        }
        break;
	}
}


void mouse(int button, int state, int x, int y)
{
    if (state)
        mgr->mouseButtonRelease(button, x, y);
    else
        mgr->mouseButtonPress(button, x, y);
        
    glutPostRedisplay();
}

void motion(int x, int y)
{
    mgr->mouseMove(x, y);
    glutPostRedisplay();
}

int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("Pong Game Server");
    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);
    //glEnable(GL_LIGHT1);
    return winid;
}


void sleep(int i)
{
	Real32 t_now;
	Real32 t_last;
	Real32 t;

	t_last = ((float)glutGet(GLUT_ELAPSED_TIME))/1000.0;
	do {t_now = ((float)glutGet(GLUT_ELAPSED_TIME))/1000.0; 	t = (t_now-t_last);}
	while(t<i);
}
	

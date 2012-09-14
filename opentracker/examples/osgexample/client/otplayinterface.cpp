/*
 * elvinmsg.cpp
 * A set of functions to dispatch and receive PDMR events. 
 * This is basically an interface between the rest of the program 
 * and the elvin procedures - keeping them separate 
 * 
 * Chris Christou 27/08/2006
 */

#include <stdio.h>
#include <stdlib.h>

#include "text.h"
#include "Puck.h"
#include "Player.h"
//#include "pdmr.h"
#include "utilities.h"

#include "otinterface.h"
#include "ot.h"
#include "otcb_obj.h"


//extern void handle_error(elvin_error_t error);
extern void       sleep(UInt32 i);
extern void       *messageQueueHead;  // top of the message stack

extern Puck *     puck;
extern Player *	  players[];
extern TextStuff  *ScoreBoard1;
extern TextStuff  *ScoreBoard2;
extern int	  nodeId;
extern int	  serverId;
extern int	  numPlayers; // from main.c
extern int	  registeredplayers[];

static bool	  paddleDataSubscribedTo;
extern bool	  playerPositionReceived;

extern double	  assignedPlayerPx ;
extern double	  assignedPlayerPy ;
extern double	  assignedPlayerPz ;
extern double	  assignedPlayerOx ;
extern double	  assignedPlayerOy ;
extern double	  assignedPlayerOz ;

int
EntityEvent(int id,int sndr,int dest,char *name,char *role)
{
  ot::Event event;
  event.timeStamp();

  // PACK THE MESSAGE //
  event.setAttribute("event", std::string("EntityEvent"));
  event.setAttribute("id", id);
  event.setAttribute("sndr", sndr);
  event.setAttribute("dest", dest);
  event.setAttribute("name", std::string(name));
  event.setAttribute("role", std::string(role));
  

  // SEND THE REQUEST //
  otCallforwardModule ->callForward("OutEntityEvent", event);
  
  return 1;
}

int
ProcessEvent(int id,int sndr,int dest,char *name,char *host,char *role,char *conf)
{

  ot::Event event;
  event.timeStamp();
  // PACK THE MESSAGE //
  event.setAttribute("event", std::string("ProcessEvent"));
  event.setAttribute("id", id);
  event.setAttribute("sndr", sndr);
  event.setAttribute("dest", dest);
  event.setAttribute("name", std::string(name));
  event.setAttribute("host", std::string(host));
  event.setAttribute("role", std::string(role));
  event.setAttribute("conf", std::string(conf));

  // SEND THE REQUEST //
  otCallforwardModule ->callForward("OutEntityEvent", event);



  return 1;
}

int
EntityQueryEvent(int sndr,int dest,int objectId,char *classType,char *role)
{

  ot::Event event;
  event.timeStamp();


  // PACK THE MESSAGE //
  event.setAttribute("event", std::string("EntityQueryEvent"));
  event.setAttribute("sndr", sndr);
  event.setAttribute("dest", dest);
  event.setAttribute("objectId", objectId);
  event.setAttribute("classType", std::string(classType));
  event.setAttribute("roles", std::string(role));



  // SEND THE REQUEST //
  otCallforwardModule ->callForward("OutEntityEvent", event);

  return 1;
}

int
UserEvent(int id,int sndr,int dest,char *name,int space,char *role,char *conf)
{  
  ot::Event event;
  event.timeStamp();

  // PACK THE MESSAGE //
  event.setAttribute("event", std::string("UserEvent"));
  event.setAttribute("id", id);
  event.setAttribute("sndr", sndr);
  event.setAttribute("dest", dest);
  event.setAttribute("name", std::string(name));
  event.setAttribute("spac", space);
  event.setAttribute("role", std::string(role));
  event.setAttribute("conf", std::string(conf));


  // SEND THE REQUEST //
  otCallforwardModule ->callForward("OutEntityEvent", event);
	
  return 1;
}

int
SpaceEvent(int id,int sndr,int dest,char *name,int parent,char *conf)
{
  ot::Event event;
  event.timeStamp();

  // PACK THE MESSAGE //
  event.setAttribute("event", std::string("SpaceEvent") );
  event.setAttribute("id", id);
  event.setAttribute("sndr", sndr);
  event.setAttribute("dest", dest);
  event.setAttribute("name", std::string(name));
  event.setAttribute("prnt", parent);
  event.setAttribute("conf", std::string(conf));

  // SEND THE REQUEST //
  otCallforwardModule ->callForward("OutEntityEvent", event);

  return 1;
}

int
ObjectEvent(int id,int sndr,int dest,char *name,int space,char *conf)
{
  ot::Event event;
  event.timeStamp();

  // PACK THE MESSAGE //
  event.setAttribute("event", std::string("ObjectEvent"));
  event.setAttribute("id", id);
  event.setAttribute("sndr", sndr);
  event.setAttribute("dest", dest);
  event.setAttribute("name", std::string(name));
  event.setAttribute("spac", space);
  event.setAttribute("conf", std::string(conf));


  // SEND THE REQUEST //
  otCallforwardModule ->callForward("OutEntityEvent", event);


  return 1;
}

int
Rep3DFileObjectEvent(int id,int sndr,int dest,int repOf,int parent, char *form, double x, double y, double z,
			 double h, double p, double r,char *fileObject)
{
  ot::Event event;
  event.timeStamp();


  // PACK THE MESSAGE //
  /*  event.setAttribute("event", std::string("Rep3DFileObjectEvent" ));
  event.setAttribute("id", id);
  event.setAttribute("sndr", sndr);
  event.setAttribute("dest", dest);
  event.setAttribute("repOf", repOf);
  event.setAttribute("prnt", parent);
  event.setAttribute("form", std::string(form));
  */
  float pos[3];
	pos[0]=x;
	pos[1]=y;
	pos[2]=z;
  event.setPosition(pos);
  /*
  event.setAttribute("h", h);
  event.setAttribute("p", p);
  event.setAttribute("r", r);
  event.setAttribute("fileObject", std::string(fileObject));
  */
  // SEND THE REQUEST //
  otCallforwardModule ->callForward("OutRep3DFileObjectEvent", event);

  return 1;
}

 

//----------------------------------------- Player Functions



// We have a message - find out who sent it 
// if they are not in the current registeredplayers list 
// add them and increment number of players by 1
int lookupPlayer(int playerNodeId )
{
int i;
int found = -1;

// CHECK IF CURRENT MESSAGE HOST IS IN THE HOSTS LIST
	for(i=0 ; i<MAXPLAYERS ; i++)
		{
			if(registeredplayers[i] == playerNodeId)
			{
				found = i;
			}
		}

	return found;
}

// Look at registeredplayers and return first available free slot
// This is called when a new player requests to play
int getNextFreePositionIndex()
{
	int i;

	for(i=0;i<MAXPLAYERS;i++)
	{
		if(registeredplayers[i] == 0)
			return i;
	}

	return -1;

}

//--------------------------------------Send and Receive

//-------------------------------------- send 
void otSendPuckData( Puck *puck )
{

// not implemented for players
}

void otSendEntityEvent(int sndr, int dest)
{
	int id = (int)rand();
	EntityEvent(id, sndr,dest,"player","player");

}

void otSendExitGame()
{
		int id = (int)rand();
	int sndr	= nodeId;
	int dest	= NULL;
	int repOf	= NULL;
	int parent	= NULL;

	EntityEvent( id, sndr, dest,"player","exit");

}

// send a notification to player 'playerIndex' telling them they have scored
// Need to determine players nodeId and send message to this node only
void otSendScoreFor(int playerIndex)
{

}

void otSendScoreAgainst(int playerIndex)
{

}
void otSendPaddleData( double x, double y, double z)
{

	int id = (int)rand();
	int sndr	= nodeId;
	int dest	= NULL;
	int repOf	= NULL;
	int parent	= NULL;

	Rep3DFileObjectEvent(id,sndr, dest, repOf, parent, "paddle", x, y, z,0,0,0, "position");

}

void otSendPlayerData( int score, double x, double y, double z,double h, double p, double r)
{
	int id = (int)rand();
	int sndr	= nodeId;
	int dest	= serverId;
	int repOf	= score;
	int parent	= NULL;

	Rep3DFileObjectEvent(id, sndr, dest, repOf, parent, "player", x, y, z, h, p, r, "data");

}


void otSendPlayerRequestToJoin(  )
{
	int id = (int)rand();
	int sndr	= nodeId;
	int dest	= NULL;

	printf("sending request to join\n");
	EntityEvent(id , sndr, dest,"player","requesttojoin");

}

void otSendPlayerRequestToStart(  )
{
	int id = (int)rand();
	int sndr	= nodeId;
	int dest	= serverId;

#ifdef DEBUG
	printf("sending start request\n");
#endif

	EntityEvent(id , sndr, dest,"player","requesttostart");

}

void otSendPowerShotRequest(  )
{
	int id = (int)rand();
	int sndr	= nodeId;
	int dest	= serverId;

#ifdef DEBUG
	printf("sending power shot request\n");
#endif

	EntityEvent(id , sndr, dest,"player","requestpowershot");

}
void otSendPlayerConfirmation(  )
{
	int id = (int)rand();
	int sndr	= nodeId;
	int dest	= NULL;

	EntityEvent(id , sndr, serverId,"player","confirm");

}

void otSendPlayerPaddleHit( int playerIndex )
{


}


void otSendNewGame(  )
{

// not used by players - used by server

}

//---------------------------------------- receive

void otPuckPositionReceived(double x, double y, double z )
{

	puck->position = Vec3f(x,y,z);
	puck->updatePosition();  // this simply updates the translation matrix
	

}
void otPuckVelocityReceived(double x, double y, double z )
{
	puck->velocity = Vec3f(x,y,z);

}

void otPaddlePositionReceived(int id, double x, double y, double z )
{
	// id is the node id
	Player *currentplayer = players[id];
	currentplayer->paddle->positionLC = Vec3f(x,y,z);

}



//------------------------------------: Interface

// If there is a message waiting this produre will decide what it is 
// and take the appropriate action
// This is the heart of the communication between the opensg game side
// and the ot communications side


/*
void processEntityEvent( cEntityEvent * EntityEventC ){


  //what does the message say?...
  if(!strcmp(EntityEventC->role, "goalfor") && EntityEventC->dest==nodeId)
    {
      players[0]->score++;   //this player scored
      PlaySound("./data/goal.wav", NULL, SND_ASYNC);
      beginEditCP(players[0]->viewPort);
      players[0]->viewPort->getForegrounds().push_back(players[0]->goalForegroundPtr);
      players[0]->goalTimer=50;
      endEditCP  (players[0]->viewPort);
      ScoreBoard1->updateScore(players[0]->score,players[1]->score);
      ScoreBoard2->updateScore(players[0]->score,players[1]->score);
      printf("Player1 Score: %d Player2 Score: %d\r",players[0]->score,players[1]->score);
    }
  else
    if(!strcmp(EntityEventC->role, "goalagainst") && EntityEventC->dest==nodeId)
      {	
	int ii = RandInt(3); 
	PlaySound("./data/ooh.wav", NULL, SND_ASYNC);
	beginEditCP(players[0]->viewPort);
	players[0]->viewPort->getForegrounds().push_back(players[0]->consolationForegroundPtr[ii]);
	players[0]->goalTimer=50;
	endEditCP  (players[0]->viewPort);
	players[1]->score++;
	ScoreBoard1->updateScore(players[0]->score,players[1]->score);
	ScoreBoard2->updateScore(players[0]->score,players[1]->score);
	
      }
    else
      if(!strcmp(EntityEventC->role, "denied") && EntityEventC->dest==nodeId)
	{
	  printf("Requested to play denied - maybe too many players\n");
	  // exit ??
	  
	}
      else
	if(!strcmp(EntityEventC->role, "paddlehit") && EntityEventC->dest==nodeId)
	  {	
	    if(EntityEventC->id==1)
	      PlaySound("./data/hit1.wav", NULL, SND_ASYNC);
	    else
	      PlaySound("./data/hit2.wav", NULL, SND_ASYNC);
	    
	  }
	else
	  if(!strcmp(EntityEventC->role, "newgame") && EntityEventC->dest==nodeId)
	    {	
	      printf("new game\n");
	      players[0]->score = 0;
	      if(players[1]->status==ACTIVE) players[1]->score=0;
	      PlaySound("./data/background1.wav", NULL, SND_ASYNC);
	      
	      
	    }
	  else
	    if(!strcmp(EntityEventC->role, "wingame") && EntityEventC->dest==nodeId)
	      {	
		
		PlaySound("./data/goal.wav", NULL, SND_ASYNC);
		beginEditCP(players[0]->viewPort);
		players[0]->viewPort->getForegrounds().clear();
		players[0]->viewPort->getMFForegrounds()->push_back(players[0]->logoFrg); 
		players[0]->viewPort->getForegrounds().push_back(players[0]->winForegroundPtr);
		
		endEditCP  (players[0]->viewPort);
		sleep(1);
		players[0]->score=0;players[1]->score=0;
		ScoreBoard1->updateScore(players[0]->score,players[1]->score);
		ScoreBoard2->updateScore(players[0]->score,players[1]->score);				
	      }
  
  
};

void EntityEventReceived_cb( ot::CallbackNode * node,  ot::Event & event, void * data ){
	  int test;
	  int id;
	  int sndr;
	  int dest;
	  const char *name;
	  const char *role;
	  
//	  int assignedPlayerPositionIndex;
	//  Real32 x,y,z,h,p,r;
	  
	  cEntityEvent * EntityEventC= NULL;

	  std::string eventType;
	  try {
	    eventType = event.getAttributeValueString("event");
	  } catch ( ... ){
	    return;
	  }
	  
	  if (eventType.compare("EntityEvent")){
	    
	    //unpack data from opentracker
	    try {
	      id    = event.getAttribute( (int*)NULL, "id");
	      sndr  = event.getAttribute( (int*)NULL, "sndr");
	      dest  = event.getAttribute( (int*)NULL, "dest");
	      name  = event.getAttributeValueString("name").c_str();
	      role  = event.getAttributeValueString("role").c_str();
	      
	      
	    } catch (std::exception &e) {
	      //	  handle_error(e.what().c_str());
	    }
	    
	    
	    if(sndr == nodeId) return;  // don't want our own stuff
	    
	    cEntityEvent *EntityEventC = new cEntityEvent();
	    
	    EntityEventC->test = ENTITY_EVENT;
	    EntityEventC->id = id;
	    EntityEventC->sndr = sndr;
	    EntityEventC->dest = dest;
	    strcpy(EntityEventC->name,name);
	    strcpy(EntityEventC->role,role);
	    
	    // just call the handle for this kind of event	    
	    processEntityEvent(EntityEventC);
	    delete EntityEventC;
	    //	EntityEventC->addToList();


	    
	  }else {
	    // do nothing for the other types
	  }
	  



}*/

void Rep3DFileObjectEventReceived_cb( ot::CallbackNode * node,  ot::Event & event, void * data ){
  
  int		test;
  int		id;
  int		sndr;
  int		dest;
  int		repOf;
  int		prnt;
  const char	*form;
  double	x;
  double	y;
  double	z;
  double	h;
  double	p;
  double	r;
  const char	*fileObject;
  int assignedPlayerPositionIndex;
  //unpack data from opentracker
  try {
    /*    id    = event.getAttribute( (int*)NULL, "id");
    sndr  = event.getAttribute( (int*)NULL, "sndr");
    dest  = event.getAttribute( (int*)NULL, "dest");
    repOf  = event.getAttribute( (int*)NULL, "repOf");
    prnt  = event.getAttribute( (int*)NULL, "prnt");
    
    form  = event.getAttributeValueString("form").c_str();
    */
    std::vector<float> & pos = event.getPosition();
    x  = pos[0];
    y  = pos[1];
    z  = pos[2];
    /*    h  = event.getAttribute( (double*)NULL, "h");
    p  = event.getAttribute( (double*)NULL, "p");
    r  = event.getAttribute( (double*)NULL, "r");
    fileObject  = event.getAttributeValueString("fileObject").c_str();
    */
    
  } catch (std::exception &e) {
    //handle_error(e.what().c_str());
  }
  
  if(sndr == nodeId)		return ;
  
  /* not using the queue
     if( ((cPDMRMessage *)messageQueueHead) != NULL)
     if(((cPDMRMessage *)messageQueueHead)->remaining() >3)  // don't store more than 3 of these messages
     return ;
  */
  
  /*  cRep3DEvent *Rep3DEventC = new cRep3DEvent() ;
  
  Rep3DEventC->test = REP3D_EVENT;
  Rep3DEventC->id = id;
  Rep3DEventC->sndr = sndr;
  Rep3DEventC->dest = dest;
  Rep3DEventC->repOf = repOf;
  Rep3DEventC->prnt = prnt;
  strcpy(Rep3DEventC->form, form);
  Rep3DEventC->x = x;
  Rep3DEventC->y = y;
  Rep3DEventC->z = z;
  Rep3DEventC->h = h;
  Rep3DEventC->p = p;
  Rep3DEventC->r = r;
  strcpy(Rep3DEventC->fileObject, fileObject);
  
  //	Rep3DEventC->addToList();
  
  
  // extract the message by casting to a rep3devent object
  Rep3DEventC=((cRep3DEvent *)messageQueueHead);
  */
  //------------------------PUCK
  //  if(!strcmp(Rep3DEventC->form, "puck"))
  if ((int)data == 0)
    {
      //      if(!strcmp(Rep3DEventC->fileObject, "position")){
	  otPuckPositionReceived( x, y, z );
	  
	  /*	}
      else {
	if(!strcmp(Rep3DEventC->fileObject, "velocity"))
	  {
	    // velocity updates are not used here
	    }
	    }*/
    }
  /*  else
    //----------------------------PLAYER
        if(!strcmp(Rep3DEventC->form, "player"))

      {
	
	if(!strcmp(Rep3DEventC->fileObject, "takeposition") && Rep3DEventC->dest == nodeId)
	  {
	    serverId         = Rep3DEventC->sndr;
	    assignedPlayerPx = Rep3DEventC->x;
	    assignedPlayerPy = Rep3DEventC->y;
	    assignedPlayerPz = Rep3DEventC->z;
	    assignedPlayerOx = Rep3DEventC->h;
	    assignedPlayerOy = Rep3DEventC->p;
	    assignedPlayerOz = Rep3DEventC->r;
	    printf("takeposition %f %f %f %f\n",assignedPlayerPx,assignedPlayerPy,assignedPlayerPz,assignedPlayerOx);
	    playerPositionReceived = true;
	    
	  }
	else
	  if(!strcmp(Rep3DEventC->fileObject, "data") && numPlayers>0)
	    {
	      
	      int playerPositionIndex;
	      Player *player;
	      x = Rep3DEventC->x;
	      y = Rep3DEventC->y;
	      z = Rep3DEventC->z;
	      h = Rep3DEventC->h;
	      p = Rep3DEventC->p;
	      r = Rep3DEventC->r;
	      playerPositionIndex = lookupPlayer( Rep3DEventC->sndr );
	      
	      // Check if this player has a valid registerplayer index if < 0 then
	      // we need to add them to the list by assigning them to the next free 
	      // slot in our local players list.
	      // This list is just to update the scene graph and draw all players
	      // It does not affect the actual game 
	      if(playerPositionIndex <0)
		{
		  assignedPlayerPositionIndex = getNextFreePositionIndex();
		  printf("Adding new player %d\n",assignedPlayerPositionIndex);
		  players[assignedPlayerPositionIndex]->activate( x,y,z,h,p,r);
		  players[assignedPlayerPositionIndex]->status = ACTIVE;
		  players[assignedPlayerPositionIndex]->positionIndex = assignedPlayerPositionIndex;
		  registeredplayers[assignedPlayerPositionIndex] = Rep3DEventC->sndr;
		  players[0]->score=0;players[1]->score=0;
		  numPlayers++;
		  
		  if(!paddleDataSubscribedTo){
		    //			  register_subscription(REP3D_EVENT,"(event == \"Rep3DFileObjectEvent\") && ((form == \"paddle\"))"); 
		  }

		} else
		  {
		  player = players[playerPositionIndex];
		  player->score=Rep3DEventC->repOf;
		  player->t_lastKeepAlive = timeGetTime()/1000.0;
		  player->setPosition(x,y,z);
		  player->setOrientation(h,p,r);
		  // Apply this players transformation matrix to the scene graph
		  beginEditCP(player->playerTransPtr, Transform::MatrixFieldMask);
		  player->playerTransPtr->setMatrix(player->matrixLocalToWorld);
		  endEditCP(player->playerTransPtr, Transform::MatrixFieldMask);
		}
	      
	    }
	    }*/
    else
      //  if(!strcmp(Rep3DEventC->form, "paddle"))
    if ((int)data >0)
	{
	  //	  if(!strcmp(Rep3DEventC->fileObject, "position")){
	      int playerPositionIndex;
	      playerPositionIndex = (int)data;
	      
	      if(playerPositionIndex <0)
		{
		  printf("warning:player unknown\n");
//		  delete Rep3DEventC;
		  return;
		}
		    
	      Player *player = players[playerPositionIndex];
	      player->paddle->positionLC[0] = x;
	      player->paddle->positionLC[1] = y;
	      player->paddle->positionLC[2] = z;
	      player->paddle->updatePosition();
	      //printf("got paddle dadta from player %d = %f %f %f\n",Rep3DEventC->sndr,player->paddle->positionLC[0],player->paddle->positionLC[1],player->paddle->positionLC[2]);
	      //	    }
	}
  //  delete Rep3DEventC;
  
}



#ifndef INCLUDE_OTINTERFACE
#define INCLUDE_OTINTERFACE
/*
 * interface.h
 * Declare set of functions to dispatch and receive PDMR events. 
 * This is basically an interface between the rest of the program 
 * and the ot procedures - keeping them separate 
 * 
 * Eduardo Veas 27/08/2006
 */ 

#include "Puck.h"
#include "Player.h"




#define ENTITY_EVENT			3000
#define PROCESS_EVENT			3001
#define ENTITYQUERY_EVENT		3002
#define OBJECT_EVENT			3003
#define USER_EVENT				3004
#define SPACE_EVENT				3005
#define REP3D_EVENT				3006


//--------------------send
void otSendPuckData( Puck *puck );
void otSendPlayerRequestToJoin(  void );
void otSendPlayerRequestToStart(  void  );
void otSendEntityEvent( int sndr, int dest);
void otSendPaddleData(  double x, double y, double z);
void otSendPlayerData(  int score, double x, double y, double z,double h, double p, double r);
void otSendPlayerConfirmation( void );
void otSendPlayerKeepAlive(  void );
void otSendPlayerStartPosition(int dest, Real32 x,Real32 y, Real32 z, Real32 h,Real32 p, Real32 r);
void otSendScoreFor(int playerIndex);
void otSendScoreAgainst(int playerIndex);
void otSendRequestToPlayDenied( int dest );
void otSendPowerShotRequest( void );
void otSendPlayerPaddleHit( int playerIndex );
void otSendPlayerWin( int playerIndex );
void otSendNewGame( void  );
void otSendExitGame( void );



//-------------------receive
void otPuckPositionReceived	(int id, double x, double y, double z );
void otPuckVelocityReceived	(int id, double x, double y, double z );
void otPaddlePositionReceived(int id, double x, double y, double z );  // all player related receives need to identify the player with an id



//----------------------general
 void register_subscription(int evenType, char * subString);


#endif //INCLUDE_OTINTERFACE

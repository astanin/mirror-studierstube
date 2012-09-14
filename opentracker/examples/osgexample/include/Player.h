#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "opensg_dep.h"
#include "otcb_obj.h"

#define MAXPLAYERS 2
#define PLAYER_HEIGHT 1.25
// different states the player maybe in
#define INACTIVE 2000
#define ACTIVE  2001
#define PENDING 2002
#define PLAYING 2003
#define DROPPED 2004

OSG_USING_NAMESPACE


class Paddle;

class Player : public otCallbackObj
{
private:
	NodePtr addGeometry(NodePtr root);
	void addCamera(NodePtr root);
	void addSplash( NodePtr root );
	void addWinSplash( void );
	void addScreenScore( void);
	void addLogo();
	void Player::addStart( void );
	void addGoalSplash();
	void addConsolationSplash( void );

	//void makePaddle(NodePtr root);

public:
	void init(NodePtr root);

	//otCallbackObj interface
	void _register();
	static void callback( ot::CallbackNode * node,  ot::Event & event, void * data ) ;
	void sendData();
	//otCallbackObj interface

	void localplayer();
	void requestToJoin( void );
	void confirmPosition( void );
	void activate( Real32 x, Real32 y , Real32 z, Real32 h, Real32 p, Real32 r   );
	void deActivate( void );
	void mouseInput(Real32 x, Real32 y);
	void createPaddle(NodePtr root);

	// position related
	void setPosition(Real32 x,Real32 y,Real32 z);
	void setPosition(Vec3f p);
	Vec3f getPosition( void );

	// orintatation
	void setOrientation(Real32 h,Real32 p,Real32 r);
	void setOrientation(Vec3f p);
	Vec3f getOrientation( void );



	void draw();
	void move();

public:
	Player();
	~Player(void);
	void	broadcastData();

private:
	Vec3f position;
	Vec3f orientation;

public:
	int						positionIndex;
	int						status;
	int						score;
	int						goalTimer;
	int						startButtonPressed;
	int						powerShotCountDown;
	bool					hasMovedPaddle;
	bool					powerShot;
	Real32					t_lastKeepAlive;

	// opensg 
	Matrix					matrixLocalToWorld;
	Vec3f					origin;
	PerspectiveCameraPtr	camera;
	NodePtr					beacon;
	NodePtr					sceneRoot;
	ViewportPtr				viewPort;
	ViewportPtr				viewPortBG;
	TransformPtr			playerTransPtr; 
	TransformPtr			cameraTrans;
	ImageForegroundPtr		logoFrg;
	ImageForegroundPtr		goalForegroundPtr;
	ImageForegroundPtr		winForegroundPtr;
	ImageForegroundPtr		startForegroundPtr;
	ImageForegroundPtr		splashForegroundPtr;
	ImageForegroundPtr		consolationForegroundPtr[5];
	SimpleStatisticsForegroundPtr scoreForegroundPtr;


	// every player needs a paddle
	Paddle *paddle;

};

#endif

#ifndef CLASS_PADDLE
#define CLASS_PADDLE

#include "opensg_dep.h"
#include "otcb_obj.h"
//

OSG_USING_NAMESPACE

#define PADDLE_RADIUS 0.5
#define PADDLE_HALF_HEIGHT 0.1

class Puck;

class Paddle:public otCallbackObj
{

/* Constructor(s) */
public:
	Paddle( void );

public:
	~Paddle(void);

public:

	//otCallbackObj interface
	void _register();
	static void callback( ot::CallbackNode * node,  ot::Event & event, void * data ) ;
	void sendData();
	//otCallbackObj interface

	void init(NodePtr root);
	bool hitTest(Puck *puck, TransformPtr playerTransPtr, bool ps);
	void translate(Real32 x, Real32 y, Real32 z, Matrix m);
	void updatePosition();

	
public:
	TransformPtr paddleTransPtr;   // this is the transform node for the paddle
	Vec3f origin;
	Vec3f positionWC;   // this is position in world coords.
	Vec3f positionLC;   // this is position in world coords.
	Vec3f velocity;   // this is velocity in world coords.
	Real32 r_radius;  // the width of the puck
	Real32 r_halfheight;  // the height

	
private:
	//Player *player;

};

#endif

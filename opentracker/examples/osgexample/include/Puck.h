#ifndef CLASS_PUCK
#define CLASS_PUCK

#include "opensg_dep.h"
#include "otcb_obj.h"
// Activate the OpenSG namespace
OSG_USING_NAMESPACE

/* Default Values */
#define DEFAULT_SPEED 0.01
#define PUCK_HALF_HEIGHT 0.2
#define DEFAULT_RADIUS 0.4

/* Projectile Types */
#define TYPE_SPHERE 0
#define TYPE_CYL 2
#define TYPE_CONE 3
#define TYPE_DISK 4

//Generates a random float in the range [0;1]
#define RANDOM_FLOAT (((float)rand())/RAND_MAX)

class Puck: public otCallbackObj{

private:
	bool goal;
	float puck_floor;
	float halfradius;
	int goalCountDown;
	int startCountDown;


public:
	Real32 t_lastUpdate;
	Vec3f position;
	Vec3f velocity;
	TransformPtr transPtr; 
	Real32 speed;
	float radius;
	//GLint type;
  
public:	/* Methods */
	/* Constructor */
	Puck();
	static void callback( ot::CallbackNode * node,  ot::Event & event, void * data ) ;
	void _register();
	void broadcastData( void );
	void updatePosition();
	void updateVelocity(double vx, double vy,double vz);
	
	void newMatch( void );   // A match lasts until goal is scored
	void newGame( void );    // Right now a game lasts forever or until a new player starts
	bool update( void );
	bool goalScored( void );
	GLint collisionDetection(NodePtr arena);
	NodePtr init();
	// The points used for visualising the ray and hit object
	GeoPositions3fPtr isectPoints;

	// The visualisation geometry, needed for update.
	GeometryPtr testgeocore;

};

#endif

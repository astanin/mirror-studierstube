#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#include "Player.h"
#include "Puck.h"
#include "Pong.h"
#include "scene.h"
#include "utilities.h"

//#include "otinterface.h"

#define PIdiv180 3.14159265359/180.0
#define SPEED 8.0


//extern Player *players[];

/* Constructor */
void Puck::callback( ot::CallbackNode * node,  ot::Event & event, void * data ) {
  Puck * self = reinterpret_cast<Puck*>(data);
  try{
    printf("Puck:: receiving event\n");
    std::vector<float> & pos = event.getPosition();
    std::vector<float> & vel = event.getAttribute((std::vector<float>*)NULL, "velocity");
    self->position = Vec3f(pos[0], pos[1], pos[2]);
    self->updatePosition();
    //    self->velocity = Vec3f(vel[0], vel[1], vel[2]);
    self->updateVelocity(vel[0], vel[1], vel[2]);
  } catch(...){
    printf("PUCK::Error unpacking event\n");
  }
};

void Puck::_register(){
  registerCallback( &Puck::callback, (void*)this);	
}

Puck::Puck():otCallbackObj()
{


	int rr;
	double r;

	goalCountDown=0;
	startCountDown=0;
	puck_floor = PUCK_HALF_HEIGHT;
	/* Initialize position */
	position[0]  = 0.0;
	position[1]  = puck_floor;
	position[2]  = 0.0;

	/* Initialize velocity */
	rr = RandInt(10);

	if(rr <= 5)
	{
		r = RandFloat();
		velocity[0] = cos(r);
		velocity[1] = 0.0;
		velocity[2] = -1.0;
		
	}
	else
	{
		r = RandFloat();
		velocity[0] = cos(r);
		velocity[1] = 0.0;
		velocity[2] = 1.0;
		
	}

	velocity.normalize();
	/* Initialize speed */
	speed = SPEED;
  
	/* Initialize radius */
	radius = DEFAULT_RADIUS;
	halfradius = radius/2.0;
}

NodePtr Puck::init()
{
	// CREATE THE PUCK
    NodePtr puck_trans_node = makeCoredNode<Transform>(&transPtr);
    beginEditCP(transPtr);
	{
		transPtr->getMatrix().setTranslate(position[0],position[1],position[2]);
	}
    endEditCP(transPtr);


    NodePtr puck = OSG::makeCylinder(PUCK_HALF_HEIGHT*2.0,radius, 32, true, true ,true);
    beginEditCP(puck_trans_node);
	{
		puck_trans_node->addChild(puck);
	}
    endEditCP(puck_trans_node);

    SimpleMaterialPtr puck_mat = SimpleMaterial::create();
    beginEditCP(puck_mat);
	{
		puck_mat->setAmbient(Color3f(0.0,0.0,0.0));
		puck_mat->setDiffuse(Color3f(1.0,0.0,0.0));
	}
    endEditCP(puck_mat);

    GeometryPtr puck_geo = GeometryPtr::dcast(puck->getCore());
    beginEditCP(puck_geo);
		puck_geo->setMaterial(puck_mat);
    beginEditCP(puck_geo);

	/////////////////////////////
	// SETUP THE INTERSECTION TEST COMPONENTS
	// Create a small geometry to show the ray and what was hit
    // Contains a line and a single triangle.
    // The line shows the ray, the triangle whatever was hit.
	isectPoints = GeoPositions3f::create();
    beginEditCP(isectPoints);
    {
        isectPoints->addValue(Pnt3f(0,0,0));
        isectPoints->addValue(Pnt3f(0,0,0));
        isectPoints->addValue(Pnt3f(0,0,0));
        isectPoints->addValue(Pnt3f(0,0,0));
        isectPoints->addValue(Pnt3f(0,0,0));
    }
    endEditCP(isectPoints);
	GeoIndicesUI32Ptr index = GeoIndicesUI32::create();     
    beginEditCP(index);
    {
        index->addValue(0);
        index->addValue(1);
        index->addValue(2);
        index->addValue(3);
        index->addValue(4);
    }
    endEditCP(index);

    GeoPLengthsUI32Ptr lens = GeoPLengthsUI32::create();    
    beginEditCP(lens);
    {
        lens->addValue(2);
        lens->addValue(3);
    }
    endEditCP(lens);
    
    GeoPTypesUI8Ptr type = GeoPTypesUI8::create();  
    beginEditCP(type);
    {
        type->addValue(GL_LINES);
        type->addValue(GL_TRIANGLES);
    }
    endEditCP(type);
    
    SimpleMaterialPtr red = SimpleMaterial::create();
    
    beginEditCP(red);
    {
        red->setDiffuse     (Color3f( 1,0,0 ));   
        red->setTransparency(0.5);   
        red->setLit         (false);   
    }
    endEditCP  (red);

    testgeocore = Geometry::create();
    beginEditCP(testgeocore);
    {
        testgeocore->setPositions(isectPoints);
        testgeocore->setIndices(index);
        testgeocore->setLengths(lens);
        testgeocore->setTypes(type);
        testgeocore->setMaterial(red);
    }
    endEditCP(testgeocore);   

	NodePtr testgeo = Node::create();
    beginEditCP(testgeo);
    {
        testgeo->setCore(testgeocore);
    }
    endEditCP(testgeo);
    
	beginEditCP(puck_trans_node);
	{
		puck_trans_node->addChild(testgeo);
	}


	return puck_trans_node;
}

//----------------------------- reset

void Puck::newMatch( void )
{
	int rr;
	double r;

	position = Vec3f(0,puck_floor,0);

	/* Initialize velocity */
	rr = RandInt(10);

	if(rr <= 5)
	{
		r = RandFloat();
		if(RandFloat() > 0.5)
			velocity[0] = cos(r);
		else
			velocity[0] = -cos(r);
		velocity[1] = 0.0;
		velocity[2] = -1.0;
	}
	else
	{
		r = RandFloat();
		if(RandFloat() > 0.5)
			velocity[0] = cos(r);
		else
			velocity[0] = -cos(r);
		velocity[1] = 0.0;
		velocity[2] = 1.0;
	}

	velocity.normalize();
	updatePosition();

	// INITIALISE THE TIMER
	t_lastUpdate = timeGetTime()/1000.0;
	startCountDown=30;
	speed += 0.1;
}

void Puck::newGame( void )
{
	Real32 r;

	position = Vec3f(0,puck_floor,0);
	goal = false;
	/*	for(int i = 0;i<MAXPLAYERS; i++)
	{
		players[i]->score =0;
		}*/


	/* Initialize velocity */
	int rr = RandInt(10);

	if(rr <= 5)
	{
		r = RandFloat();
		if(RandFloat() > 0.5)
			velocity[0] = cos(r);
		else
			velocity[0] = -cos(r);
		velocity[1] = 0.0;
		velocity[2] = -1.0;
		
	}
	else
	{
		r = RandFloat();
		if(RandFloat() > 0.5)
			velocity[0] = cos(r);
		else
			velocity[0] = -cos(r);
		velocity[1] = 0.0;
		velocity[2] = 1.0;
		
	}

	velocity.normalize();
	/* Initialize speed */
	speed = SPEED;
	//	otSendNewGame();

	updatePosition();
	// INITIALISE THE TIMER
	t_lastUpdate = timeGetTime()/1000.0;
	startCountDown = 100;

}


//---------------------------------------:move
bool Puck::update()
{
	//Update Times
	float diffTime, currTime;
	currTime = timeGetTime()/1000.0;
	diffTime = currTime - t_lastUpdate;
	t_lastUpdate = currTime;

	if(goal)  // if goal wait for goalsplash then countdown to start new match
	{
		if(--goalCountDown<= 0) 
		{
			newMatch();
			goal = false;
			
		}
	}

	if(--startCountDown==0)
		speed= SPEED;
	else
	if(startCountDown <0)
		speed += 0.001;
	else
		speed = 0.0;


	/* Update Puck Position */
	position[0] += velocity[0]*speed*diffTime;
	position[2] += velocity[2]*speed*diffTime;

	/// calculate the reflection here...
	// bounce off imaginary walls so puck is channelled nicely from player to player
	// LEFT
	if(position[0]-radius <= -WALLX){
		position[0] = -WALLX+radius;
		velocity[0] = -(velocity[0])+(0.2*RandFloat()-0.1);
		velocity[2] = (velocity[2])+(0.1*RandFloat()-0.05);
	}
	// RIGHT
	if(position[0]+radius >= WALLX){
		position[0] = WALLX-radius;
		velocity[0] = -(velocity[0])+(0.2*RandFloat()-0.1);
		velocity[2] = (velocity[2])+(0.1*RandFloat()-0.05);
	} 

	if(!goal && goalScored())
	{
		goal = true;
		speed = 0.0;
		startCountDown = goalCountDown = 130;
	}

	// adjust translation matrix appropriately
	updatePosition();

	return goal;
}

bool Puck::goalScored()
{
  
	bool result = false;
	// check behind player 2
	if(position[2]-radius <= -WALLZ)
	{
		position[2] = -WALLZ+radius; 
		velocity[0] = (velocity[0])+(0.2*RandFloat()-0.1);
		velocity[2] = -(velocity[2])+(0.1*RandFloat()-0.05);



		// check for goal
/*		if(position[0] >= -2.0 && position[0] <= 2.0)
		{
			if(players[0]->status == ACTIVE)
			{
				players[0]->score++;
				//	otSendScoreFor(0);	
				
				if(players[1]->status == ACTIVE){
				  //	otSendScoreAgainst(1);	
				}
				return true;
			}			
		}
		*/
	}

   // check behing player 1
	if(position[2]+radius >= WALLZ)
	{
		position[2] = WALLZ-radius;
		velocity[0] = (velocity[0])+(0.2*RandFloat()-0.1);
		velocity[2] = -(velocity[2])+(0.1*RandFloat()-0.05);
		/*
		velocity[2] = -(velocity[2]);
		velocity.normalize();*/
		// check for goal
		/*		if(position[0] >= -2.0 && position[0] <= 2.0)
		{
			if(players[1]->status == ACTIVE)
			{
				players[1]->score++;
				//				otSendScoreFor(1);

				if(players[0]->status == ACTIVE){
				  //					otSendScoreAgainst(0);	
				}
				return true;
			}

			}*/
	}
  
	return false;
}
//---------------------------------------------------------: broadcast
void Puck::broadcastData()
{

  //	otSendPuckData( this );

}



//---------------------------------------------------------:updatePosition
GLvoid Puck::updatePosition()
{
     beginEditCP(	transPtr, Transform::MatrixFieldMask );
	{
		transPtr->getMatrix().setTranslate(position);
	}
	endEditCP( transPtr, Transform::MatrixFieldMask);
}

//---------------------------------------------------------:updatePosition
GLvoid Puck::updateVelocity(double x, double y, double z)
{

	velocity = Vec3f((Real32)x,(Real32)y,(Real32)z);

}


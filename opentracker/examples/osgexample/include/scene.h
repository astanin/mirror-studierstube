#ifndef FILE_SCENE
#define FILE_SCENE

#include "opensg_dep.h"
//#include "Player.h"

#define WALLX 6.0
#define WALLY 7.0
#define WALLZ 9.0
#define LIGHTHEIGHT 6.5
#define ARENA_RADIUS 10.0


// Activate the OpenSG namespace
OSG_USING_NAMESPACE


NodePtr createArena( void );
NodePtr makeGround();
NodePtr makeSurround();
NodePtr createScene();
void addSceneCamera(NodePtr);

#define PIdiv180 3.14159265359/180.0

#endif

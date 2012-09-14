
#ifdef WIN32
#include <windows.h>
#endif //win32
#include "text.h"
OSG_USING_NAMESPACE


NodePtr createScoreBoards( void );
TextStuff  *ScoreBoard1;
TextStuff  *ScoreBoard2;



NodePtr createScoreBoards()
{
	GeometryPtr geo;
	NodePtr bg;
	SimpleMaterialPtr m;
	ScoreBoard1 = new TextStuff() ;
	ScoreBoard2 = new TextStuff() ;

	// First get the global group node
    OSG::NodePtr scoreBoardsNodePtr = OSG::Node::create();
    scoreBoardsNodePtr->setCore(OSG::Group::create());

    // Setup text 1
    ScoreBoard1->initialize();
    ScoreBoard1->updateFace();
    ScoreBoard1->updateScore(0,0);
    // Setup text 2
    ScoreBoard2->initialize();
    ScoreBoard2->updateFace();
    ScoreBoard2->updateScore(0,0);

	////////// 1 /////////
	// make its transform
	TransformPtr trans1;
    NodePtr trans_node1 = makeCoredNode<Transform>(&trans1);
    beginEditCP(trans1);
		trans1->getMatrix().setTransform(Vec3f(0,4,-10.5),Quaternion( Vec3f(0,1,0),deg2rad(0)));
    endEditCP(trans1);

	// make geometry
	bg = makePlane(9.3, 1, 8,2);
	 m= SimpleMaterial::create();
    beginEditCP(m);
    {		
        m->setAmbient      (Color3f(0,0,0));
        m->setDiffuse      (Color3f(0.0,0.0,0.0));
    }
    endEditCP  (m);  
	geo = GeometryPtr::dcast(bg->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m);
    beginEditCP(geo);

	beginEditCP(bg);
	bg->addChild(ScoreBoard1->mRootNode);
	endEditCP(bg);

	beginEditCP(trans_node1);
		trans_node1->addChild(bg);
	endEditCP(trans_node1);

	////////// 2 /////////
	// make its transform
	TransformPtr trans2;
    NodePtr trans_node2 = makeCoredNode<Transform>(&trans2);
    beginEditCP(trans2);
		trans2->getMatrix().setTransform(Vec3f(0,4,10.5),Quaternion( Vec3f(0,1,0),deg2rad(180)));
    endEditCP(trans2);

	// make geometry
	bg = makePlane(9.3, 1, 8,2);
	m = SimpleMaterial::create();
    beginEditCP(m);
    {		
        m->setAmbient      (Color3f(0,0,0));
        m->setDiffuse      (Color3f(0.0,0.0,0.0));
    }
    endEditCP  (m);  
	geo = GeometryPtr::dcast(bg->getCore());  
    beginEditCP(geo);
		geo->setMaterial(m);
    beginEditCP(geo);

	beginEditCP(bg);
	bg->addChild(ScoreBoard2->mRootNode);
	endEditCP(bg);

	beginEditCP(trans_node2);
		trans_node2->addChild(bg);
	endEditCP(trans_node2);


	beginEditCP(scoreBoardsNodePtr);
		scoreBoardsNodePtr->addChild(trans_node1);
		scoreBoardsNodePtr->addChild(trans_node2);
	endEditCP(scoreBoardsNodePtr);
  

    return scoreBoardsNodePtr;
}


/*
// react to keys
void keyboard(unsigned char k, int , int )
{
   switch (k)
   {
   case ' ':
	   gTextStuff.updateScene();
	   break;
   case 27:
      {
         OSG::osgExit();
         exit(0);
      }
      break;

   case '-':
      gTextStuff.incFaceSize(false);
      break;
   case '=':
      gTextStuff.incFaceSize(true);
      break;

   case '[':
      gTextStuff.incTextureSize(false);
      break;
   case ']':
      gTextStuff.incTextureSize(true);
      break;

   case '<':
      gTextStuff.incMaxExtent(false);
      break;
   case '>':
      gTextStuff.incMaxExtent(true);
      break;

   case ',':
      gTextStuff.incLineSpacing(false);
      break;
   case '.':
      gTextStuff.incLineSpacing(true);
      break;

   case '{':
      gTextStuff.incGeoScale(false);
      break;
   case '}':
      gTextStuff.incGeoScale(true);
      break;


   case 'f':
      gTextStuff.goToNextFamily();
      break;

   case 'h':
      std::cerr << "Keys:" << std::endl;
      std::cerr << "- =: change face size" << std::endl;
      std::cerr << "{ }: change geo scale" << std::endl;
      std::cerr << "[ ]: change texture size" << std::endl;
      std::cerr << "< >: change max extend for 1st line" << std::endl;
      std::cerr << ", .: change line spacing" << std::endl;
      std::cerr << "f  : next font family" << std::endl;
      std::cerr << "d  : dump scene" << std::endl;
   }
}

void keyboard_special(int k, int , int )
{
   const float offset_inc(0.025f);

   switch(k)
   {
   case GLUT_KEY_LEFT:
      gTextStuff.incOffset(-offset_inc,0);
      break;
   case GLUT_KEY_RIGHT:
      gTextStuff.incOffset(offset_inc,0);
      break;
   case GLUT_KEY_UP:
      gTextStuff.incOffset(0,offset_inc);
      break;
   case GLUT_KEY_DOWN:
      gTextStuff.incOffset(0,-offset_inc);
      break;
   }

}

void initgl(void)
{
   glClearColor(0.1, 0.0, 0.1, 0.0);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
}

// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    int winid = glutCreateWindow("IOV UI Test");

    initgl();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);

    return winid;
}
*/

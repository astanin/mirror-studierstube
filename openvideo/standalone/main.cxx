/**
 * The main function for the standalone program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */

#include <ace/Thread.h>

// Standard includes
#include <iostream>
#include <string>

// OpenVideo includes
#include <openvideo/Manager.h>

#ifdef ENABLE_GLUTSINK
#include <GL/glut.h>
#endif //ENABLE_GLUTSINK

// Using namespaces
using namespace std;
using namespace openvideo;

void pollLoop(void*)
{
    while(true)
    {
        Manager::update(NULL);

    }
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    cout << "Usage : " << argv[0] <<  " configfile" << endl;
    return 1;
  }

#ifdef ENABLE_GLUTSINK
  glutInit(&argc, argv);
#endif //ENABLE_GLUTSINK

  cout << "**************************************" << endl;
  cout << "**** OpenVideo -standalone- v1.0 *****" << endl;
  cout << "**************************************" << endl;
  
  Manager * manager = Manager::getInstance();
  string ovConfig = argv[1];
  manager->parseConfiguration(ovConfig);
  cout << "Parsing complete." << endl;
  manager->initTraversal();
  //ACE_hthread_t* threadHandle = new ACE_hthread_t();
  //if(ACE_Thread::spawn((ACE_THR_FUNC)::pollLoop,
  //    0, 	
  //    THR_NEW_LWP|THR_JOINABLE, 	
  //    0, 	
  //    threadHandle,
  //    0, 	
  //    0, 	
  //    0
  //    )==-1)
  //{ 
  //    printf("error spaning thread\n");
  //}
  manager->run();



  return 0;
}

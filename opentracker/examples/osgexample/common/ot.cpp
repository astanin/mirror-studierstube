#include "ot.h"
#include "otcb_obj.h"
//#include <OpenTracker/core/Context.h>

ot::Context * runtime;

ot::Context * getOpenTrackerRuntime(){
  return runtime;
};

int otInit (int argc, char ** argv){

    const char * filename;
    if( argc != 2 )
    {
        printf( "Usage : %s naming service\n", argv[0]);;
        return 1;
    }
    if (argc > 2)
    {
        filename = argv[2];
    } else {
        filename = argv[1];
    }


    // important parts of the system
    // get a context, the default modules and factories are
    // added allready ( because of the parameter 1 )
    // this call creates the context only, so that the configurator does not start 
    // the configuration thread with an empty context.
#ifdef USE_LIVECONTEXT
	runtime = ot::getLiveContext(argv[1]);
#endif //USE_LIVECONTEXT
    ot::Context & ct = otContext;
    printf( "Context established.\n");
    otCallbackModule;
    otCallforwardModule;

#ifndef USE_LIVECONTEXT
    ct .parseConfiguration(std::string(argv[1]));
	otConfigurator ->runConfigurationThread( filename );
#endif //USE_LIVECONTEXT

	printf( "Parsing complete.\nStarting mainloop !\n");


    

    //run is done by the otInterface method, by calling context.loopOnce()
    //    context.run();
    
    
};

void otExit(void) {
#ifdef USE_LIVECONTEXT
  delete runtime;
#endif //USE_LIVECONTEXT
};


void otInterface( void )
{

  otContext .loopOnce();

}

//void otInterface();

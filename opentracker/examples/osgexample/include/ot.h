#ifndef DPONG_OPENTRACKER_HH_INCLUDED
#define DPONG_OPENTRACKER_HH_INCLUDED

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>

#ifdef USE_LIVECONTEXT
#  define otConfigurator ot::Configurator::instance()
#  define otContext      (* getOpenTrackerRuntime() )
#else
#  define otConfigurator ot::Configurator::instance()
#  define otContext      ot::Configurator::instance() ->getContext()
#endif //USE_LIVECONTEXT

ot::Context * getOpenTrackerRuntime();

int otInit (int argc, char ** argv);
void otExit(void);
void otInterface();


#endif //DPONG_OPENTRACKER_HH_INCLUDED

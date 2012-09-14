#include <OpenTracker/core/Module.h>
#include <OpenTracker/core/OtLogger.h>

namespace ot {

  Context*  context = NULL;

  Module::~Module() {
    //ot::logPrintW("Deleting Module\n");
  };

  OT_IMPLEMENT_IREFCOUNTED(Module);
};

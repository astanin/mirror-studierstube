
#if defined(_WIN32_WCE) && !defined(_DEBUG)

  // include map to prevent this error in WinCE release build:
  // error C2375: 'qsort' : redefinition; different linkage
#  include <stdlib.h>

#endif

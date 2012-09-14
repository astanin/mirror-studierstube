////////////////////////////////////////////
//
// fixes compilation isues for Windows CE
//


// fix for badly ported ACE under VS2005...
//
#if defined(_WIN32_WCE) && (_MSC_VER>=1400)
// ACE needs some defitions from STL before it is included
#  include <map>
#endif //_WIN32_WCE

/* 
 * ------------------------------------------------------------
 *   End of FixWinCE.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */

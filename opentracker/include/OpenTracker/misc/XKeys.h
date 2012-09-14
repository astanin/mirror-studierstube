//----------------------------------------------------------------------
//                Institute for Computer Graphics and Vision
//                  (c) 2002,2003,2004 All Rights Reserved
//----------------------------------------------------------------------
// @author 	Bernhard Reitinger
// created 	Thu Jul 14 09:29:17 2005
//----------------------------------------------------------------------
// $Id$
//----------------------------------------------------------------------

/**
 * This class is a wrapper for handling X key events which are used
 * for the Targus device under linux. This class relies on the code
 * provided by xbindkeys (http://hocwp.free.fr/xbindkeys/xbindkeys.html)
 */

#ifndef _OT_XKEYS_H
#define _OT_XKEYS_H

// this is a linux implementation !!!
#ifndef WIN32

#include <X11/Xlib.h>
#include <iostream>
#include <string>

/**
 * The module and factory to drive the TargusSource node.
 * @author Bernhard Reitinger
 * @ingroup input
 */

namespace ot {

   class XKeys {

   public:
      typedef enum {
	 SYM, 
	 CODE, 
	 BUTTON 
      } KeyType_t;
      
      typedef enum { 
	 PRESS, 
	 RELEASE
      } EventType_t;
      
      typedef struct {
	 KeyType_t type;
	 EventType_t event_type;
	 
	 union {
	    KeySym sym;
	    KeyCode code;
	    unsigned int button;
	 } key;
	 
	 unsigned int modifier;
	 char *command;
      } Keys_t;


   public:

      // ctor
      XKeys()
      { init(); }

      // ctor
      XKeys(const char* rcfile);

      // dtor
      ~XKeys()
      { freeKeys(); }

      // this method loads the RC file by xbindkeys
      bool loadRC(const char* rcfile);

      void showKeys(Display * d);
      void print_key(Display * d, Keys_t * key);
      void grab_keys (Display * dpy);

   protected:
      
      void init();
      void initKeys();
      void freeKeys();

      bool add_key(KeyType_t type, EventType_t event_type, KeySym keysym, KeyCode keycode,
		   unsigned int button, unsigned int modifier, char *command, const char* rc_file);

      
      void set_keysym (Keys_t * key, EventType_t event_type, KeySym keysym,
		       unsigned int modifier, char *command);
      void set_keycode (Keys_t * key, EventType_t event_type, KeySym keycode, 
			unsigned int modifier, char *command);
      void set_button (Keys_t * key, EventType_t event_type, unsigned int button, 
		       unsigned int modifier, char *command);
      void free_key (Keys_t * key);

      void modifier_to_string (unsigned int modifier, char *str);

      void my_grab_button (Display * dpy, unsigned int button, unsigned int modifier,
			   Window win);
      void my_grab_key (Display * dpy, KeyCode keycode, unsigned int modifier,
			Window win);

   public:

      int nb_keys;
      Keys_t *keys;

      unsigned int numlock_mask;
      unsigned int scrolllock_mask;
      unsigned int capslock_mask;

      
   protected:
     
      bool verbose;
      
   };
}

// win32
#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of XKeys.h
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

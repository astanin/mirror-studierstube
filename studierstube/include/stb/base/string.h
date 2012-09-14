/*
** string.h
** 
** Made by Bernhard Reitinger
** Login   <breiting@localhost.localdomain>
** 
** Started on  Thu Feb  9 11:33:07 2006 Bernhard Reitinger
** Last update Thu Feb  9 11:33:07 2006 Bernhard Reitinger
*/

/**
   \brief This file encapsulates basic data structures like string,
   list, and so on. For the PDA-based setup, STL cannot be used.
*/


#ifndef   	_STB_STRING_H_
# define   	_STB_STRING_H_


#include <string>
#include <stb/base/macros.h>

BEGIN_NAMESPACE_STB

// String
///////////////// std::string
#ifdef WIN32
typedef std::string string;
#else
typedef std::string string;
#endif

///////////////// std::strcasecmp

int stricasecmp(const stb::string str1, const stb::string str2);

// return the base name (directory) of a file string
void getBaseName(stb::string& in_string, const stb::string& str, const char* delim);


END_NAMESPACE_STB



#endif 	    /* !_STB_STRING_H_ */

//========================================================================
// End of string.h
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================

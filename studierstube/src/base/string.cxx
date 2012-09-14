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
#include <stb/base/string.h>

BEGIN_NAMESPACE_STB

int stricasecmp(const stb::string str1, const stb::string str2)
{
#if defined(WIN32) || defined(_WIN32_WCE)
    return _stricmp(str1.c_str(),str2.c_str());
#else //LINUX
    #include <strings.h>
    return strcasecmp(str1.c_str(),str2.c_str());
#endif
    return -1;
}

void getBaseName(stb::string& in_string, const stb::string& str, const char* delim)
{
    in_string =  str.substr(0, str.rfind(delim));
}

///////////////// std::strcasecmp

END_NAMESPACE_STB


//========================================================================
// End of string.cxx
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

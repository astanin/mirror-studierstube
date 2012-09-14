//////////////////////////////////////////////////////////////////////////
//
//                    Custom Memory Manager for TinyXML
//
//             written by Daniel Wagner (daniel@icg.tu-graz.ac.at)
//


#include <new>
#ifndef __APPLE__
#include <malloc.h>
#else
#include <cstdio>
#include <cstdlib>
#include <malloc/malloc.h>
#endif

#define TINYXML_INTERNAL
#include <TinyXML/TinyXMLMemManager.h>


static TinyXML::MemoryManager* memManager = 0;
static bool alreadyReservedMemory = false;


extern "C" {

#ifdef WIN32
__declspec(dllexport) 
#endif
bool
setTinyXMLMemoryManager(TinyXML::MemoryManager* nNewManager)
{
	if(!alreadyReservedMemory)
		memManager = nNewManager;

	return !alreadyReservedMemory;
}


}  // extern "C"


//
// new and delete just need to be linked to the DLL
// in order to overload them for the complete module
//

/*
void * operator new(size_t size)
{
	alreadyReservedMemory = true;

	if(memManager)
		return memManager->malloc(size);
	else
#ifndef __APPLE__
	  return ::malloc(size);
#else
	  return malloc(size);
#endif
}


void * operator new[](size_t size)
{
	alreadyReservedMemory = true;

	if(memManager)
		return memManager->malloc(size);
	else
		return ::malloc(size);
}


void operator delete(void *rawMemory)
{
	alreadyReservedMemory = true;

	if(memManager)
		memManager->free(rawMemory);
	else
		::free(rawMemory);
}


void operator delete[](void *rawMemory)
{
	alreadyReservedMemory = true;

	if(memManager)
		memManager->free(rawMemory);
	else
		::free(rawMemory);
}
*/

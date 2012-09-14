//////////////////////////////////////////////////////////////////////////
//
//                    Custom Memory Manager for Coin
//
//             written by Daniel Wagner (daniel@icg.tu-graz.ac.at)
//


#include <new>
#include <malloc.h>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define COIN_DLL_API __declspec(dllexport)
#else
#  define COIN_DLL_API
#endif


#ifndef COIN_INTERNAL
#  define COIN_INTERNAL
#endif

#include "MemManager.h"


static Coin::MemoryManager* memManager = 0;
static bool alreadyReservedMemory = false;


extern "C" {


COIN_DLL_API bool
setCoinMemoryManager(Coin::MemoryManager* nNewManager)
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

void * operator new(size_t size)
{
	if(memManager)
		return memManager->malloc(size);
	else
		return ::malloc(size);
}


void * operator new[](size_t size)
{
	if(memManager)
		return memManager->malloc(size);
	else
		return ::malloc(size);
}


void operator delete(void *rawMemory)
{
	if(memManager)
		memManager->free(rawMemory);
	else
		::free(rawMemory);
}


void operator delete[](void *rawMemory)
{
	if(memManager)
		memManager->free(rawMemory);
	else
		::free(rawMemory);
}



extern "C" {


void* customMalloc( size_t size )
{
	if(memManager)
		return memManager->malloc(size);
	else
		return ::malloc(size);
}


void customFree(void *rawMemory)
{
	if(memManager)
		memManager->free(rawMemory);
	else
		::free(rawMemory);
}


void* customRealloc(void *rawMemory, size_t size)
{
	if(memManager)
		return memManager->realloc(rawMemory, size);
	else
		return ::realloc(rawMemory, size);
}


void* customCalloc(size_t num, size_t size)
{
	if(memManager)
		return memManager->calloc(num, size);
	else
		return ::calloc(num, size);
}


}  // extern "C"

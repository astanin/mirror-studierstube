//////////////////////////////////////////////////////////////////////////
//
//                    Custom Memory Manager for Coin
//
//             written by Daniel Wagner (daniel@icg.tu-graz.ac.at)
//


#ifndef __COIN_MEMMANAGER_HEADERFILE__
#define __COIN_MEMMANAGER_HEADERFILE__


namespace Coin {

  // basic interface for a custom memory manager which can
  // be implemented by an API user...
  //
  class MemoryManager {
  public:
    virtual ~MemoryManager() = 0;

    virtual void* malloc(size_t size) = 0;
  
    virtual void free(void* rawmemory) = 0;
  
    virtual void* realloc(void *rawMemory, size_t size) = 0;

    virtual void* calloc(size_t num, size_t size) = 0;
  };
}	// namespace Coin



// API users can setMemoryManager() to apply their own
// memory manager which has to be derived from 
// Coin::MemoryManager
//
#ifndef COIN_INTERNAL

#ifdef __cplusplus
extern "C" {
#endif

bool setCoinMemoryManager(Coin::MemoryManager* nNewManager);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif




#endif //__COIN_MEMMANAGER_HEADERFILE__

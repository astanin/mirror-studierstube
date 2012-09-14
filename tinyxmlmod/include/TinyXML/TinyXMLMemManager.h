//////////////////////////////////////////////////////////////////////////
//
//                    Custom Memory Manager for TinyXML
//
//             written by Daniel Wagner (daniel@icg.tu-graz.ac.at)
//


#ifndef __TINYXML_MEMMANAGER_HEADERFILE__
#define __TINYXML_MEMMANAGER_HEADERFILE__


namespace TinyXML {


// basic interface for a custom memory manager which can
// be implemented by an API user...
//
class MemoryManager {
public:
	virtual void* malloc(size_t size) = 0;

	virtual void free(void* rawmemory) = 0;

	virtual void* realloc(void *rawMemory, size_t size) = 0;

	virtual void* calloc(size_t num, size_t size) = 0;
};


}  // namespace TinyXML


// API users can setMemoryManager() to apply their own
// memory manager which has to be derived from 
// TinyXMLMemoryManager
//
#ifndef TINYXML_INTERNAL

#ifdef __cplusplus
extern "C" {
#endif

bool setTinyXMLMemoryManager(TinyXML::MemoryManager* nNewManager);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif


#endif //__TINYXML_MEMMANAGER_HEADERFILE__

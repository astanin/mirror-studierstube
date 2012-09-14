
#if defined(WIN32) && (_MSC_VER>=1400)

     // somehow ::TryEnterCriticalSection() is not declared in release build...
     // (needed by ACE)
     BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

#endif //defined(WIN32) && (_MSC_VER>=1400)

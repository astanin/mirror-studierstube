#ifndef SWIG
#include <ace/Task.h>
#endif

#ifndef _PHANTOMLISTENER_H
#define _PHANTOMLISTENER_H


namespace ot {

 class OPENTRACKER_API PhantomListener : public ACE_Task_Base
    {
  public:
    PhantomListener (const std::string& group) : _stop(false) { 
      msr = new Phantom::Utils::MulticastSocketReceiver(group.c_str());
    };

    virtual ~PhantomListener () {
      delete msr;
    };

    void Stop() {
      lock();
      _stop = true;
      unlock();
    };

    void Start() {
      lock();
      _stop = false;
      unlock();
    };

    bool isStopping() {
      lock();
      bool stopping = _stop;
      unlock();
      return stopping;
    };
    
    virtual void addNode(PhantomMiddlewareSource* node) = 0;

    virtual void removeNode(PhantomMiddlewareSource* node) = 0;

    virtual void pushEvent() = 0;
    
 protected:
    inline void lock() {lock_.acquire();};
    inline void unlock() {lock_.release();};
    bool _stop;
    Phantom::Utils::MulticastSocketReceiver* msr;
  };
}

#endif //_PHANTOMLISTENER_H

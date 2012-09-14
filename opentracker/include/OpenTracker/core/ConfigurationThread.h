#ifndef OT_CONFIGURATION_THREAD_HH
#define OT_CONFIGURATION_THREAD_HH

namespace ot{

class ConfigurationThread{

 public:
  virtual ~ConfigurationThread(){};

  virtual void start()=0;
  virtual void closeThread()=0;
  virtual void configTask(){};
  
  static void thread_func(void *data){
      ((ConfigurationThread *)data)->configTask();
       printf( "THREAD_FUNC:: thread finished\n");
  }

};

} //namespace ot
#endif //OT_CONFIGURATION_THREAD_HH

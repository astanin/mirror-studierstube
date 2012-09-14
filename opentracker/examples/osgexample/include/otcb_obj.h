#ifndef DPONG_OPENTRACKER_CALLBACKOBJECT_HH_INCLUDED
#define DPONG_OPENTRACKER_CALLBACKOBJECT_HH_INCLUDED

#include <ot.h>

#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallforwardModule.h>


#define otCallforwardModule dynamic_cast<ot::CallforwardModule*> ( ( otContext ).getModule("CallforwardConfig") )
#define otCallbackModule dynamic_cast<ot::CallbackModule*> ( ( otContext ).getModule("CallbackConfig") )

class otCallbackObj{
protected:
	std::string objname;

	bool registered;
public:
	otCallbackObj();
	virtual ~otCallbackObj();
	static void callback( ot::CallbackNode * node,  ot::Event & event, void * data ) ;
	void setName(const char *);
	virtual void sendData();
	void registerCallback(OTCallbackFunction * , void * obj);
	virtual void deregister();
	virtual void _register();
	
};

void otSendPaddleData(const char * name,  double x, double y, double z);
void otSendPlayerData( const char * name, int score, double x, double y, double z,double h, double p, double r);


#endif //DPONG_OPENTRACKER_HH_INCLUDED

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoInteractionKit.h>
#include <Inventor/events/SoEvent.h> 
#include <Inventor/actions/SoHandleEventAction.h> 
#include <Inventor/SbLinear.h>

#include <Inventor/nodes/SoSphere.h> 
#include <Inventor/nodes/SoCube.h> 

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/fields/SoSFFloat.h> 
#include <Inventor/fields/SoSFInt32.h> 
#include <Inventor/fields/SoSFVec3f.h> 
#include <Inventor/fields/SoSFRotation.h> 
#include <Inventor/fields/SoSFString.h> 
#include <Inventor/fields/SoSFTrigger.h> 
#include <Inventor/fields/SoSFBool.h> 
#include <Inventor/sensors/SoSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stb/kernel/Kernel.h>

// asssign button numbers
#define OTCOM_NONE				0
#define OTCOM_RESETPOSITION		1
#define OTCOM_RESETROTATION		2
#define OTCOM_TOGGLE_ROTATECAMERAAXIS  3

class SoGoGoDeviceKit : public SoBaseKit{
	SO_KIT_HEADER(SoGoGoDeviceKit);

	// PARTS
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(geometry);

public:
	SoGoGoDeviceKit(void);
	static void initClass(void);
	// FIELDS
	SoSFString shape;		// the .iv File with the Cursor geometry

	SoSFTrigger resetPosition;
	SoSFTrigger resetRotation;
//	SoSFTrigger storePosition;
//	SoSFTrigger recallPosition;
	SoSFTrigger measureAdvance;
	SoSFTrigger measureBack;

	SoSFTrigger toggleRotationAxis;
//	SoSFBool showDistance;
//	SoSFString pickedObject;

	// send to OpenTracker
	SoSFInt32 gogoButton;
	//SoSFVec3f weightFactor;
	SoSFFloat weightFactorPos;
	SoSFFloat weightFactorRot;

	// receive from OpenTracker
	SoSFString cursorKey;
	SoSFString cursorValue;

protected:	
	virtual ~SoGoGoDeviceKit(void);
	virtual void handleEvent( SoHandleEventAction *action);
	SbBool setUpConnections(SbBool onOff, SbBool doitalways);

private:
////////// Field Sensors
	SoFieldSensor *shapeSensor;

	SoFieldSensor *resetPosSensor;
	SoFieldSensor *resetRotSensor;
//	SoFieldSensor *storePosSensor;
//	SoFieldSensor *recallPosSensor;
	SoFieldSensor *measureAdvSensor;
	SoFieldSensor *measureBackSensor;
	SoFieldSensor *rotACASensor;

////////// Field Modified callbacks
    static void shapeFM(void * userdata, SoSensor * sensor);
    static void resetPosFM(void * userdata, SoSensor * sensor);
    static void resetRotFM(void * userdata, SoSensor * sensor);
//    static void storePosFM(void * userdata, SoSensor * sensor);
//    static void recallPosFM(void * userdata, SoSensor * sensor);
    static void measureAdvFM(void * userdata, SoSensor * sensor);
    static void measureBackFM(void * userdata, SoSensor * sensor);
	static void rotACAFM(void *userdata, SoSensor * sensor);

	
////////// internal functions
	void loadGeometry();
	SbVec3f *calculateWorldPosition();
//	SoNode *measureLine;

	int measureState;
};

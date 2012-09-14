/* ========================================================================
* Copyright (C) 2006  Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
//////////////////////////////////////////////////////////////////////////////
/////////////////////// todo: integrate event adapter for receiving ot events
///////////////////////       integrate all extra attributes & cleanup kit structure
//////////////////////////////////////////////////////////////////////////////
#include <stb/components/starlight/SoGoGoDeviceKit.h>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
// attention here: it links to the kernel (do starlight should be indipendent??)
#include <stb/kernel/Kernel.h>
#include <stb/kernel/SceneManager.h>
#include <stb/components/event/event.h>

SO_KIT_SOURCE(SoGoGoDeviceKit);

void SoGoGoDeviceKit::initClass(void){
	SO_KIT_INIT_CLASS(SoGoGoDeviceKit, SoBaseKit, "BaseKit");
}
SoGoGoDeviceKit::SoGoGoDeviceKit(void){
	SO_KIT_CONSTRUCTOR(SoGoGoDeviceKit);

// Set up Fields
	SO_KIT_ADD_FIELD(shape, (""));
	SO_KIT_ADD_FIELD(resetPosition, ());
	SO_KIT_ADD_FIELD(resetRotation, ());
//	SO_KIT_ADD_FIELD(storePosition, ());
//	SO_KIT_ADD_FIELD(recallPosition, ());
	SO_KIT_ADD_FIELD(measureAdvance, ());
	SO_KIT_ADD_FIELD(measureBack, ());

	SO_KIT_ADD_FIELD(toggleRotationAxis, ());
//	SO_KIT_ADD_FIELD(showDistance, (true));

//	SO_KIT_ADD_FIELD(pickedObject, (""));

//	SO_KIT_ADD_FIELD(translationField, (0.0f, 0.0f, 0.0f));
//	SO_KIT_ADD_FIELD(rotationField, (0.0f, 0.0f, 0.0f, 1.0f));
	SO_KIT_ADD_FIELD(gogoButton, (0));
//	SO_KIT_ADD_FIELD(confidenceField, (1.0f));

	SO_KIT_ADD_FIELD(cursorKey,(""));
	SO_KIT_ADD_FIELD(cursorValue,(""));

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, this, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(geometry, SoNode, SoCube, TRUE, topSeparator, \x0, TRUE);

	SO_KIT_INIT_INSTANCE();

	// instantiate Sensors
	this->shapeSensor = new SoFieldSensor(SoGoGoDeviceKit::shapeFM, this);
	this->shapeSensor->setPriority(0); // immediate sensor
	this->resetPosSensor = new SoFieldSensor(SoGoGoDeviceKit::resetPosFM, this);
	this->resetPosSensor->setPriority(0); // immediate sensor
	this->resetRotSensor = new SoFieldSensor(SoGoGoDeviceKit::resetRotFM, this);
	this->resetRotSensor->setPriority(0); // immediate sensor
	//this->storePosSensor = new SoFieldSensor(SoGoGoDeviceKit::storePosFM, this);
	//this->storePosSensor->setPriority(0); // immediate sensor
	//this->recallPosSensor = new SoFieldSensor(SoGoGoDeviceKit::recallPosFM, this);
	//this->recallPosSensor->setPriority(0); // immediate sensor
	//this->measureAdvSensor = new SoFieldSensor(SoGoGoDeviceKit::measureAdvFM, this);
	//this->measureAdvSensor->setPriority(0); // immediate sensor
	//this->measureBackSensor = new SoFieldSensor(SoGoGoDeviceKit::measureBackFM, this);
	//this->measureBackSensor->setPriority(0); // immediate sensor
	this->rotACASensor = new SoFieldSensor(SoGoGoDeviceKit::rotACAFM, this);
	//this->rotACASensor->setPriority(0); // immediate sensor

	measureState = 0;
	//cursorEvents=NULL;

	setUpConnections(TRUE, TRUE);
}

SoGoGoDeviceKit::~SoGoGoDeviceKit(void) {
	delete this->shapeSensor;
	delete this->resetPosSensor;
	delete this->resetRotSensor;
	//delete this->storePosSensor;
	//delete this->recallPosSensor;
	//delete this->measureAdvSensor;
	//delete this->measureBackSensor;
	delete this->rotACASensor;
}

void SoGoGoDeviceKit::handleEvent( SoHandleEventAction * action ) {
  const SoEvent *event = action->getEvent();
    event = NULL;
}

// SetupConnections
SbBool SoGoGoDeviceKit::setUpConnections(SbBool onOff, SbBool doItAlways = 0) {

	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onOff, doItAlways);

		// Attach Field Sensors
		shapeFM(this, NULL);
		if (this->shapeSensor->getAttachedField() != &this->shape)
			this->shapeSensor->attach(& this->shape);
		resetPosFM(this, NULL);
		if (this->resetPosSensor->getAttachedField() != &this->resetPosition)
			this->resetPosSensor->attach(& this->resetPosition);
		resetRotFM(this, NULL);
		if (this->resetRotSensor->getAttachedField() != &this->resetRotation)
			this->resetRotSensor->attach(& this->resetRotation);
		//storePosFM(this, NULL);
		//if (this->storePosSensor->getAttachedField() != &this->storePosition)
		//this->storePosSensor->attach(& this->storePosition);
		//recallPosFM(this, NULL);
		//if (this->recallPosSensor->getAttachedField() != &this->recallPosition)
		//	this->recallPosSensor->attach(& this->recallPosition);
		//measureAdvFM(this, NULL);
		//if (this->measureAdvSensor->getAttachedField() != &this->measureAdvance)
		//this->measureAdvSensor->attach(& this->measureAdvance);
		//measureBackFM(this, NULL);
		//if (this->measureBackSensor->getAttachedField() != &this->measureBack)
		//	this->measureBackSensor->attach(& this->measureBack);
		rotACAFM(this, NULL);
		if (this->rotACASensor->getAttachedField() != &this->toggleRotationAxis)
			this->rotACASensor->attach(& this->toggleRotationAxis);

		/////////////////////////////////////////////////////////// 
		//cursorEvents->getAttribute("float", "scalingFactor");
		//cursorEvents->getAttribute("float", "cursorDistance");
		//mover->translation.connectFrom(&tracker->translation);
		//mover->rotation.connectFrom(&tracker->rotation);

	} else {
		// Detach Field Sensors
		if (this->shapeSensor->getAttachedField() != NULL)
			this->shapeSensor->detach();
		if (this->resetPosSensor->getAttachedField() != NULL)
			this->resetPosSensor->detach();
		if (this->resetRotSensor->getAttachedField() != NULL)
			this->resetRotSensor->detach();
		//if (this->storePosSensor->getAttachedField() != NULL)
		//	this->storePosSensor->detach();
		//if (this->recallPosSensor->getAttachedField() != NULL)
		//	this->recallPosSensor->detach();

		//if (this->measureAdvSensor->getAttachedField() != NULL)
		//	this->measureAdvSensor->detach();
		//if (this->measureBackSensor->getAttachedField() != NULL)
		//	this->measureBackSensor->detach();

		if (this->rotACASensor->getAttachedField() != NULL)
			this->rotACASensor->detach();

		// We disconnect BEFORE base class.
		SoBaseKit::setUpConnections(onOff, doItAlways);
	}
	return !(connectionsSetUp = onOff);
}

// Callback for the field sensor.
void SoGoGoDeviceKit::shapeFM(void * userdata, SoSensor * sensor) 
{
	SoGoGoDeviceKit *me = (SoGoGoDeviceKit *)userdata;
	//if (sensor==NULL) return;
	me->loadGeometry();
}

void SoGoGoDeviceKit::loadGeometry() 
{
    const char *geomfile;
	SoSeparator *geom;
   	
	geomfile = shape.getValue().getString();
	// don't try to load an empty file
	if (*geomfile == 0) {	
		return;
	}
    // read geometry into the geometry part
	SoInput::addDirectoryFirst("./");
    SoInput myinput;
	if (!myinput.openFile(geomfile)) 
	{
        SoInput::removeDirectory("./");
        return;
    }
	geom = SoDB::readAll(&myinput);
    myinput.closeFile();
    SoInput::removeDirectory("./");
    if (geom == NULL) return;
	setPart("geometry", geom);
}

// Callback for the field sensor.
void SoGoGoDeviceKit::resetPosFM(void * userdata, SoSensor * sensor) 
{
	SoGoGoDeviceKit *me = (SoGoGoDeviceKit *)userdata;
	if (sensor == NULL)
		return;
	me->gogoButton = OTCOM_RESETPOSITION;
}

// Callback for the field sensor.
void SoGoGoDeviceKit::resetRotFM(void * userdata, SoSensor * sensor) 
{
	SoGoGoDeviceKit *me = (SoGoGoDeviceKit *)userdata;
	if (sensor == NULL)
		return;
	me->gogoButton = OTCOM_RESETROTATION;
}

/*
// TODO: implement store and recall functions

// Callback for the field sensor.
void SoGoGoDeviceKit::storePosFM(void * userdata, SoSensor * sensor) {
	SoGoGoDeviceKit *me = (SoGoGoDeviceKit *)userdata;
	if (sensor == NULL)
		return;
}
// Callback for the field sensor.
void SoGoGoDeviceKit::recallPosFM(void * userdata, SoSensor * sensor) {
	SoGoGoDeviceKit *me = (SoGoGoDeviceKit *)userdata;
	if (sensor == NULL)
		return;
}

*/

// Callback for the field sensor.
void SoGoGoDeviceKit::measureAdvFM(void * userdata, SoSensor * sensor) 
{
	SoGoGoDeviceKit *me = (SoGoGoDeviceKit *)userdata;
	printf ("#### MEASURE ADVANCE ####\n");
	if (sensor == NULL)
		return;
	SbVec3f *pos = me->calculateWorldPosition();
	  printf ("GOT POSITION: %.02f %.02f %.02f", (*pos)[0], (*pos)[1], (*pos)[2]);

	me->measureState = (me->measureState+1)%3;
}

// Callback for the field sensor.
void SoGoGoDeviceKit::measureBackFM(void * userdata, SoSensor * sensor) 
{
	SoGoGoDeviceKit *me = (SoGoGoDeviceKit *)userdata;
	me = NULL;
	if (sensor == NULL)
		return;
 	//me->measureState = (me->measureState-1)%3;   
	//measureStateMachine();
}

// This function calculates the absolute World Position of the GoGoDeviceKit	
SbVec3f *SoGoGoDeviceKit::calculateWorldPosition() 
{
	SbVec3f *translation = new SbVec3f();
	SoSearchAction * searchaction = new SoSearchAction;
	searchaction->setNode(this);
	searchaction->apply(stb::Kernel::getInstance()->getSceneManager()->getSceneRoot());

	SoPath * path = searchaction->getPath();
	assert(path != NULL);

	// TODO: Get Viewport Region from current Studierstube Viewer
	//       (couldn't figure out that yet)
	//const SbViewportRegion vpr = stb::Kernel::getInstance()->getSceneManager()->getDisplayRoot->getByName("SoViewPort");
	//SoGetMatrixAction * getmatrixaction = new SoGetMatrixAction(vpr);		// doesn't work (yet)

	//getmatrixaction->apply(path);
	//SbMatrix transformation = getmatrixaction->getMatrix();

	//SbRotation rotation;
	//SbVec3f scalevector;
	//SbRotation scaleorientation;

	//transformation.getTransform(*translation, rotation, scalevector, scaleorientation);

	return translation;
}

void SoGoGoDeviceKit::rotACAFM(void *userdata, SoSensor * sensor) {
  SoGoGoDeviceKit *me = (SoGoGoDeviceKit *)userdata;
  if (sensor == NULL)
	  return;
  me->gogoButton = OTCOM_TOGGLE_ROTATECAMERAAXIS;
}

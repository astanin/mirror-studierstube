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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Source file for EventSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: EventSource.cpp 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoLists.h>
#include <stb/components/event/EventSource.h>

using namespace std;

EventSource::EventSource( const std::string & node_, const std::string & position_, const std::string & orientation_,
            const std::string & button_, const std::string & confidence_ ) 
            : Node(), changed( false ), setup( false ), node( node_ ), position( position_),
            orientation( orientation_ ),  button( button_), confidence( confidence_ )
{
    positionSensor.setFunction( positionCB );
    positionSensor.setDeleteCallback( deleteCB, this );
    positionSensor.setData( this );

    orientationSensor.setFunction( orientationCB );
    orientationSensor.setDeleteCallback( deleteCB, this );
    orientationSensor.setData( this );

    buttonSensor.setFunction( buttonCB );
    buttonSensor.setDeleteCallback( deleteCB, this );
    buttonSensor.setData( this );

    confidenceSensor.setFunction( confidenceCB );
    confidenceSensor.setDeleteCallback( deleteCB, this );
    confidenceSensor.setData( this );
}

void EventSource::deleteCB( void * data, SoSensor * sensor)
{
    ((SoFieldSensor *)sensor)->detach();
    EventSource * self = (EventSource *)data;
    self->setup = false;
}

void EventSource::positionCB( void * data, SoSensor * sensor)
{
    EventSource * self = (EventSource *) data;
    self->state.timeStamp();
    SoSFVec3f * vector = (SoSFVec3f *)((SoFieldSensor *)sensor)->getAttachedField();
#ifdef USE_OT_1_1
    vector->getValue().getValue(self->state.position[0], 
                                self->state.position[1], 
                                self->state.position[2]);
#else
    vector->getValue().getValue( self->state.getPosition()[0], 
                                 self->state.getPosition()[1], 
                                 self->state.getPosition()[2]);
#endif
    self->changed = true;
}

void EventSource::orientationCB( void * data, SoSensor * sensor)
{

    EventSource * self = (EventSource *) data;
    self->state.timeStamp();
    SoSFRotation * rotation = (SoSFRotation *)((SoFieldSensor *)sensor)->getAttachedField();
    // @todo check if the ordering of the quaternion pars is correct !!!
#ifdef USE_OT_1_1
    rotation->getValue().getValue(  self->state.orientation[0], 
                                    self->state.orientation[1], 
                                    self->state.orientation[2], 
                                    self->state.orientation[3]);
#else
    rotation->getValue().getValue( self->state.getOrientation()[0], 
                                   self->state.getOrientation()[1], 
                                   self->state.getOrientation()[2], 
                                   self->state.getOrientation()[3]);
#endif
    self->changed = true;
}

void EventSource::buttonCB( void * data, SoSensor * sensor)
{
    EventSource * self = (EventSource *) data;
    self->state.timeStamp();
    SoSFInt32 * int32 = (SoSFInt32 *)((SoFieldSensor *)sensor)->getAttachedField();
#ifdef USE_OT_1_1
    self->state.button = int32->getValue();
#else
    self->state.setButton(int32->getValue());
#endif
    self->changed = true;
}

void EventSource::confidenceCB( void * data, SoSensor * sensor)
{   
    EventSource * self = (EventSource *) data;
    self->state.timeStamp();
    SoSFFloat * floatValue = (SoSFFloat *)((SoFieldSensor *)sensor)->getAttachedField();
#ifdef USE_OT_1_1
    self->state.confidence = floatValue->getValue();
#else
    self->state.setConfidence(floatValue->getValue());
#endif
    self->changed = true;
}

int EventSource::isEventGenerator()
{
    return 1;
}

void EventSource::checkForNodes()
{
    setup = true;
    if( positionSensor.getAttachedField() == NULL && position.compare("") != 0)
    {
        setup = false;
        SoField * fieldInstance = getFieldByName( node + "." + position );
        if( fieldInstance != NULL )
        {
            if( fieldInstance->getTypeId() == SoSFVec3f::getClassTypeId())
            {
                positionSensor.attach( fieldInstance );
                setup = true;
            }
        }       
    }
    if( orientationSensor.getAttachedField() == NULL && orientation.compare("") != 0)
    {
        setup = false;
        SoField * fieldInstance = getFieldByName( node + "." + orientation );
        if( fieldInstance != NULL )
        {
            if( fieldInstance->getTypeId() == SoSFRotation::getClassTypeId())
            {
                orientationSensor.attach( fieldInstance );
                setup = true;
            }
        }       
    }
    if( buttonSensor.getAttachedField() == NULL && button.compare("") != 0)
    {
        setup = false;
        SoField * fieldInstance = getFieldByName( node + "." + button );
        if( fieldInstance != NULL )
        {
            if( fieldInstance->getTypeId() == SoSFInt32::getClassTypeId())
            {
                buttonSensor.attach( fieldInstance );
                setup = true;
            }
        }       
    }
    if( confidenceSensor.getAttachedField() == NULL && confidence.compare("") != 0)
    {
        setup = false;
        SoField * fieldInstance = getFieldByName( node + "." + confidence );
        if( fieldInstance != NULL )
        {
            if( fieldInstance->getTypeId() == SoSFFloat::getClassTypeId())
            {
                confidenceSensor.attach( fieldInstance );
                setup = true;
            }
        }       
    }
}

SoField * EventSource::getFieldByName( const string & longName )
{
    string::size_type pos = (unsigned int) longName.find_first_of('.');
    if( pos != string::npos )
    {
        string nodeName = longName.substr(0,pos);
        string fieldName = longName.substr(pos + 1);
        SoNodeList nodeList;
        int len = SoNode::getByName( nodeName.c_str(), nodeList );
        for( int i = 0; i < len; i++ )
        {
            SoNode * nodeInstance = nodeList[i];
            SoField * fieldInstance = nodeInstance->getField( fieldName.c_str());
            if( fieldInstance != NULL )
            {
                return fieldInstance;
            }
        }       
    }
    return NULL;
}

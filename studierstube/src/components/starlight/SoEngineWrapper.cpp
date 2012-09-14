 /* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: stbapi
 * ======================================================================== */
/**
 * implementation of generic SoEngineWrapper engine wrapper node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoEngineWrapper.cpp 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */ 

#include <Inventor/SoLists.h>
#include <Inventor/fields/SoFields.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoDebugError.h>

#include <stb/components/starlight/SoEngineWrapper.h>


unsigned int SoEngineWrapper::classinstances = 0;
SoFieldData * SoEngineWrapper::fieldData = NULL;
const SoFieldData ** SoEngineWrapper::parentFieldData = NULL;
SoEngineOutputData * SoEngineWrapper::outputdata = NULL;
const SoEngineOutputData ** SoEngineWrapper::parentoutputdata = NULL;

SoType SoEngineWrapper::classTypeId;

void SoEngineWrapper::initClass()
{
    SO_NODEENGINE_INIT_CLASS(SoEngineWrapper, SoNodeEngine, "NodeEngine");
}

SoEngineWrapper::SoEngineWrapper()
{
    SO_NODEENGINE_CONSTRUCTOR(SoEngineWrapper);

    instanceFieldData = NULL;
    instanceEngineOutputData = NULL;
    
    SO_NODE_ADD_FIELD(engine, (NULL));
    setup();
}

SoEngineWrapper::~SoEngineWrapper()
{
    if( instanceFieldData != fieldData )
        delete instanceFieldData;
    if( instanceEngineOutputData != outputdata )
        delete instanceEngineOutputData;
}

void SoEngineWrapper::setup(void)
{
    if( instanceFieldData != fieldData )
        delete instanceFieldData;
    if( instanceEngineOutputData != outputdata )
        delete instanceEngineOutputData;
    
    if( engine.getValue() == NULL ) // no engine set, just use standard field data
    {   
        instanceFieldData = fieldData;
        instanceEngineOutputData = outputdata;
    }
    else 
    {
        int i = 0;
        SoEngine * engineInstance = engine.getValue();
        //set up our local FieldData and EngineOutputData instances
        instanceFieldData = new SoFieldData(fieldData);
        instanceEngineOutputData = new SoEngineOutputData(outputdata);
        instanceFieldData->addField(this, "engine", &engine);

        SoFieldList fields;
        engineInstance->getFields( fields );
        SbName name;
        for( i = 0; i < fields.getLength(); i++)
        {
            engineInstance->getFieldName(fields[i], name);
            instanceFieldData->addField(this, name.getString(), fields[i]);
        }
        SoEngineOutputList outputs;
        engineInstance->getOutputs( outputs );
        for( i = 0; i < outputs.getLength(); i++)
        {
            engineInstance->getOutputName(outputs[i], name);
            instanceEngineOutputData->addOutput(this, name.getString(), outputs[i], outputs[i]->getConnectionType());
        }
    }
}  

SbBool SoEngineWrapper::readInstance(SoInput * in, unsigned short flags)
{    
    SbBool result = FALSE;
    // first we try to read in the 'engine' field
    const SoFieldData * fd = this->getFieldData();
    if (fd) {
        SbBool notbuiltin;
        result = fd->read(in, this, FALSE, notbuiltin);
    }
    if( result == FALSE)           
        return result;
    // now that we have an engine or not, we do the setup
    setup();
    // and continue to read in any other fields (for IS references etc)
    if(!SoNodeEngine::readInstance(in, flags))
    {
        return FALSE;
    }
    return TRUE;
}

void SoEngineWrapper::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
    if (this == from) 
        return;
    
    // can't do anything usefull in this case
    if(!from->isOfType(SoEngineWrapper::getClassTypeId()))
        return;

    SoEngineWrapper * other = (SoEngineWrapper *)from;

    // the basic trick is to reset everything to the default
    SoFieldData * toTemp = (SoFieldData *) this->getFieldData();
    SoFieldData * fromTemp = (SoFieldData *) other->getFieldData();

    this->instanceFieldData = fieldData;
    other->instanceFieldData = fieldData;

    // then we get a copy of the engine automatically
    SoNodeEngine::copyContents( from, copyconnections );

    this->instanceFieldData = toTemp;
    other->instanceFieldData = fromTemp;

    // and finally, we only have to do the setup again :)
    // field values are correct, because the engine was copied !!
    // FIXME: I'm not sure if connections are copied correctly
    //        However, it works fine for PROTOs !
    setup();
}

const SoFieldData * SoEngineWrapper::getFieldData() const 
{
    return instanceFieldData;
}

const SoEngineOutputData * SoEngineWrapper::getOutputData() const
{
    return instanceEngineOutputData;
}

void * SoEngineWrapper::createInstance()
{
    return new SoEngineWrapper;
}

SoType SoEngineWrapper::getClassTypeId(void) 
{ 
    return SoEngineWrapper::classTypeId; 
}

SoType SoEngineWrapper::getTypeId(void) const 
{ 
    return SoEngineWrapper::classTypeId; 
} 

void SoEngineWrapper::evaluate(void)
{
}

void SoEngineWrapper::destroy(void)
{
    if( instanceFieldData != fieldData )
        delete instanceFieldData;
    if( instanceEngineOutputData != outputdata )
        delete instanceEngineOutputData;
    instanceFieldData = fieldData;
    instanceEngineOutputData = outputdata;


    // just a copy of SoNodeEngine::destroy because I can not call it
    // directly as it is private !
#if COIN_DEBUG && 0 // debug
    SbName n = this->getName();
    SoType t = this->getTypeId();
    SoDebugError::postInfo("SoNodeEngine::destroy", "start -- '%s' (%s)",
        n.getString(),
        t.getName().getString());
#endif // debug
    
    // evaluate() before we actually destruct. It would be too late
    // during the destructor, as the SoNodeEngine::evaluate() method is pure
    // virtual.
    //
    // The explicit call here is done so attached fields will get the
    // chance to update before we die. SoField::disconnect() will
    // normally call SoNodeEngine::evaluate(), but we disable that feature
    // by setting SoEngineOutput::isEnabled() to FALSE before
    // decoupling.
    
    // need to lock to avoid that evaluateWrapper() is called
    // simultaneously from more than one thread
#ifdef COIN_THREADSAFE
    cc_recmutex_internal_field_lock();
#endif // COIN_THREADSAFE
    this->evaluateWrapper();
#ifdef COIN_THREADSAFE
    cc_recmutex_internal_field_unlock();
#endif // COIN_THREADSAFE
    
    // SoBase destroy().
    inherited::destroy();
    
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoNodeEngine::destroy", "done -- '%s' (%s)",
        n.getString(),
        t.getName().getString());
#endif // debug

}

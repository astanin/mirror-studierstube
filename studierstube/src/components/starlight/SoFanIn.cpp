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
 * implementation of generic SoFanIn engine
 *
 * @author Florian Ledermann, Gerhard Reitmayr
 *
 * $Id: SoFanIn.cpp 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */ 

#include <Inventor/fields/SoFields.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoDebugError.h>

#include <stb/components/starlight/SoFanIn.h>
#include <stb/base/OS.h>


//cannot use this because of special handling of inputs/outputs...
//SO_ENGINE_SOURCE(SoFanIn);
#ifdef __COIN__
unsigned int SoFanIn::classinstances = 0;
SoFieldData * SoFanIn::inputdata = NULL;
const SoFieldData ** SoFanIn::parentinputdata = NULL;
SoEngineOutputData * SoFanIn::outputdata = NULL;
const SoEngineOutputData ** SoFanIn::parentoutputdata = NULL;

SoType SoFanIn::classTypeId;

#else 
SO__ENGINE_VARS(SoFanIn);
#endif

// Constants for all of the multi-value fields
enum TypeConst {
    MFBitMask,
    MFBool,
    MFColor,
    MFEnum,
    MFFloat,
    MFInt32,
    MFMatrix,
    MFName,
    MFNode,
    MFPath,
    MFPlane,
    MFRotation,
    MFShort,
    MFString,
    MFTime,
    MFUInt32,
    MFUShort,
    MFVec2f,
    MFVec3f,
    MFVec4f,
    
    BAD_TYPE
};

void
SoFanIn::initClass()
{
    SO_ENGINE_INIT_CLASS(SoFanIn, SoEngine, "Engine");
}

SoFanIn::SoFanIn(SoType inputType) :
   out(NULL),
   lastChangedField(NULL)
{
    SO_ENGINE_CONSTRUCTOR(SoFanIn);
    setup(inputType);    
}

/// Only used when reading from file
SoFanIn::SoFanIn() :
   out(NULL),
   lastChangedField(NULL)
{
    SO_ENGINE_CONSTRUCTOR(SoFanIn);
}

SoFanIn::~SoFanIn()
{
    delete instanceFieldData;
    delete instanceEngineOutputData;
    
    if (out != NULL) {
        delete out;
        for( int i = 0; i < in.getLength(); i++ )
            delete in[i];
        in.truncate(0);
    }
}

void SoFanIn::setup(SoType inputType)
{
    
    // This handy macro sets up conversionCase, which is used to quickly
    // decide what type we're hooked up to at evaluate() time:
    
#define DECIDE(_class_) \
    (inputType == So##_class_::getClassTypeId()) { conversionCase = _class_; }
    
    // Set up for which switch to use in evaluate() routine:
    if DECIDE(MFBitMask) 
        else if DECIDE(MFBool)    
        else if DECIDE(MFColor)   
        else if DECIDE(MFEnum)    
        else if DECIDE(MFFloat)   
        else if DECIDE(MFInt32)   
        else if DECIDE(MFMatrix)  
        else if DECIDE(MFName)    
        else if DECIDE(MFNode)    
        else if DECIDE(MFPath)    
        else if DECIDE(MFPlane)   
        else if DECIDE(MFRotation)
        else if DECIDE(MFShort)   
        else if DECIDE(MFString)  
        else if DECIDE(MFTime)    
        else if DECIDE(MFUInt32)  
        else if DECIDE(MFUShort)  
        else if DECIDE(MFVec2f)   
        else if DECIDE(MFVec3f)   
        else if DECIDE(MFVec4f)   
#undef DECIDE
        else {
#ifdef STB_IS_DEBUG
        SoDebugError::post("SoFanIn::setup", "Can't fan in field of type %s", inputType.getName().getString());
#endif
        conversionCase = BAD_TYPE;
        return;
    }
    
    //set up our local FieldData and EngineOutputData instances
#ifdef __COIN__
    instanceFieldData = new SoFieldData(inputdata);
    instanceEngineOutputData = new SoEngineOutputData(outputdata);
#else //OSOIV
    instanceFieldData = new SoFieldData(inputData);
    instanceEngineOutputData = new SoEngineOutputData(outputData);
#endif
    
    // create input fields from in0 to in9 with the specified type
    // Instead of SO_ENGINE_ADD_INPUT().   
    for( int i = 0; i < 10; i++ )
    {
        SbString name("in");
        name += SbString(i);
        SoField * f = (SoField *)inputType.createInstance();
        this->in.append(f);
        f->setContainer(this);
        this->instanceFieldData->addField(this, name.getString(), f);
    }

    index.setContainer(this);
    this->instanceFieldData->addField(this,"index",&index);
    index.setValue(-1);
    
    
    // Instead of SO_ENGINE_ADD_OUTPUT().
    this->out = new SoEngineOutput;
    this->instanceEngineOutputData->addOutput(this, "out", this->out, inputType);
    this->out->setContainer(this);

    lastChangedField = in[0];
}  

void SoFanIn::inputChanged(SoField * whichField)
{
    if (index.getValue()<-1 || index.getValue()>=in.getLength()) SoDebugError::post("SoFanIn::inputChanged","Index %d out of range",index.getValue());
    if (index.getValue()<0 || index.getValue()>=in.getLength() || 
        (in[index.getValue()]->isConnected() && whichField==in[index.getValue()])) lastChangedField = whichField;
}

void SoFanIn::evaluate()
{
#define DO_MF(_class_) \
    SO_ENGINE_OUTPUT((*out), _class_, setNum(((_class_ *)lastChangedField)->getNum())); \
    SO_ENGINE_OUTPUT((*out), _class_, setValues(0,((_class_ *)lastChangedField)->getNum(), \
                                               ((_class_ *)lastChangedField)->getValues(0))); \
    break;

    switch(conversionCase) {
        case MFBitMask  : DO_MF(SoMFBitMask);
        case MFBool     : DO_MF(SoMFBool);
        case MFColor    : DO_MF(SoMFColor);
        case MFEnum     : DO_MF(SoMFEnum);
        case MFFloat    : DO_MF(SoMFFloat);
        case MFInt32    : DO_MF(SoMFInt32);
        case MFMatrix   : DO_MF(SoMFMatrix);
        case MFName     : DO_MF(SoMFName);
        case MFNode     : DO_MF(SoMFNode);
        case MFPath     : DO_MF(SoMFPath);
        case MFPlane    : DO_MF(SoMFPlane);
        case MFRotation : DO_MF(SoMFRotation);
        case MFShort    : DO_MF(SoMFShort);
        case MFString   : DO_MF(SoMFString);
        case MFTime     : DO_MF(SoMFTime);
        case MFUInt32   : DO_MF(SoMFUInt32);
        case MFUShort   : DO_MF(SoMFUShort);
        case MFVec2f    : DO_MF(SoMFVec2f);
        case MFVec3f    : DO_MF(SoMFVec3f);
        case MFVec4f    : DO_MF(SoMFVec4f);
#undef DO_MF
        case BAD_TYPE:
            ; // Do nothing, already complained
            break;
        default:
            // Something is seriously wrong:
#ifdef STB_IS_DEBUG
            SoDebugError::post("SoFanIn::evaluate",
                "conversionCase is %d!", conversionCase);
#endif
            break;
    }
}

SbBool
SoFanIn::readInstance(SoInput * in, unsigned short flags)
{
    SbName typeName;
    if (!in->read(typeName) || typeName != "type") {
        SoReadError::post(in,
            "\"type\" keyword is missing, erroneous format for "
            "engine class '%s'.",
            this->getTypeId().getName().getString());
        return FALSE;
    }
    
    SbName typeVal;
    if (!in->read(typeVal)) {
        SoReadError::post(in, "Couldn't read input type for engine.");
        return FALSE;
    }
    
    SoType inputType = SoType::fromName(typeVal);

    if( inputType == SoType::badType() || !inputType.isDerivedFrom(SoMField::getClassTypeId()))
    {
        SoReadError::post(in, "Type %s not a valid type for SoFanIn engine.", typeVal.getString());
        return FALSE;
    }    
    
    this->setup(inputType);
    
    return SoEngine::readInstance(in, flags);
}


void
SoFanIn::writeInstance(SoOutput * out)
{
    if (this->writeHeader(out, FALSE, TRUE)) return;
    
    SbBool binarywrite = out->isBinary();
    
    if (!binarywrite) out->indent();
    out->write("type");
    if (!binarywrite) out->write(' ');
    out->write(this->in[0]->getTypeId().getName());
    if (binarywrite) out->write((unsigned int)0);
    else out->write('\n');
    
    this->getFieldData()->write(out, this);
    this->writeFooter(out);
}

void
SoFanIn::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
    SoFanIn * src = (SoFanIn *)from;
    if (src->out != NULL) {
        this->setup(src->in[0]->getTypeId());
    }
    SoEngine::copyContents(from, copyconnections);
}

const SoFieldData *
SoFanIn::getFieldData() const 
{
    return instanceFieldData;
}


const SoEngineOutputData *
SoFanIn::getOutputData() const
{
    return instanceEngineOutputData;
}

void *
SoFanIn::createInstance()
{
    return new SoFanIn;
}

#ifdef __COIN__
SoType SoFanIn::getClassTypeId(void) { return SoFanIn::classTypeId; }
#endif
SoType SoFanIn::getTypeId(void) const { return SoFanIn::classTypeId; } 

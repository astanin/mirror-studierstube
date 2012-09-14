/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
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
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The source file for the Node class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: Node.cxx 245 2007-05-08 13:31:53Z bornik $
  * @file                                                                   
  * ======================================================================= */

#include <openvideo/Node.h>
#include <openvideo/State.h>

using namespace openvideo;

// constructor
Node::Node()
{
	name="";
	curInDegree=0;
	curOutDegree=0;
	state=NULL;
	curPixelFormat=PIXEL_FORMAT(FORMAT_UNKNOWN);
}

// destructor

Node::~Node()
{
	if(state)
		delete state;
}

State* 
Node::getState()
{
	return state;
}

bool 
Node::setParameter(std::string key, std::string value)
{
	if(key=="name"){
		name=value;
		return true;
	}
	else if(key=="DEF"){
		defName=value;
		return true;
	}
	else if(key=="pixelformat"){
		curPixelFormat= PixelFormat::StringToFormat(value);
		return true;
	}

	
	return false;
}

bool 
Node::validateCurrentPixelFormat()
{
	for(int i=0;i<(int)pixelFormats.size();i++)
	{
		if(pixelFormats.at(i)==curPixelFormat)
			return true;
	}

	return false;
}

const char* 
Node::getName() const
{
	return this->name.c_str();
}

PIXEL_FORMAT Node::getCurrentPixelFormat() const
{
  return curPixelFormat;
}

void 
Node::init()
{
}

void 
Node::start()
{
}

void 
Node::stop()
{
}

void 
Node::process()
{
}

void 
Node::postProcess( )
{
}

void 
Node::preProcess( )
{
}

const char* 
Node::getDefName()
{
	return defName.c_str();
}

std::vector<Node*>* 
Node::getOutputs()
{
	return &(this->outputs);
}

std::vector<Node*>* 
Node::getInputs()
{
	return &(this->inputs);		
}
void 
Node::addOutput( Node * output)
{
	this->outputs.push_back(output);
	this->curOutDegree++;
}

void 
Node::addInput( Node * inputs)
{
	this->inputs.push_back(inputs);
	curInDegree++;
}
int 
Node::getCurInDegree()
{
	return curInDegree;
}

int 
Node::getInDegree()
{
	return (int)inputs.size();
}

int 
Node::getOutDegree()
{
	return (int)outputs.size();
}

int 
Node::getCurOutDegree()
{
	return this->curOutDegree;
}

void 
Node::resetCurInOutDegree()
{
	this->curInDegree=(int)inputs.size();
	this->curOutDegree=(int)outputs.size();
}

void 
Node::decCurOutDegree()
{
	this->curOutDegree--;
}

void
Node::decCurInDegree()
{
	this->curInDegree--;
}

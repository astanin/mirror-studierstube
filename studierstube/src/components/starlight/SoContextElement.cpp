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
 * PROJECT: Studierstube
 * ======================================================================== */
/**
 * source file for SoContextElement.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoContextElement.cpp 31 2005-04-04 15:46:47Z daniel $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#ifdef __COIN__
#include <Inventor/actions/SoAudioRenderAction.h>
#endif

#include <stb/components/starlight/SoContextElement.h>

SbString SoContextElement::emptyString = "";
SoContextElement::NodeContextType SoContextElement::nodeMap;

SO_ELEMENT_SOURCE(SoContextElement);

SoContextElement::~SoContextElement()
{
    context.clear();
    nodeMap.clear();
}

// Initializes the SoModelMatrixElement class
void SoContextElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoContextElement, SoAccumulatedElement);

    SO_ENABLE(SoCallbackAction, SoContextElement);
    SO_ENABLE(SoGLRenderAction, SoContextElement);
    SO_ENABLE(SoGetBoundingBoxAction, SoContextElement);
    SO_ENABLE(SoGetMatrixAction, SoContextElement);
    SO_ENABLE(SoHandleEventAction, SoContextElement);
    SO_ENABLE(SoPickAction, SoContextElement);
    SO_ENABLE(SoSearchAction, SoContextElement);
    // SO_ENABLE(SoHandle3DEventAction, SoContextElement);
#ifdef __COIN__
    SO_ENABLE(SoAudioRenderAction, SoContextElement);
#endif

    nodeMap.clear();
}

// Initializes element
void SoContextElement::init(SoState *state)
{
    SoAccumulatedElement::init( state );
    context.clear();
    nodeMap.clear();
}

// Overrides push() method to copy values from next instance in the stack
void SoContextElement::push(SoState * /*state*/)
{
    SoContextElement * ctxElt = (SoContextElement *) getNextInStack();
    context = ctxElt->context;
    // this comes from OpenInventor,
    // it's not done by the superclass, so we need to do it ourselves.
#ifdef __COIN__
    // Coin provides a dedicated method for this task, that may have 
    // some side effects as well ?!
    copyNodeIds( ctxElt );
#else
    nodeIds = ctxElt->nodeIds;  
#endif
}

void SoContextElement::add(SoState * state, SoNode * node, 
                           const SbName * indices, const SbString * values, const int32_t size )
{
    SoContextElement * elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoContextElement *) getElement(state, classStackIndex);

    elt->add(indices, values, size);

    // Update node id list in element
    elt->addNodeId(node);
}

void SoContextElement::add( SoState * state, SoNode * node, 
                            const SbName * indices, const SoNode ** values, const int32_t size)
{
    SoContextElement * elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoContextElement *) getElement(state, classStackIndex);

    elt->add(indices, values, size);

    // Update node id list in element
    elt->addNodeId(node);
}

void SoContextElement::set(SoState * state, SoNode * node, 
                           const SbName * indices, const SbString * values, const int32_t size)
{
    SoContextElement * elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoContextElement *) getElement(state, classStackIndex);

    elt->set(indices, values, size);

    // Update node id list in element
    elt->setNodeId(node);
}

void SoContextElement::clear(SoState * state, SoNode * node, 
                             const SbName * indices, const int32_t size )
{
    SoContextElement * elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoContextElement *) getElement(state, classStackIndex);

    elt->clear(indices, size);

    // Update node id list in element
    elt->addNodeId(node);
}

void SoContextElement::clear(SoState * state, SoNode * node)
{
    SoContextElement * elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoContextElement *) getElement(state, classStackIndex);

    elt->context.clear();

    // Update node id list in element
    elt->setNodeId(node);
}

const SoContextElement::ContextType & SoContextElement::get(SoState *state)
{
    const SoContextElement *elt;
    elt = (const SoContextElement *)getConstElement(state, classStackIndex);
    return elt->context;
}

SbBool SoContextElement::isSet(SoState *state, const SbName & index)
{
    const SoContextElement *elt;
    elt = (const SoContextElement *)getConstElement(state, classStackIndex);
    return (elt->context.find( index ) != elt->context.end());
}

const SbString & SoContextElement::get(SoState *state, const SbName & index)
{
    const SoContextElement *elt;
    elt = (const SoContextElement *)getConstElement(state, classStackIndex);
    ContextType::const_iterator it = elt->context.find( index );
    if( it != elt->context.end())
        return it->second;
    return emptyString;
}

SoNode * SoContextElement::getNode(SoState *state, const SbName & index)
{
    const SoContextElement *elt;
    elt = (const SoContextElement *)getConstElement(state, classStackIndex);
    ContextType::const_iterator it = elt->context.find( index );
    if( it == elt->context.end())
        return NULL;
    NodeContextType::const_iterator nodeIt = nodeMap.find( it->second );
    if( nodeIt == nodeMap.end())
        return NULL;
    return nodeIt->second;
}

void SoContextElement::add( const SbName * indices, const SbString * values, const int32_t size )
{
    for( int32_t i = 0; i < size; i++ )
    {
		context[indices[i]] = values[i];
    }
}

void SoContextElement::add( const SbName * indices, const SoNode ** values, const int32_t size )
{
    for( int32_t i = 0; i < size; i++ )
    {        
        SbString pointer((unsigned long)values[i]);
		context[indices[i]] = pointer;
        nodeMap[pointer] = (class SoNode *)values[i];
    }    
}

void SoContextElement::set( const SbName * indices, const SbString * values, const int32_t size)
{
    context.clear();
	add( indices, values, size );    
}

void SoContextElement::clear( const SbName * indices, const int32_t size )
{
    for( int32_t i = 0; i < size; i++ )
    {
        context.erase( indices[i]);
    }
}

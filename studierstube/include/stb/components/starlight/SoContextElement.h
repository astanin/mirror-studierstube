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
 * header file for SoContextElement
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoContextElement.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOCONTEXTELEMENT_H_
#define _SOCONTEXTELEMENT_H_

#include <map>

#include <Inventor/elements/SoAccumulatedElement.h>

#include "starlight.h"

/** stores the expanded vector type in compact form. 
 * first of all not for all indices a value is set.
 * the null value is represented by the absence of the index.
 * @ingroup context
 * @author Gerhard Reitmayr
 */
class STARLIGHT_API SoContextElement : public SoAccumulatedElement  
{    
     SO_ELEMENT_HEADER(SoContextElement);
public:	

    class lessName {
    public :
        bool operator()( const SbName & n1, const SbName & n2 ) const
        {
            return (n1.getString() < n2.getString());
        }
    };

    typedef std::map<SbName, SbString, lessName> ContextType;

    typedef std::map<SbName, SoNode *, lessName> NodeContextType;

    virtual ~SoContextElement();

    /// Initializes the SoContextElement class
    static void	initClass();

    /// Initializes element
    virtual void init(SoState *state);

    /// Overrides push() method to copy values from next instance in the stack
    virtual void push(SoState *state);

    /** adds the given value pairs to the context stored
     * in the element
     */
    static void add( SoState * state, SoNode * node, 
        const SbName * indices, const SbString * value, const int32_t size);

    /** adds the given value pairs to the context stored
     * in the element
     */
    static void add( SoState * state, SoNode * node, 
        const SbName * indices, const SoNode ** value, const int32_t size);

    /** replaces the context by the new value pairs.
     */    
    static void set( SoState * state, SoNode * node, 
        const SbName * indices, const SbString * value, const int32_t size );

    /**
     * clears the given indices in the context
     */
    static void clear( SoState * state, SoNode * node, 
        const SbName * indices, const int32_t size );

    /**
     * clears the context entirely
     */
    static void clear( SoState * state, SoNode * node );

    /**
     * returns the entire context as the underlying type
     */
    static const ContextType & get(SoState *state);

    /**
     * tests whether a given index is set
     */
    static SbBool isSet(SoState *state, const SbName & index);
    
    /**
     * returns the value of a given index or the empty string, if
     * it is not set.
     */
    static const SbString & get(SoState *, const SbName & index);

    /**
     * returns the value of a given index or the NULL pointer, if
     * it is not set or does not map to a node entry.
     */
    static SoNode * getNode(SoState *, const SbName & index);

protected:

    /** adds the given value pairs to the context stored
     * in the element
     */
    virtual void add( const SbName * indices, const SbString * value, const int32_t size );

    /** adds the given value pairs to the context stored
     * in the element
     */
    virtual void add( const SbName * indices, const SoNode ** value, const int32_t size );

    /** replaces the context stored in the element by
     * the new value pairs.
     */
    virtual void set( const SbName * indices, const SbString * value, const int32_t size );

    /** clears the given indices from the context
     */
    virtual void clear( const SbName * indices, const int32_t size );

    /** stores the expanded vector type in compact form. 
     * first of all not for all indices a value is set.
     * the null value is represented by the absence of the index.
     */
    ContextType context;

    /// stores the internal map to the node pointers !
    static NodeContextType nodeMap;

    /// the empty string instance returned, if no string is found
    static SbString emptyString;
};

#endif // !defined(_SOCONTEXTELEMENT_H_)

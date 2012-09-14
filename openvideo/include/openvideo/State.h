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
/** The header file for the State class.
  *
  * @author Denis Kalkofen
  *
  * $Id: State.h 184 2006-06-02 14:58:35Z breiting $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STATEOV_H
#define _STATEOV_H


#include "openVideo.h"
#include "PixelFormat.h"

#include <map>
#include <string>
#include <vector>
#include <assert.h>


class ACE_Thread_Mutex;


namespace openvideo {


class DSVLSrc;


class OPENVIDEO_API Buffer
{
public:
	Buffer();

	virtual ~Buffer();

	virtual const unsigned char* getPixels() const  {  return buffer;  }

	virtual int getWidth() const  {  return width;  }

	virtual int getHeight() const  {  return height;  }

	virtual PIXEL_FORMAT getFormat() const  {  return format;  }

	virtual int getLockCounter() const  {  return lockCtr;  }

	virtual void lock();

	virtual void unlock();

	virtual bool isLocked() const  {  return lockCtr>0;  }

	virtual unsigned int getUpdateCounter() const  {  return updateCtr;  }

protected:
	unsigned char*		buffer;
	int					lockCtr;
	unsigned int		updateCtr;
	ACE_Thread_Mutex*	mutex;
	int					width,height;
	PIXEL_FORMAT		format;
};


typedef std::vector<Buffer*> BufferVector;

/**
*@ingroup core
*	A State is used to pass data along OpenVideo's graph.
*	An OpenTracker node passes exactly one state object to all of its children. 
*	Since each node is allowed to create a new State a node can in turn access multiple states.\n
*	The Statet implementation holds variables specific for video data like (image)width, (image)height, image(format) or the current(frame).
*	On top of this, a State implements the ability to share any arbitraty data in its elements map. Therefore It provides functions to set, access and remove those data from the map. \n
*	The following drawing demonstrates the use of multiple State objects in an OpenVideo graph.
*	
*	\image html Stategraph.gif
*	
*/
class OPENVIDEO_API State
{
public:
	/** 
	*	constructor
    */
    State();
   	
	/** 
	*	destructor
    */
	virtual ~State();
	
	/** 
	*	Store the image width.
    */
	int width;

	/** 
	*	Store the image height.
    */
	int height;

	/** 
	*	Store the image format
    */
	PIXEL_FORMAT format;

	/** 
	*	Clear the entire State.
    */
	void clear();

	/** 
	*	Adds an Element to the element map.
    */
	void  addElement(std::string key,void* value);

	/** 
	*	Returns an element from the map.
    */
	void* getElementData(std::string key);

	/** 
	*	Remove an element from the map.
    */
	void  removeElement(std::string key);

	/// Returns the latest updated buffer
	virtual Buffer* getCurrentBuffer()  {  return currentBuffer;  }

	/// Unlocks all frame buffers
	void unlockAllBuffers();


	/// Returns a frame with state STATE_UNUSED. If no frame can be found, NULL is returned
	Buffer* findFreeBuffer();

	int getNumFrames() const  {  return buffers.size();  }

	int getNumLockedBuffers() const;

private:
	/** 
	*	A map to store additional data in the State.
    */
	std::map<std::string,void*> elements;


protected:
	BufferVector buffers;
	Buffer* currentBuffer;
};


/// Synchronizes buffer storage and assignment
/**
 *  Makes sure that a client can get a locked buffer
 *  in one thread without overwriting that buffer variable
 *  from another thread.
 */
// This class prevents a problem that could happen
// in a usual client internal thread:
//
//   buffer = currentBuffer;
//   buffer->lock();
//   ...
//   buffer->unlock();
//
// The internal thread makes a copy of the variable to
// make sure that another thread can update currentBuffer at any time.
// The problem happens if currentBuffer is unlocked between
// 'buffer = currentBuffer;' and 'buffer->lock();'
// To prevent this a client should use BufferSynchronizer which
// provides an atomic action for getting a locked buffer
// without overwriting the variable before it is locked
//
class OPENVIDEO_API BufferSynchronizer
{
public:
	BufferSynchronizer();
	~BufferSynchronizer();

	/// Assigns the new buffer to the internal buffer pointer
	/**
	 *  This methods synchronizes the assignment with the getLocked() operation
	 *  and thereby makes sure that a client can always get a valid, locked buffer object.
	 */
	void assign(Buffer* newBuffer);

	/// Returns a buffer object with a lock count increased by one
	/**
	 *  The calling instance is responsible for unlocking
	 *  the buffer as soon as it is no longer used.
	 *  Note: testing for NULL is suggested since getLocked() might return NULL,
	 *  if no buffer of NULL was assigned!
	 */
	Buffer* getLocked();

protected:
	Buffer*				buffer;
	ACE_Thread_Mutex*	mutex;
};



// ----> inlines <-----
// constructor method.

inline State::State()
{
	clear();
}
inline State::~State()
{
	clear();
}

inline void 
State::clear()
{
	elements.clear();
	width = height =0;
	format = FORMAT_UNKNOWN;
	currentBuffer = NULL;
	// FIXME: should this methods also free all buffers?
}
inline void 
State::addElement(std::string key,void* value)
{
	elements[key]=value;
}

inline void* 
State::getElementData(std::string key)
{
	std::map<std::string,void*>::iterator it = elements.find( key );
    if( it == elements.end())
        return NULL;
 
	return it->second;
}

inline void 
State::removeElement(std::string key)
{
	elements.erase(key);
}


}  //namespace openvideo


#endif //_STATEOV_H


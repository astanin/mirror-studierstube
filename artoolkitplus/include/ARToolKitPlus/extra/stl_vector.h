/* ========================================================================
* PROJECT: ARToolKitPlus
* ========================================================================
* This work is based on the original ARToolKit developed by
*   Hirokazu Kato
*   Mark Billinghurst
*   HITLab, University of Washington, Seattle
* http://www.hitl.washington.edu/artoolkit/
*
* Copyright of the derived and new portions of this work
*     (C) 2006 Graz University of Technology
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
*   Dieter Schmalstieg
*   <schmalstieg@icg.tu-graz.ac.at>
*   Graz University of Technology, 
*   Institut for Computer Graphics and Vision,
*   Inffeldgasse 16a, 8010 Graz, Austria.
* ========================================================================
** @author   Daniel Wagner
*
* $Id: stl_vector.h 180 2006-08-20 16:57:22Z daniel $
* @file
* ======================================================================== */


#include <ARToolKitPlus/OS.h>



#ifndef __ARTKP_VECTOR_HEADERFILE__
#define __ARTKP_VECTOR_HEADERFILE__


#ifdef __ARTKP_NO_STL_VECTOR__

namespace std
{

/// Simple class that implements a similar API as std::vector
template <class TYPE>
class vector
{
public:   
	typedef		TYPE			value_type;
	typedef		TYPE&			reference;
	typedef		const TYPE&		const_reference;
	typedef		TYPE*			pointer;
	typedef		const TYPE*		const_pointer;
	typedef		unsigned int	size_type;
	typedef		const_pointer	const_iterator;
	typedef		pointer			iterator;

	vector()
	{
		array = 0;
		current_size = block_size = 0;
		grow();
	}

	vector(const vector<TYPE> &other)
	{
		array = 0;
		current_size = block_size = 0;
		assign(other.begin(),other.end());
	}

	vector(const_iterator first, const_iterator last)
	{
		array = 0;
		current_size = block_size = 0;
		assign(first,last);
	}

	~vector()
	{
		delete array;
		array = 0;
		current_size = block_size = 0;
	}

	iterator begin()
	{
		return array;
	}

	const_iterator begin() const
	{
		return array;
	}

	iterator end()
	{
		return array+current_size;
	}

	const_iterator end() const
	{
		return array+current_size;
	}

	size_type size() const
	{
		return current_size;
	}

	bool empty() const
	{
		return current_size==0;
	}

	void assign(const_iterator first, const_iterator last)
	{
		current_size = 0;
		resize(last-first);

		for(iterator dst=array; first!=last;)
			*dst++ = *first++;
	}


	/// Adds one element to the tail of the list
	void push_back(const TYPE& item)
	{
		if(current_size==block_size)
			grow();
		array[current_size++] = item;
	}

	void pop_back()
	{
		current_size--;
	}


	/// Returns the element at the given position
	const TYPE& at(size_type index) const
	{
		return array[index];
	}


	TYPE& at(size_type index)
	{
		return array[index];
	}


	/// Empties the list.
	void clear()
	{
		current_size = 0;
	}

	void resize(size_type new_size)
	{
		if(new_size==current_size)
			return;

		TYPE* tmpArray = new TYPE[new_size];
		size_type num = (new_size<current_size ? new_size : current_size);

		for(size_type i=0; i<num; i++)
			tmpArray[i] = array[i];

		delete array;
		array = tmpArray;
		current_size = block_size = new_size;
	}

	const TYPE& operator[](size_type index) const
	{
		return array[index];
	}

	TYPE& operator[](size_type index)
	{
		return array[index];
	}



protected:

	void grow()
	{
		size_t oldSize = current_size;
		resize(current_size+growingSize);
		current_size = oldSize;
	}

	enum {
		growingSize=16
	};

	size_type		current_size;
	size_type		block_size;
	TYPE*			array;
};


}	// namespace std

#else  //__ARTKP_VECTOR_HEADERFILE__

#include <vector>

#endif	//__ARTKP_VECTOR_HEADERFILE__


#endif //__ARTKP_VECTOR_HEADERFILE__

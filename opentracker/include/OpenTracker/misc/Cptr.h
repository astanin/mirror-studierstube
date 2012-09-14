/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** The include file for the basic CountingPtr class.
 *
 * @author Eduardo Veas
 *
 * $Id:  1691 2007-01-04 15:15:44Z veas $
 *
 * @file                                                                   */
/* ======================================================================= */


#ifndef OT_MISC_CPTR_H_INCLUDED
#define OT_MISC_CPTR_H_INCLUDED

#ifndef SWIG
#include <cassert>
#endif

/// counting pointer

/// uses reference counting interface (ircountable ) to keep an object alive
/// for as long as the Cptr instance is alive.
template <class T>
class Cptr{

 protected:
  T * obj;

 public:

  Cptr () :obj(0){};
  Cptr(T * o):obj(0){
    *this = o;
  }; 

  Cptr (const Cptr<T> & p): obj(0){ 
    *this = p;
  };
  
  ~Cptr (){
    if(obj) obj->_deref();
  };

  inline Cptr<T>& operator = (const Cptr<T> &p){

    if(obj) obj->_deref();
    obj=p.obj;
    if(obj) obj->_ref();
    return *this;
  };

  inline Cptr<T>& operator = (T * p){

    if(obj) obj->_deref();
    obj=p;
    if(obj) obj->_ref() ;
    return *this;
  };

  inline T& operator *(){
    assert (obj != 0 && "Tried to access empty obj");
    return *obj;
  };


  inline T* operator ->() const {

    assert (obj != 0 && "Tried to -> empty obj");
    return obj;
  };

  inline operator T* () const{
    return obj;
  } ;

  inline T* item () const {
    return obj;
  }	

  inline bool operator !() {return !(obj);};

  inline bool isValid() const {
    return (obj !=0);
  };

  inline bool operator ==(const Cptr<T> &p) const{

    return (obj == p.obj);
  };

  inline bool operator == (T * o) const{
    return (obj == o);
  };

};

#endif // MEM_CPTR_H_INCLUDED


/* 
 * ------------------------------------------------------------
 *   End of Cptr.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */

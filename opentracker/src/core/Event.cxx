/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *y
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
/** The source file for the multi-modal Event class.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#include <string>
#include <sstream>
#include <vector>

#include <OpenTracker/core/Event.h>
#include <OpenTracker/core/Translator.h>
#include <OpenTracker/core/OtLogger.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

namespace ot {

    // typesafe null reference
    Event Event::null;

    // standard constructor
    Event::Event()
    {
        timeStamp();
    }

    // copy constructor, copying all attributes
    Event::Event(const Event &rv)
    {
        *this = rv;
    }
#ifdef USE_CORBA
    // copy constructor, copying all attributes
    Event::Event(const OT_CORBA::Event &ev)
    {
        CORBA::ULong l = ev.length();
        for (CORBA::ULong i=0; i<l; i++) {
            std::string name((const char*) CORBA::string_dup(ev[i].name));
            CORBA::TypeCode_var tc = ev[i].value.type();
            switch (tc->kind()) {
                case CORBA::tk_short:
                    CORBA::Short s;
                    ev[i].value >>= s;
                    setAttribute(name, (short) s);
                    break;
                case CORBA::tk_boolean:
                    CORBA::Boolean b;
                    ev[i].value >>= CORBA::Any::to_boolean(b);
                    setAttribute(name, (bool) b);
                    break;
                case CORBA::tk_long:
                    CORBA::Long l;
                    ev[i].value >>= l;
                    setAttribute(name, (int) l);
                    break;
                case CORBA::tk_longlong:
                    CORBA::LongLong ll;
                    ev[i].value >>= ll;
                    setAttribute(name, (long) ll);
                    break;
                case CORBA::tk_ulong:
                    CORBA::ULong ul;
                    ev[i].value >>= ul;
                    setAttribute(name, (unsigned long) l);
                    break;
                case CORBA::tk_ushort:
                    CORBA::UShort us;
                    ev[i].value >>= us;
                    setAttribute(name, (unsigned short) us);
                    break;
                case CORBA::tk_double:
                    CORBA::Double d;
                    ev[i].value >>= d;
                    setAttribute(name, (double) d);
                    break;
                case CORBA::tk_float:
                    CORBA::Float f;
                    ev[i].value >>= f;
                    setAttribute(name, (float) f);
                    break;
                case CORBA::tk_string:
                    {
                        const char* msg;
                        ev[i].value >>= msg;
                        CORBA::String_var copy(msg);
                        std::string val((const char*) copy);
                        setAttribute(name, val);
                    }
                    break; 
                case CORBA::tk_sequence:
                    {
                        // got a sequence
                    }
                    break; 
                default:
                    // Value could not be extracted
                    ;
            
            }
            // More complex types should be extracted here
            if (tc->equal(OT_CORBA::_tc_FloatVector)) {
                OT_CORBA::FloatVector* fv;
                if (ev[i].value >>= fv) {
                    CORBA::ULong l = fv->length();
                    std::vector<float> vecF = std::vector<float>(l);
                    for (CORBA::ULong i=0; i < l; i++) {
                        vecF[i] = (float) (*fv)[i];
                    }
                    setAttribute(name, vecF);
                } else {
                    std::cerr << "couldn't extract float vector" << std::endl;
                }

            }
        }
    }
#endif //USE_CORBA


    // destructor, deleting all attributes
    Event::~Event()
    {
        clearAttributes();
    }

#ifdef USE_CORBA
    // convert Event to OpenTracker Event type
    OT_CORBA::Event Event::getCORBAEvent() {
        // code to convert
        int i = 0;
        OT_CORBA::Event ev;
        ev.length(attributes.size());
        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {
            OT_CORBA::EventAttribute att;
            std::string name = (*it).first;
            EventAttributeBase *Att = (*it).second;
            std::string type_name = Att->getGenericTypeName();

            att.name = CORBA::string_dup((const char*) name.c_str());
            if (type_name.compare("bool") == 0) {
                att.value <<= CORBA::Any::from_boolean((CORBA::Boolean) getAttribute(name, (bool) false));
            } else if (type_name.compare("int") == 0) {
                att.value <<= (CORBA::Long) getAttribute(name, (int) 0);
            } else if (type_name.compare("long") == 0) {
                att.value <<= (CORBA::LongLong) getAttribute(name, (long) 0);
            } else if (type_name.compare("short") == 0) {
                att.value <<= (CORBA::Short) getAttribute(name, (short) 0);
            } else if (type_name.compare("unsigned_int") == 0) {
                att.value <<= (CORBA::ULong) getAttribute(name, (long) 0);
            } else if (type_name.compare("unsigned_long") == 0) {
                att.value <<= (CORBA::ULongLong) getAttribute(name, (unsigned long) 0);
            } else if (type_name.compare("unsigned_short") == 0) {
                att.value <<= (CORBA::UShort) getAttribute(name, (unsigned short) 0);
            } else if (type_name.compare("double") == 0) {
                att.value <<= (CORBA::Double) getAttribute(name, (double) 0.0);
            } else if (type_name.compare("float") == 0) {
                att.value <<= (CORBA::Float) getAttribute(name, (float) 0.0f);
            } else if (type_name.compare("string") == 0) {
                std::string dummy("");
                std::string val = getAttribute(&dummy, name);
                att.value <<= (char*) val.c_str();
            } else if (type_name.compare("vector<float>") == 0) {
                std::vector<float> dummy;
                std::vector<float> val = getAttribute(&dummy, name);
                int l = val.size();
                OT_CORBA::FloatVector fv;
                fv.length(l);
                for (CORBA::ULong i=0; i < l; i++) {
                    fv[i] = (CORBA::Float) val[i];
                }
                att.value <<= fv;
            }
            ev[i] = att;
            i++;
        }
        return ev;
    }
#endif //USE_CORBA

    // assignment operator, copying all attributes
    Event& Event::operator=(const Event& rv) //throw (std::runtime_error)
    {
        if( this == &rv ) return *this;

        time = rv.time;
        clearAttributes();
        const AttributeMap &rvMap = rv.attributes;
        for (AttributeMap::const_iterator it = rvMap.begin(); it != rvMap.end(); ++it)
        {
            std::string name = (*it).first;
            EventAttributeBase *rvAtt = (*it).second;
            EventAttributeBase *myAtt = EventAttributeBase::create(rvAtt->getGenericTypeName());
            *myAtt = *rvAtt;
            attributes[name] = myAtt;
        }
        return *this;
    }

    // copy all but standard data fields (used for transformation nodes)
    void Event::copyAllButStdAttr(const Event &rv)
    {
        const AttributeMap &rvMap = rv.attributes;
        for (AttributeMap::const_iterator it = rvMap.begin(); it != rvMap.end(); ++it)
        {
            std::string name = (*it).first;
            if (name != "position" &&
                name != "orientation" &&
                name != "confidence" &&
                name != "button")
            {
                EventAttributeBase *rvAtt = (*it).second;
                EventAttributeBase *newAtt = EventAttributeBase::create(rvAtt->getGenericTypeName());
                *newAtt = *rvAtt;
                delAttribute(name);
                attributes[name] = newAtt;
            }
        }
    }

    // get name of EventAttributeBase by index
    const std::string& Event::getAttributeName(const int index) const throw (std::invalid_argument)
    {
        int i = 0;
        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {
            if (i == index)
                return (*it).first;
            i++;
        }
        std::ostringstream ss;
        ss << "event has no attribute with index '" << index << "', check size using Event::size() before access by index!";
        throw std::invalid_argument(ss.str());
    }

    // get index of EventAttributeBase by name
    int Event::getAttributeIndex(const std::string &name) const throw (std::invalid_argument)
    {
        int index = 0;
        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {
            if ((*it).first == name)
                return index;
            index++;
        }
        std::string errorStr = "event does not have attribute called '" + name + "'";
        throw std::invalid_argument(errorStr);
    }

    // get attribute type
    const std::type_info& Event::getAttributeType(const std::string &name) const throw (std::invalid_argument)
    {
        if (hasAttribute(name))
        {
            EventAttributeBase *att = (*attributes.find(name)).second;
            return att->getType();
        }
        std::string errorStr = "event does not have attribute called '" + name + "'";
        throw std::invalid_argument(errorStr);
    }

    // get generic attribute type name
    const std::string& Event::getAttributeTypeName(const std::string &name) const throw (std::invalid_argument)
    {
        if (hasAttribute(name))
        {
            EventAttributeBase *att = (*attributes.find(name)).second;
            return att->getGenericTypeName();
        }
        std::string errorStr = "event does not have attribute called '" + name + "'";
        throw std::invalid_argument(errorStr);
    }

    // get attribute value encoded in a string
    const std::string Event::getAttributeValueString(const std::string &name) const throw (std::invalid_argument)
    {
        if (hasAttribute(name))
        {
            std::ostringstream ss(std::ostringstream::out|
                                  std::ostringstream::binary);
            EventAttributeBase *att = (*attributes.find(name)).second;
            ss << std::noskipws << *att;
            return ss.str();
        }
        std::string errorStr = "event does not have attribute called '" + name + "'";
        throw std::invalid_argument(errorStr);
    }

    // get position
    std::vector<float>& Event::getPosition()
    {
        return getAttribute("position", std::vector<float>(3, .0f));
    };

    // get orientation
    std::vector<float>& Event::getOrientation()
    {
        std::vector<float> defVec = std::vector<float>(3, .0f);
        defVec.push_back(1.f);
        return getAttribute("orientation", defVec);
    }

    // get confidence
    float& Event::getConfidence()
    {
        return getAttribute("confidence", 1.0f);
    }

    // get button
    unsigned short& Event::getButton()
    {
        return getAttribute(std::string("button"), (unsigned short)0);
    }

    // serialize the event
    void Event::serialize(std::ostream &out) const
    {
        std::ostringstream tmp(std::ostringstream::out|
                               std::ostringstream::binary);

        //        out.precision(24);
        //        out << "{" << std::fixed << time << "-" << attributes.size() << ":";

        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {

            if (it != attributes.begin())
                tmp << ",";
            std::ostringstream tmp_sstream(std::ostringstream::out|
                                           std::ostringstream::binary);
            tmp_sstream << std::noskipws << *((*it).second);
            tmp << std::noskipws << (*it).second->getGenericTypeName() << "." << (*it).first << "#" << tmp_sstream.str().length() << "=" << tmp_sstream.str();
        }

        out <<std::noskipws << "{" << std::fixed << time << "-" << attributes.size() << "<" << tmp.str().length() << ":" << tmp.str() << "}";
    }

    const std::string Event::serialize() const
    {
        std::ostringstream ss(std::ostringstream::out|
                              std::ostringstream::binary);

        ss << std::noskipws << *this;
        return ss.str();
    }

    void Event::deserialize(std::string &str)
    {
        std::istringstream ss(str,
                              std::istringstream::out|
                              std::istringstream::binary);
        ss >> std::noskipws >> *this;
    }

    // deserialize the event
    std::istream& Event::deserialize(std::istream &in)
    {
        char* dataCArray,*attrCArray, *dataTmpArray;
        std::string dataStr, typeStr, nameStr, attrStr, sizeStr;
        std::string::size_type typeDelimiter, nameDelimiter,  sizeDelimiter; //attrDelimiter,
        AttributeMap::size_type i, mapSize, eventSize;
        char c;

        clearAttributes();

        // read "{", time , "-", size, ":" and data string
        if (!(in >> c) || c != '{'
            || !(in >> time)
            || !(in >> c) || c != '-'
            || !(in >> mapSize)
            || !(in >> c) || c != '<'
            || !(in >> eventSize)
            || !(in >> c) || c != ':')
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }

        dataCArray=new char[eventSize+2];

        in.read(dataCArray,eventSize+2);

        dataStr = std::string(dataCArray,eventSize);
        dataTmpArray = dataCArray;

        // read data
        for (i = 0; i < mapSize; i++)
        {

            // segmentation of data string
            typeDelimiter = dataStr.find(".");
            nameDelimiter = dataStr.find("#");
            sizeDelimiter = dataStr.find("=");

            typeStr = dataStr.substr(0, typeDelimiter);
            nameStr = dataStr.substr(typeDelimiter + 1, nameDelimiter - (typeDelimiter + 1));
            sizeStr = dataStr.substr(nameDelimiter + 1, sizeDelimiter - (nameDelimiter + 1));

            int size;
            size=atoi(sizeStr.c_str());

            attrStr = dataStr.substr(sizeDelimiter + 1, size);
            attrCArray=&dataTmpArray[sizeDelimiter+1];

            dataTmpArray += sizeDelimiter + 2 + size;
            dataStr = eventSize >= static_cast<AttributeMap::size_type>(dataTmpArray - dataCArray) ? std::string(dataTmpArray,eventSize - (dataTmpArray - dataCArray)): std::string();
            // check if segmentation is correct
            if (typeDelimiter == std::string::npos
                || nameDelimiter == std::string::npos
                || sizeDelimiter == std::string::npos
                || typeDelimiter >= nameDelimiter
                || typeDelimiter >= sizeDelimiter
                || hasAttribute(nameStr))
            {
                in.setstate(std::ios_base::failbit);
                delete dataCArray;
                return in;
            }
            try
            {
                EventAttributeBase *att = EventAttributeBase::create(typeStr);
#ifndef WIN32
                std::stringstream ss(std::stringstream::in|
                                     std::stringstream::out|
                                     std::stringstream::binary);
                ss.write(attrCArray,size);
                if (ss >> std::noskipws >> *att)
                    attributes[nameStr] = att;
#else
                std::ostringstream os(std::ostringstream::out|
                                      std::ostringstream::binary);
                os.write(attrCArray,size);

                std::istringstream is(os.str(), 
                                      std::istringstream::in|
                                      std::istringstream::binary);

                if (is >> std::noskipws >> *att)
                    attributes[nameStr] = att;
#endif
                else
                {
                    in.setstate(std::ios_base::failbit);
                    delete dataCArray;
                    return in;
                }
            }
            catch (std::runtime_error)
            {
                in.setstate(std::ios_base::failbit);
                delete dataCArray;
                return in;
            }
        }
        delete dataCArray;
        return in;
    }

    // add attribute using strings encoding type and value
    bool Event::addAttribute(const std::string &type, const std::string &name, const std::string &value) //throw (std::runtime_error)
    {
        if (hasAttribute(name))
            return false;

        EventAttributeBase *att = EventAttributeBase::create(type); // may throw std::runtime_error
        std::istringstream ss(value, 
                              std::istringstream::in |
                              std::istringstream::binary);
        if (ss >> std::noskipws >> *att)
        {
            attributes[name] = att;
            return true;
        }
        else
        {
            delete att;
            return false;
        }
    }

    // set attribute using a string encoding the value
    bool Event::setAttribute(const std::string &type, const std::string &name, const std::string &value) //throw (std::runtime_error)
    {
        if (!hasAttribute(name))
            attributes[name] = EventAttributeBase::create(type); // may throw std::runtime_error
        EventAttributeBase *att = attributes[name];
        std::istringstream ss(value,
                              std::istringstream::in |
                              std::istringstream::binary);
        if (ss >>std::noskipws >> *att)
            return true;
        return false;
    }

    // delete attribute by name
    bool Event::delAttribute(const std::string &name)
    {
        AttributeMap::iterator it = attributes.find(name);
        if (it == attributes.end())
            return false;
        else
        {
            delete (*it).second;
            (*it).second = NULL;
            attributes.erase(it);
            return true;
        }
    }

    // delete all attributes
    void Event::clearAttributes()
    {
        for (AttributeMap::iterator it = attributes.begin(); it != attributes.end(); ++it)
            delete (*it).second;
        attributes.clear();
    }

    // print out all attributes
    void Event::printout() const
    {
	logPrintI("%s \n",getPrintOut().c_str());
        //std::cout << getPrintOut() << std::endl;
    }

    // get string for print out
    const std::string Event::getPrintOut() const
    {        
        std::ostringstream ss;

        ss << "  timestamp: " << std::fixed << time << std::endl;

        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
            ss << "  " << (*it).first << " (" << (*it).second->getGenericTypeName() << "): " << *((*it).second) << std::endl;
		return ss.str();
    }

    // get string for print out
	void Event::getPrintOut(std::string &printOut) const
    {       
        std::ostringstream ss;

        ss << "  timestamp: " << std::fixed << time << std::endl;

        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
            ss << "  " << (*it).first << " (" << (*it).second->getGenericTypeName() << "): " << *((*it).second) << std::endl;

		printOut = ss.str();     
    }

    // rename attribute
    bool Event::renAttribute(const std::string &oldName, const std::string &newName)
    {
        AttributeMap::iterator oldIt = attributes.find(oldName);
        AttributeMap::iterator newIt = attributes.find(newName);

        if (newIt != attributes.end() || oldIt == attributes.end())
            return false;
        else
        {
            attributes[newName] = (*oldIt).second;
            attributes.erase(oldIt);
            return true;
        }
    }

    // check for attribute
    bool Event::hasAttribute(const std::string &name) const
    {
        return (attributes.find(name) != attributes.end());
    }

    // check whether type name was registered
    bool Event::knowsType(const std::string typeName)
    {
        return EventAttributeBase::translator.knowsType(typeName);
    }

    // register all known types
    void Event::registerAllKnownTypes()
    {
        registerGenericTypeName((bool*)NULL, "bool");
        registerGenericTypeName((char*)NULL, "char");
        registerGenericTypeName((signed char*)NULL, "signed_char");
        registerGenericTypeName((unsigned char*)NULL, "unsigned_char");
        registerGenericTypeName((int*)NULL, "int");
        registerGenericTypeName((long*)NULL, "long");
        registerGenericTypeName((short*)NULL, "short");
        registerGenericTypeName((unsigned int*)NULL, "unsigned_int");
        registerGenericTypeName((unsigned long*)NULL, "unsigned_long");
        registerGenericTypeName((unsigned short*)NULL, "unsigned_short");
        registerGenericTypeName((double*)NULL, "double");
        registerGenericTypeName((long double*)NULL, "long_double");
        registerGenericTypeName((float*)NULL, "float");
        registerGenericTypeName((std::string*)NULL, "string");
        registerGenericTypeName((std::vector<float>*)NULL, "vector<float>");
    }

    // timestamp the event to current time
    void Event::timeStamp()
    {
        time = OSUtils::currentTime();
    }

    // stream the event to an input stream
    std::istream& operator>>(std::istream &in, ot::Event &event)
    {
        event.deserialize(in);
        return in;
    };

    // stream the event to an output stream
    std::ostream& operator<<(std::ostream &out, const ot::Event &event)
    {
        event.serialize(out);
        return out;
    };

} // namespace ot

/*
 * ------------------------------------------------------------
 *   End of Event.cxx
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

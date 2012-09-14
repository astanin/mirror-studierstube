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
/** The header file for the multi-modal Event class.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#ifndef EVENT_HEADER
#define EVENT_HEADER

#ifndef SWIG
#include <string>
#include <vector>
#include <stdexcept>
#endif

#include "../dllinclude.h"
#include "EventAttribute.h"
#include "EventAttributeBase.h"
#include "iostream_ext.h"
#include "OSUtils.h"

#ifdef USE_CORBA
#ifndef SWIG
#include <OpenTracker/skeletons/OT_CORBA.hh>
#endif
#endif //USE_CORBA

namespace ot
{
    /**
     * A map matching attribute names to according EventAttributeBase
     * pointers. The only place where the names of attributes are stored.
     */
    typedef std::map<std::string, EventAttributeBase*> AttributeMap;
    /**
     * Copies the values of a vector into an array. Convenience function
     * for switching between arrays used in old OT11 code and vectors in
     * newer code.
     * @param vector the vector that is copied
     * @param array the array that the values are copied to (must have the same size as @p vector!)
     * @return pointer to the array
     */
    template <typename T>
    static T* copyV2A(const std::vector<T> &vector, T* array)
    {
        for (unsigned int i = 0; i < vector.size(); i++)
            array[i] = vector[i];
        return array;
    };
    /**
     * Copies the values of an array into a vector. Convenience function
     * for switching between arrays used in old OT11 code and vectors in
     * newer code.
     * @param array the array that is copied
     * @param arraySize how many values should be copied
     * @param vector the vector that the values are copied to
     * @return reference to the vector
     */
    template <typename T>
    static std::vector<T>& copyA2V(const T *array, const int arraySize, std::vector<T> &vector)
    {
        vector.clear();
        for (int i = 0; i < arraySize; i++)
            vector.push_back(array[i]);  // do not use operator[] here, it will not work, since no memory will be allocated!
        return vector;
    };
    /**
     * Copies the values of an array into a vector. Convenience function
     * for switching between arrays used in old OT11 code and vectors in
     * newer code.
     * @param array the array that is copied
     * @param arraySize how many values should be copied
     * @return reference to the vector
     */
    template <typename T>
    static const std::vector<T> copyA2V(const T *array, const int arraySize)
    {
        std::vector<T> vector;
        for (int i = 0; i < arraySize; i++)
            vector.push_back(array[i]);  // do not use operator[] here, it will not work, since no memory will be allocated!!
        return vector;
    };


    /**
     * The Event, which is passing through the configuration tree and is holding all multi-modal
     * attributes. This class implements all kinds of functions necessary to deal with events and
     * their attributes. Attributes can be added, read, changed, or deleted. This class encapsulates
     * access to all attributes. It is therefore the only interface to deal with the values of
     * the attributes.
     *
     * This class replaces the obsolete State class, which was used in OpenTracker 1.1.
     * @author Jochen von Spiczak
     * @ingroup core
     */
    class OPENTRACKER_API Event
    {
    public:
        /**
         * Standard constructor. The event gets a current timestamp.
         */
        Event();
        /**
         * Copy constructor. Copies all attributes of another event into the new one.
         * @param rv the right-value, which is the event to copy from
         */
        Event(const Event &rv);

        /**
         * Copy constructor. Copies all attributes from the CORBA OT_CORBA::Event event type into the new one.
         * @param ev the corba event, which is the event to copy from
         */
#ifdef USE_CORBA
        Event(const OT_CORBA::Event &ev);
#endif

        /**
         * Destructor, deleting all attributes held by the event.
         */
        ~Event();


        /**
         * Assignment operator. Copies all attributes of another event into this one.
         * @param rv the right-value, which is the event to copy from
         */
        Event& operator=(const Event &rv); //throw (std::runtime_error);
        /**
         * Checks for the existence of an attribute by its name.
         * @param name the name of the attribute to check for
         * @return whether the event has an attribute called @p name
         */
        bool hasAttribute(const std::string &name) const;
        /**
         * Deletes an attribute.
         * @param name name of the attribute that should be deleted
         * @return whether deletion was successful
         */
        bool delAttribute(const std::string &name);
        /**
         * Renames an attribute.
         * @param oldName the name of the attribute that should be renamed
         * @param newName the new name
         * @return whether renaming was successful
         */
        bool renAttribute(const std::string &oldName, const std::string &newName);
        /**
         * Copies all but standard attributes, which were used in OT11. Standard attributes are
         * position, orientation, button, and confidence.
         * @param rv the right-value, which is the event to copy from
         */
        void copyAllButStdAttr(const Event &rv);
        /**
         * Deletes all attributes held by the event.
         */
        void clearAttributes();
        /**
         * Prints out all data held by the event. Uses the standard output device. This function
         * should be used mainly for debugging purposes.
         */
        void printout() const;
        /**
         * Returns a string that can be used to print out all data held by the event. For a more
         * compact layout of the same data use the serialize functions or streaming operators.
         * @return string holding the event's data
         */
        const std::string getPrintOut() const;
		/**
         * Returns a string that can be used to print out all data held by the event. For a more
         * compact layout of the same data use the serialize functions or streaming operators.
         * @return string holding the event's data
         */
		void getPrintOut(std::string &outstr) const;
        /**
         * Updates the event's timestamp to the current time.
         */
        void timeStamp();
        /**
         * Serializes the event's data into an output stream.
         * @param out the output stream
         */
        void serialize(std::ostream &out) const;
        /**
         * Serializes the event's data into a string.
         * @return a string encoding the event's data
         */
        const std::string serialize() const;
        /**
         * Deserializes an event from an input stream. Naturally, the data coming from the input
         * stream must be a correctly serialized event.
         * @param in the input stream
         * @return the input stream
         */
        std::istream& deserialize(std::istream &in);
        /**
         * Deserializes an event from a string. Naturally, the data in the string must be a
         * correctly serialized event.
         * @param str the string encoding the event
         */
        void deserialize(std::string &str);
        /**
         * Return the number of attributes held by the event.
         * @return number of attributes
         */
        inline int getSize() const { return (int)attributes.size(); };
        /**
         * Returns the RTTI type information of an attribute's value. An exception is thrown if
         * the name is not valid.
         * @param name the name of the attribute
         * @return RTTI type information
         */
        const std::type_info& getAttributeType(const std::string &name) const throw (std::invalid_argument);
        /**
         * Return the generic type name of an attribute's value. An exception is thrown if the
         * name is not valid.
         * @param name the name of the attribute
         * @return the generic type name
         */
        const std::string& getAttributeTypeName(const std::string &name) const throw (std::invalid_argument);
        /**
         * Returns the name of an attribute by index. An exception is thrown if the index is
         *  not valid.
         * @param index the index of the attribute
         * @return the name of the attribute
         */
        const std::string& getAttributeName(const int index) const throw (std::invalid_argument);
        /**
         * Returns the index of an attribute by name. An exception is thrown if the name is not
         * valid.
         * @param name the name of the attribute
         * @return the index of the attribute
         */
        int getAttributeIndex(const std::string &name) const throw (std::invalid_argument);


        /**
         * @name Convenience Functions to Access Former Standard Attributes
         * This set of methods does not add any functionality, it just provides an easy and
         * convenience way to access former standard attributes (position, orientation, button,
         * and confidence). These functions can also be used for easily porting applications using
         * the fixed data structure of OpenTracker 1.1 states to new multi-modal events.
         *
         * All get-methods may throw std::invalid_argument exceptions if attributes with the same
         * name but of another type exist!
         */

        //@{

        /**
         * Returns the value of the attribute 'position', which is a float vector of size 3.
         * @return position attribute
         */
        std::vector<float>& getPosition();
        /**
         * Returns the value of the attribute 'position', which is a float vector of size 3.
         * @return position attribute
         */
        inline const std::vector<float>& getPosition() const { return (const_cast<Event *>(this))->getPosition(); }
        /**
         * Returns the value of the attribute 'orientation', which is a float vector of size 4.
         * @return orientation attribute
         */
        std::vector<float>& getOrientation();
        /**
         * Returns the value of the attribute 'orientation', which is a float vector of size 4.
         * @return orientation attribute
         */
        inline const std::vector<float>& getOrientation() const { return (const_cast<Event *>(this))->getOrientation(); }
        /**
         * Returns the value of the attribute 'confidence', which is a single float value.
         * @return confidence attribute
         */
        float& getConfidence();
        /**
         * Returns the value of the attribute 'confidence', which is a single float value.
         * @return confidence attribute
         */
        const float& getConfidence() const { return (const_cast<Event *>(this))->getConfidence(); }
        /**
         * Returns the value of the attribute 'button', which is a single unsigned short value.
         * @return button attribute
         */
        unsigned short& getButton();
        /**
         * Returns the value of the attribute 'button', which is a single unsigned short value.
         * @return button attribute
         */
        const unsigned short& getButton() const { return (const_cast<Event *>(this))->getButton(); }
        /**
         * Sets the value of the attribute 'position'.
         * @param value the new position value (size 3 expected)
         */
        inline void setPosition(const std::vector<float> &value) { setAttribute("position", value); };
        /**
         * Sets the value of the attribute 'position'.
         * @param value the new position value (size 3 expected)
         */
        inline void setPosition(const float *value) { setAttribute("position", copyA2V(value, 3)); };
        /**
         * Sets the value of the attribute 'orientation'.
         * @param value the new orientation value (size 4 expected)
         */
        inline void setOrientation(const std::vector<float> &value) { setAttribute("orientation", value); };
        /**
         * Sets the value of the attribute 'orientation'.
         * @param value the new orientation value (size 4 expected)
         */
        inline void setOrientation(const float *value) { setAttribute("orientation", copyA2V(value, 4)); };
        /**
         * Sets the value of the attribute 'confidence'.
         * @param value the new confidence value
         */
        inline void setConfidence(const float &value) { setAttribute("confidence", value); };
        /**
         * Sets the value of the attribute 'button'.
         * @param value the new button value
         */
        inline void setButton(const unsigned short &value) { setAttribute("button", value); };

        //@}


        /**
         * @name Non Template Interface
         * This set of methods allows access of attributes if the attribute's type is not known
         * yet during compile time. Therefore, type and value of attributes are encoded in strings.
         */
        //@{

        /**
         * Adds a new attribute. The attribute's type and value are encoded in strings. Throws an
         * exception if type is not known.
         * @param type the generic type name of the attribute
         * @param name the name of the attribute
         * @param value the value of the new attribute
         * @return whether creation of the new attribute was successful
         */
        bool addAttribute(const std::string &type, const std::string &name, const std::string &value); //throw (std::runtime_error);
        /**
         * Sets the value of an attribute. The type and the value of the attribute are encoded in
         * strings. If currently no attribute called @p name is present, a new attribute is created
         * accordingly. Throws an exception if type is not known.
         * @param type the generic type name of the attribute
         * @param name the name of the attribute
         * @param value the value encoded in a string
         * @return whether setting or creation of the attribute was successful
         */
        bool setAttribute(const std::string &type, const std::string &name, const std::string &value); //throw (std::runtime_error);
        /**
         * Returns the value of an attribute encoded in a string.
         * @param name the name of the attribute
         * @return the attribute's value encoded in a string
         */
        const std::string getAttributeValueString(const std::string &name) const throw (std::invalid_argument);

        //@}

        /**
         * Registers all types that can be used within the OpenTracker library. Every type is
         * registered with a generic typename, which is arbitrary but must be unique. In this
         * function, the generic type names are matched to the according creator functions,
         * which are created automatically.
         *
         * If a new type needs to be added to the library permanently, a new line should be added to
         * this function. Every registered type must provide a standard constructor and streaming
         * operators, which can be defined in 'core/iostream_ext.h'.
         */
        static void registerAllKnownTypes();
        /**
         * Checks whether a generic type name is known by the system.
         * @param typeName the generic type name to be checked
         * @return whether the type name is known by the system
         */
        static bool knowsType(const std::string typeName);
        /**
         * Registers a new generic type name. Type names are arbitrary but must be unique. In this
         * function, the generic type name is matched to the according creator function, which is
         * created automatically.
         *
         * Every registered type must provide a standard constructor and streaming operators, which
         * can be defined in 'core/iostream_ext.h'.
         * @param dummy a dummy pointer to deduce the template parameter (use like (int*)NULL, just added for VC6 support)
         * @param genericTypeName the generic type name to be registered (must be unique!)
         */

       /**
         * Converts Event into a CORBA sequence of string-any structs, which is the CORBA definition of the OpenTracker
         * event type
         *
         * @return the CORBA Event sequence
         */
#ifdef USE_CORBA
        OT_CORBA::Event getCORBAEvent();
#endif

        template <typename T>
            static void registerGenericTypeName(const T *dummy, const std::string &genericTypeName)
        {
            EventAttributeBase::registerType(genericTypeName, typeid(T), EventAttribute<T>::create);
        };
        /**
         * Returns a reference to the value of the attribute called @p name. Throws an exception
         * if such attribute is not present.
         * @param dummy a dummy pointer to deduce the template parameter (use like (int*)NULL, just added for VC6 support)
         * @param name the name of the attribute
         * @return reference to the attribute's value
         */
        template <typename T>
            T& getAttribute(const T *dummy, const std::string &name) //throw (std::invalid_argument)
        {
            AttributeMap::const_iterator it = attributes.find(name);
            if (it == attributes.end())
            {
                std::string errorStr = "event does not have attribute called '" + name + "'";
                throw std::invalid_argument(errorStr);
            }
            else
            {
                EventAttribute<T> *att = dynamic_cast<EventAttribute<T>*>((*it).second);
                if (att)
                    return att->get();
                else
                {
                    std::string errorStr = "attribute called '" + name + "' is not of type '" + typeid(T).name() + "'";
                    throw std::invalid_argument(errorStr);
                }
            }
        };
        /**
         * Returns a reference to the value of the attribute called @p name. Throws an exception
         * if such attribute is not present.
         * @param dummy a dummy pointer to deduce the template parameter (use like (int*)NULL, just added for VC6 support)
         * @param name the name of the attribute
         * @return reference to the attribute's value
         */
        template <typename T>
            const T& getAttribute(const T *dummy, const std::string &name) const throw (std::invalid_argument)
        {
            return (const_cast<Event *>(this))->getAttribute(dummy, name);
        };
        /**
         * Returns a reference to the value of the attribute called @p name. If no such attribute is
         * present, a new attribute is created according to @p defValue. Throws an exception if the
         * type is not known or if an attribute with the same name but of a different type is present.
         * @param dummy a dummy pointer to deduce the template parameter (use like (int*)NULL, just added for VC6 support)
         * @param name the name of the attribute
         * @return reference to the attribute's value
         */
        template <typename T>
            T& getAttribute(const std::string &name, const T &defValue) //throw (std::invalid_argument, std::runtime_error)
        {
            try
            {
                return getAttribute((T*)NULL, name);
            }
            catch (std::invalid_argument)
            {
                addAttribute(name, defValue); // may throw std::runtime_error
                return getAttribute((T*)NULL, name); // may throw std::invalid_argument
            }
        };
        /**
         * Adds a new attribute. Throws an exception if type is not known.
         * @param name the name of the new attribute
         * @param value the value of the new attribute
         * @return whether creation of attribute was successful
         */
        template <typename T>
            bool addAttribute(const std::string &name, const T &value) //throw (std::runtime_error)
        {
            if (hasAttribute(name))
                return false;
            else
            {
                EventAttributeBase *att = EventAttributeBase::create(typeid(T)); // may throw std::runtime_error
                attributes[name] = att;
                return setAttribute(name, value);
            }
        };
        /**
         * Sets the value of an attribute. If currently no attribute called @p name is present, a
         * new attribute is created accordingly. Throws an exception if type is not known.
         * @param name the name of the attribute
         * @param value the new value of the attribute
         * @return whether setting or creation of the attribute was successful
         */
        template <typename T>
            bool setAttribute(const std::string &name, const T &value) //throw (std::runtime_error)
        {
            if (!hasAttribute(name))
                attributes[name] = EventAttributeBase::create(typeid(T)); // may throw std::runtime_error
            EventAttribute<T> *att = dynamic_cast<EventAttribute<T>*>(attributes[name]);
            if (att)
                att->set(value);
            return (att != NULL);
        };

    public:
        /// timestamp of the state in milliseconds since 1/1/1970
        double time;
        /// typesafe null reference, to be used instead of a NULL pointer
        static Event null;
    private:
        /// map holding the event's attributes
        AttributeMap attributes;
    };

    /**
     * Input streaming operator. Streams the data provided by the input stream into the event.
     * @param in the input stream
     * @param event the event
     * @return the input stream
     */
    OPENTRACKER_API std::istream& operator>>(std::istream &in, ot::Event &event);
    /**
     * Output streaming operator. Streams the event into the output stream.
     * @param out the output stream
     * @param event the event
     * @return the output stream
     */
    OPENTRACKER_API std::ostream& operator<<(std::ostream &out, const ot::Event &event);

} // namespace ot

#endif

/*
 * ------------------------------------------------------------
 *   End of Event.h
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

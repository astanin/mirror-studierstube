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
/** read OpenTracker files produced by the FileSink node from MatLab
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include "mex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>
#include <list>

namespace ot
{
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& object)
    {
        typename std::vector<T>::const_iterator it;

        os << "[" << object.size() << ":";
        for (it = object.begin(); it != object.end(); it++)
        {
            if (it != object.begin())
                os << ";";
            os << *it;
        }
        os << "]";
        return os;
    }

    /**
     * Output streaming operator. Streams the list into the output stream.
     * @param os the output stream
     * @param object the list
     * @return the output stream
     */
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::list<T>& object)
    {
        typename std::list<T>::const_iterator it;

        os << object.size() << ":";
        for (it = object.begin(); it != object.end(); it++)
        {
            if (it != object.begin())
                os << ";";
            os << *it;
        }
        os << ".";
        return os;
    };
    /**
     * Input streaming operator. Streams the data provided by the input stream into the vector.
     * @param is the input stream
     * @param object the vector
     * @return the input stream
     */
    template <typename T>
    std::istream& operator>>(std::istream& is, std::vector<T>& object)
    {
        typename std::vector<T>::size_type n;
        typename std::vector<T>::size_type i;
        char c;

        object.clear();

        // read "["
        if (!(is >> c))
            return is;
        if (c != '[')
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        // read size
        if (!(is >> n))
            return is;
        object.reserve(n);

        // read ":"
        if (!(is >> c))
            return is;
        if (c != ':')
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        // read data
        for (i = 0; i < n; i++)
        {
            T t;
            is >> t;
            object.push_back(t);
            // read ";" or "]"
            if (!(is >> c))
                return is;
            if (c != ';' && c != ']')
            {
                is.setstate(std::ios_base::failbit);
                return is;
            }
        }

        return is;
    };
    /**
     * Input streaming operator. Streams the data provided by the input stream into the list.
     * @param is the input stream
     * @param object the list
     * @return the input stream
     */
    template <typename T>
    std::istream& operator>>(std::istream& is, std::list<T>& object)
    {
        typename std::list<T>::size_type n;
        typename std::list<T>::size_type i;
        char c;

        object.clear();

        // read size
        if (!(is >> n))
            return is;

        // read ":"
        if (!(is >> c))
            return is;
        if (c != ':')
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        // read data
        for (i = 0; i < n; i++)
        {
            T t;
            is >> t;
            object.push_back(t);
            // read ";"
            if (!(is >> c))
                return is;
            if (c != ';' && c!= '.')
            {
                is.setstate(std::ios_base::failbit);
                return is;
            }
        }

        return is;
    };

    class EventAttributeBase;
    typedef std::map<std::string, std::string> TranslationMap;
    typedef std::map<std::string, EventAttributeBase*> AttributeMap;
    typedef EventAttributeBase * (*CreateFunction)(void);
    typedef std::map<std::string, CreateFunction> CreatorMap;
    class Translator
    {
    public:
        /**
         * Registers a combination of a generic type name and the type name provided by the compiler
         * for the RunTime Type Information (RTTI) system. The RTTI name is automatically generated
         * according to the @p typeInfo parameter.
         * @param genericName the generic type name to be registered (must be unique!)
         * @param typeInfo RTTI type information of the type to be registered
         */
        void registerTypeName(const std::string &genericName, const std::type_info &typeInfo);
        /**
         * Translates @p rttiName to the according generic type name. Throws an exception if the
         * type is not known by the system.
         * @param rttiName the RTTI name to be translated
         * @return the according generic type name
         */
        const std::string getGenericName(const std::string &rttiName) const throw (std::runtime_error);
        /**
         * Checks whether the generic type name @p genericName is known by the system.
         * @param genericName the generic type name to be checked
         * @return whether the generic type name is known
         */
        bool knowsType(const std::string genericName) const;

    protected:
        /**
         * A map matching compiler dependent type names provided by the RunTime Type Information
         * (RTTI) system to generic type names.
         */
        TranslationMap translationTable;
    };

    class EventAttributeBase
    {
        friend class Event;
        friend std::istream& operator>>(std::istream &in, ot::EventAttributeBase &att);
        friend std::ostream& operator<<(std::ostream &out, ot::EventAttributeBase &att);

    protected:
        /**
         * Virtual destructor. Must provide empty implementation for creation of RTTI type
         * information.
         */
        virtual  ~EventAttributeBase();
        /**
         * Abstract assignment operator.
         * @param rv the right-value, which is the attribute to copy from
         * @return the left value
         */
        virtual EventAttributeBase& operator=(const EventAttributeBase &rv) = 0;
        /**
         * Abstract function to serialize the attribute.
         * @param out the output stream
         */
        virtual void serialize(std::ostream &out) = 0;
        /**
         * Abstract function to de-serialize the attribute.
         * @param in the input stream
         */
        virtual void deserialize(std::istream &in) = 0;
        /**
         * Abstract function to get RTTI type information of the attribute's value.
         * @return RTTI type information
         */
        virtual const std::type_info& getType() const = 0;
        /**
         * Abstract function to get generic type name of the attribute's value.
         * @return generic type name
         */
        virtual const std::string& getGenericTypeName() const = 0;
        /**
         * Creates an EventAttribute according to @p genericTypeName. Throws an exception if this
         * type name is not known by the system.
         * @param genericTypeName generic type name
         * @return pointer to the newly created attribute
         */
        static  EventAttributeBase* create(const std::string &genericTypeName); //throw (std::runtime_error);
        /**
         * Creates an EventAttribute according to @p typeInfo. Throws an exception if this type
         * is not known by the system.
         * @param typeInfo RTTI type info
         * @return pointer to the newly created attribute
         */
        static  EventAttributeBase* create(const std::type_info &typeInfo); //throw (std::runtime_error);
        /**
         * Registers a new generic type name. Type names are arbitrary but must be unique. In this
         * function, the generic type name is matched to the according creator function and the
         * type name is registered in the translator.
         * @param genericTypeName the generic type name to be registered (must be unique!)
         * @param typeInfo RTTI type information of the new type
         * @param create creator function for attributes of the new type
         * @return
         */
        static  void registerType(const std::string &genericTypeName, const std::type_info &typeInfo, CreateFunction create);
        /**
         * Static map matching generic type names to according creator functions.
         * @param
         * @return
         */
        static  CreatorMap creators;
        /**
         * Static Translator to translate compiler dependent RTTI names to generic type names. Such
         * translations are used to create attributes by RTTI type information.
         */
        static  Translator translator;
    };

    /**
     * Input streaming operator. Streams the data provided by the input stream into the attribute.
     * @param in the input stream
     * @param att the attribute
     * @return the input stream
     */
    std::istream& operator>>(std::istream &in, ot::EventAttributeBase &att);
    /**
     * Output streaming operator. Streams the attribute into the output stream.
     * @param out the output stream
     * @param att the attribute
     * @return the output stream
     */
    std::ostream& operator<<(std::ostream &out, ot::EventAttributeBase &att);


    template <class T>
    class EventAttribute : public EventAttributeBase
    {
        friend class Event;

    private:
        /**
         * Standard constructor. Sets the generic type name.
         */
        EventAttribute()
        {
            genericTypeName = translator.getGenericName(typeid(T).name());
        };
        /**
         * Constructor setting the initial value.
         * @param initValue the initial value of the attribute
         */
        EventAttribute(const T &initValue)
            : value(initValue)
        {
            genericTypeName = translator.getGenericName(typeid(T).name());
        };
        /**
         * Copy constructor. Copies the value of another attribute into the new one.
         * @param rv the right-value, which is the attribute to copy from
         */
        EventAttribute(const EventAttribute &rv)
        {
            *this = rv;
        };
        /**
         * Static creator function, which simply returns a newly constructed EventAttribute. For
         * every template instantiation of the EventAttribute class, this creator function is added
         * to the CreatorMap of the EventAttributeBase class, which maps the generic type name of the
         * template paramter to an according creator function pointer. Thus, an EventAttribute can
         * be created with the right template paramter by just knowing the generic type name.
         * @return pointer to the newly created EventAttribute
         */
        static EventAttributeBase* create()
        {
            return new EventAttribute<T>;
        };
        /**
         * Assignment operator. Copies the value of another attribute into this one. If the right
         * value is of a different template type than the left value, the left value will not be
         * changed.
         * @param rv the right-value, which is the attribute to copy from
         * @return the left-value
         */
        EventAttributeBase& operator=(const EventAttributeBase &rv)
        {
            const EventAttribute<T> *trueRv = dynamic_cast< const EventAttribute<T>* >(&rv);
            if (trueRv)
                value = trueRv->value;
            return *this;
        };
        /**
         * Serializes the attribute's value into an output stream.
         * @param out the output stream
         */
        void serialize(std::ostream &out)
        {
            out << value;
        };
        /**
         * De-serializes a value from an input stream. Naturally, the data coming from the input
         * stream must be a correctly serialized value of the same or a similar type.
         * @param in the input stream
         */
        void deserialize(std::istream &in)
        {
            in >> value;
        };
        /**
         * Returns the attribute's value.
         * @return a reference to the attribute's value
         */
        T& get()
        {
            return value;
        };
        /**
         * Returns the attribute's value.
         * @param a const reference to the attribute's value
         * @return
         */
        const T& get() const
        {
            return value;
        };
        /**
         * Sets the attribute's value.
         * @param newValue the new value
         */
        void set(const T &newValue)
        {
            value = newValue;
        };
        /**
         * Returns the RTTI type information of the attribute's value.
         * @return RTTI type information
         */
        const std::type_info& getType() const
        {
            return typeid(T);
        };
        /**
         * Returns the generic type name of the attribute's value.
         * @return the generic type name
         */
        const std::string& getGenericTypeName() const
        {
            return genericTypeName;
        };

    private:
        /// the value stored by the attribute
        T value;
        /// the generic type name of the attribute's value
        std::string genericTypeName;
    };


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
    class  Event
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
    std::istream& operator>>(std::istream &in, ot::Event &event);
    /**
     * Output streaming operator. Streams the event into the output stream.
     * @param out the output stream
     * @param event the event
     * @return the output stream
     */
    std::ostream& operator<<(std::ostream &out, const ot::Event &event);


    Event Event::null;

    Event::Event()
    {
        timeStamp();
    }

    Event::Event(const Event &rv)
    {
        *this = rv;
    }

    Event::~Event()
    {
        clearAttributes();
    }
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
	printf("%s \n",getPrintOut().c_str());
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
        //time = OSUtils::currentTime();
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
    CreatorMap EventAttributeBase::creators;
    Translator EventAttributeBase::translator;

    const std::string Translator::getGenericName(const std::string &rttiName) const throw (std::runtime_error)
    {
        TranslationMap::const_iterator it = translationTable.find(rttiName);
		
        if (it == translationTable.end()){	      
            throw std::runtime_error("unregistered type '" + rttiName + "' - please register new types using Event::registerGenericTypeName<>() to fix this problem");
        }else
            return (*it).second;
    }

    // register new typename to translation table
    void Translator::registerTypeName(const std::string &genericName, const std::type_info &typeInfo)
    {
        translationTable[std::string(typeInfo.name())] = genericName;
    }

    // check whether generic type name was registered
    bool Translator::knowsType(const std::string genericName) const
    {
        TranslationMap::const_iterator it;
        for (it = translationTable.begin(); it != translationTable.end(); it++)
            if ((*it).second == genericName)
                return true;
        return false;
    }

    // virtual destructor
    EventAttributeBase::~EventAttributeBase()
    {
    }

    // create method
    EventAttributeBase* EventAttributeBase::create(const std::string &genericTypeName) //throw (std::runtime_error)
    {
        CreatorMap::iterator it = creators.find(genericTypeName);
        if (it != creators.end())
            return creators[genericTypeName]();
        else
            throw std::runtime_error("Unregistered type '" + genericTypeName + "' - please register new types using Event::registerGenericTypeName<>() or Event::registerAllKnownTypes() to fix this problem.");
    }

    // create method
    EventAttributeBase* EventAttributeBase::create(const std::type_info &typeInfo) //throw (std::runtime_error)
    {
        const std::string genericTypeName = translator.getGenericName(typeInfo.name());
        return create(genericTypeName);
    }

    // register create function and generic name of new type
    void EventAttributeBase::registerType(const std::string &genericTypeName, const std::type_info &typeInfo, CreateFunction create)
    {
        creators[genericTypeName] = create;
        translator.registerTypeName(genericTypeName, typeInfo);
    };

    // stream the attribute (not a member function)
    std::istream& operator>>(std::istream &in, ot::EventAttributeBase &att)
    {
        att.deserialize(in);
        return in;
    }

    // stream the attribute (not a member function)
    std::ostream& operator<<(std::ostream &out, ot::EventAttributeBase &att)
    {
        att.serialize(out);
        return out;
    }
} // namespace ot


void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray*prhs[] )
     
{
    using namespace std;
    using namespace ot;

    if (nrhs !=1)
        mexErrMsgTxt("Must have one input argument, the input file name");
    if (nlhs !=1)
        mexErrMsgTxt("Must have one output argument");
    
    if (! mxIsChar(prhs[0]))
    {
        mexErrMsgTxt("Argument must be a string");
    }

    char *filename = mxArrayToString(prhs[0]);
    //mexPrintf("%s", filename);

    Event::registerAllKnownTypes();

    ifstream inputf(filename);
    if (!inputf.is_open())
    {
        mexErrMsgTxt("file is not open");
    }
    inputf.setf( std::ios::skipws);


    map<string, int> fieldmap;
    fieldmap["station"] = 0;
    fieldmap["timestamp"] = 1;
    int fieldcount = 2;

    Event event;
    int station = -1;
    int count = 0;
    int *stationdata;

    // read once to determine structure

    while (!inputf.eof() && !inputf.bad() && !inputf.fail())
    {

        //inputf.clear();
        inputf >> station;
        if (inputf.fail()) 
        {
            //mexPrintf("failed reading station\n");       
            break;
        }
        inputf >> event;
        if (inputf.fail()) 
        {
            //mexPrintf("failed reading event\n");       
            break;
        }
        
        count++;


        //plhs[0] = station;
        //mexPrintf("%d %d\n", station, event.getSize());

        int i;
        string attname, atttype;
        for (i=0; i<event.getSize(); i++)
        {
            attname = event.getAttributeName(i);
            atttype = event.getAttributeTypeName(attname);
            if (fieldmap.find(attname) == fieldmap.end())
            {
                fieldmap[attname] =  fieldcount++;
            }

            //mexPrintf("%s (%s) -> %s\n", attname.c_str(), atttype.c_str(), //event.getAttributeValueString(attname).c_str());
        }
        
    }

    int odims[2];
    odims[0] = 1;
    odims[1] = count;

    char **fieldnames = new  char*[fieldcount];
    map<string, int>::const_iterator it;
    for (it=fieldmap.begin(); it!=fieldmap.end(); it++)
    {
        int fnsize = it->first.size();
        fieldnames[it->second] = new char[fnsize+1];
        memcpy(fieldnames[it->second], it->first.c_str(), fnsize);
        fieldnames[it->second][fnsize] = 0x0;
    }

    plhs[0] = mxCreateStructArray(2, odims, fieldcount, (const char**)fieldnames);

    mexPrintf("%d events read\n", count);

    // now do the actual reading ...
    count =0;

    inputf.close();
    ifstream xinputf(filename);
    //inputf.seekg(0, ios::beg);
    while (!xinputf.eof() && !xinputf.bad() && !xinputf.fail())
    {
        //mexPrintf("in loop ...");
        
        //inputf.clear();
        xinputf >> station;
        if (xinputf.fail()) 
        {
            //mexPrintf("failed reading station\n");       
            break;
        }
        xinputf >> event;
        if (xinputf.fail()) 
        {
            //mexPrintf("failed reading event\n");       
            break;
        }
        //mexPrintf("%d %d\n", station, event.getSize());
        
        mxArray *record_value;
        
        record_value = mxCreateDoubleMatrix(1,1,mxREAL);
        *mxGetPr(record_value) = station;
        mxSetFieldByNumber(plhs[0], count, 0, record_value);

        record_value = mxCreateDoubleMatrix(1,1,mxREAL);
        *mxGetPr(record_value) = event.time;
        mxSetFieldByNumber(plhs[0], count, 1, record_value);

        
        int i;
        string attname, atttype;
        for (i=0; i<event.getSize(); i++)
        {
            attname = event.getAttributeName(i);
            atttype = event.getAttributeTypeName(attname);   
            
            //mexPrintf("%s (%s) -> %s\n", attname.c_str(), atttype.c_str(), event.getAttributeValueString(attname).c_str());
            
            if (attname == "button")
            {
                record_value = mxCreateDoubleMatrix(1,1,mxREAL);
                *mxGetPr(record_value) = event.getButton();
                mxSetFieldByNumber(plhs[0], count, fieldmap[attname], 
                                   record_value);
            }
            else if (attname == "confidence")
            {
                record_value = mxCreateDoubleMatrix(1,1,mxREAL);
                *mxGetPr(record_value) = event.getConfidence();
                mxSetFieldByNumber(plhs[0], count, fieldmap[attname], 
                                   record_value);                
            }
            else if (attname == "position")
            {
                record_value = mxCreateDoubleMatrix(1,3,mxREAL);
                mxGetPr(record_value)[0] = event.getPosition()[0];
                mxGetPr(record_value)[1] = event.getPosition()[1];
                mxGetPr(record_value)[2] = event.getPosition()[2];
                mxSetFieldByNumber(plhs[0], count, fieldmap[attname], 
                                   record_value);                
            }
            else if (attname == "orientation")
            {
                record_value = mxCreateDoubleMatrix(1,4,mxREAL);
                mxGetPr(record_value)[0] = event.getPosition()[0];
                mxGetPr(record_value)[1] = event.getPosition()[1];
                mxGetPr(record_value)[2] = event.getPosition()[2];
                mxGetPr(record_value)[3] = event.getPosition()[3];
                mxSetFieldByNumber(plhs[0], count, fieldmap[attname], 
                                   record_value);                
            }
            else if (atttype == "unsigned_short" || atttype == "short" || atttype == "unsigned_int" || atttype == "int" )
            {
                record_value = mxCreateDoubleMatrix(1,1,mxREAL);
                *mxGetPr(record_value) = atoi(event.getAttributeValueString(attname).c_str());
                //mexPrintf("attribute %s : %s : %s \n", 
                //          attname.c_str(), atttype.c_str(),event.getAttributeValueString(attname).c_str());
               mxSetFieldByNumber(plhs[0], count, fieldmap[attname],
                                   record_value);
            }
            else if ( atttype == "float" || atttype == "double")
            {
                record_value = mxCreateDoubleMatrix(1,1,mxREAL);
                *mxGetPr(record_value) = atof(event.getAttributeValueString(attname).c_str());
                mxSetFieldByNumber(plhs[0], count, fieldmap[attname], 
                                   record_value);
            }
            else 
            {
                mexPrintf("unknown attribute type for %s : %s", 
                          attname.c_str(), atttype.c_str());

            }
            //mexPrintf("%s (%s) -> %s\n", attname.c_str(), atttype.c_str(), event.getAttributeValueString(attname).c_str());            
        }
        count++;
    }

    //mexPrintf("%d events read\n", count);
    xinputf.close();
   
    int i;
    for (i=0;i<fieldcount; i++)
    {
        delete [] fieldnames[i];
    }
    delete [] fieldnames;

    mxFree(filename);

    return;
}

/*
 * ------------------------------------------------------------
 *   End of readOpenTracker.cxx
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

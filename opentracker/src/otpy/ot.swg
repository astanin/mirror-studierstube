%module ot
%{
#include <OpenTracker/core/Event.h>
#include <OpenTracker/core/Node.h>
#include <OpenTracker/core/NodePort.h>

namespace ot
{
    class PythonNode;
}
%}

%include "std_string.i"
%include "std_vector.i"
%nodefaultctor;
%nodefaultdtor;

namespace std
{
	%template(floatvector) vector<float>;
}

namespace ot
{
	class Event
	{
	public:
		Event();

		Event(const Event &);

		~Event();

		bool hasAttribute(const std::string &name) const;
	    
		bool delAttribute(const std::string &name);

		bool renAttribute(const std::string &oldName, const std::string &newName);

		void copyAllButStdAttr(const Event &rv);

		void clearAttributes();

		void printout() const;

		const std::string getPrintOut() const;

		void timeStamp();

		int getSize() const;

		const std::string& getAttributeTypeName(const std::string &name) const throw (std::invalid_argument);

		const std::string& getAttributeName(const int index) const throw (std::invalid_argument);

		int getAttributeIndex(const std::string &name) const throw (std::invalid_argument);

		const std::vector<float>& getPosition() const;
	    
		const std::vector<float>& getOrientation() const;
	    
		const float& getConfidence() const;

		const unsigned short& getButton() const;
	   
		void setPosition(const std::vector<float> &value);

		void setOrientation(const std::vector<float> &value);

		void setConfidence(const float &value);

		void setButton(const unsigned short &value);

		bool addAttribute(const std::string &type, const std::string &name, const std::string &value);

		bool setAttribute(const std::string &type, const std::string &name, const std::string &value);

		const std::string getAttributeValueString(const std::string &name) const throw (std::invalid_argument);

		static bool knowsType(const std::string typeName);

		double time;
	    
	};

	class NodePort;
	
	class Node
	{
	public:
        virtual ~Node();

        const std::string & getType() const;

        const std::string & getName() const;

        Node * getParent();
        
        Node * getParent( unsigned int index );

        unsigned int countParents();

        unsigned int countChildren();

        Node * getChild( unsigned int index );

        unsigned int countPorts();

        NodePort * getPort( const std::string & name, unsigned int index = 0 );

        NodePort * getPort( unsigned int index );

        virtual int isEventGenerator();

        void updateObservers( Event &data );

        virtual int isEventQueue();

        virtual Event& getEvent(unsigned int index = 0);

        virtual Event& getEventNearTime(double time);

        virtual unsigned int getSize();

        virtual int isTimeDependend();

        virtual Event& getEventAtTime(double time);

        virtual int isNodePort();
        
        unsigned int countAllChildren();
        
        Node * getAnyChild( unsigned int index );
	};

    class NodePort: public Node
    {
    };
    
    class PythonNode: public Node
    {
    public:
        %extend
        {
            PythonNode(PythonNode* pPythonNode)
            {
                 return pPythonNode;
            }
        }
    };
}

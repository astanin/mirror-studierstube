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
/** source file for class ConfigurationParser.
 *
 * @author Gerhard Reitmayr
 * @author Eduardo Veas
 *
 * $Id: ConfigurationParser.cxx 1739 2007-01-23 16:57:46Z veas $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/Log_Msg.h>
#include <ace/Env_Value_T.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <OpenTracker/core/ConfigurationParser.h>
#include <OpenTracker/core/Graph.h>

// selects between usage of XERCES and TinyXML
#include <OpenTracker/tool/XMLSelection.h>


#ifdef USE_XERCES
XERCES_CPP_NAMESPACE_USE
#include <xercesc/sax/InputSource.hpp>


#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLEntityResolver.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>



#include <OpenTracker/core/DOMTreeErrorReporter.h>

#include <iostream>

//using namespace std;

XERCES_CPP_NAMESPACE_USE

namespace ot {

    class XercesConfigurationParser: public ConfigurationParser
    {
        // Members
    protected:
        /// reference to the Context to use its modules and factories
        Context & context;
        /// maps IDs to nodes
        NodeMap references;
        /// stores the parsed document tree
        OT_DOMDOCUMENT * document;
    protected:
        
        // Methods
        /**
         * builds a tree of configuration nodes. This is just mirroring
         * the structure of the XML document with ConfigNodes that 
         * just hold the element names and attributes. The resulting tree
         * is a parameter to the initialization method of modules.
         * @param elements pointer to the XML element to be parsed.
         * @return pointer to the hew ConfigNode or NULL, if something
         *         went wrong.
         */
        ConfigNode * buildConfigTree( OT_DOMELEMENT * element , Graph * graph);

        /**
         * builds the tracker tree starting from a certain DOM_Element.
         * Is used recoursively to walk the DOMTree and create new nodes.
         * @param element pointer to the element to be parsed.
         * @return pointer to the new Node or NULL
         */
        Node * buildTree( OT_DOMELEMENT * element, Graph * graph);
        /**
         * parses an Elements attributes and returns a StringTable describing them.
         * @param element pointer to the element
         * @return a StringMap mapping attribute names to attribute values. This map
         *    was created on the heap and it is now owned by the caller.
         */
        StringTable * parseElement( OT_DOMELEMENT * element);

    public:
        /**
         * constructor method. The Xerces XML parser library is initialized here.
         * @param factory_ pointer to set the member factory
         */
        XercesConfigurationParser( Context & context_);
        /** Destructor method.*/
        virtual ~XercesConfigurationParser();
        /**
         * These methods parses an XML configuration file. It returns a Node as
         * root with all the root nodes defined in the configuration file.
         * @param filename the name and path of the configuration file
         * @return pointer to the root Node or NULL
         */
        virtual Graph * parseConfigurationFile(const std::string& filename);
        virtual Graph * parseConfigurationString(const char* xmlstring);

	Graph * parseConfiguration(InputSource* input_source);

    };


    ConfigurationParser * getConfigurationParser(Context & context){
        return new XercesConfigurationParser(context);
    };


    const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull };
    static const char*  otMemBufId = "otgraph";

    class OpenTrackerResolver : public XMLEntityResolver {
    protected:
        Context * context;

    public:
        OpenTrackerResolver ( Context * context_ ) : context(context_)
        {};

        virtual InputSource * resolveEntity( XMLResourceIdentifier * resourceIdentifier );
    };

    InputSource * OpenTrackerResolver::resolveEntity( XMLResourceIdentifier * resourceIdentifier )
    {
        if (resourceIdentifier->getResourceIdentifierType() == XMLResourceIdentifier::ExternalEntity)
        {
            XMLCh * test = XMLString::transcode("opentracker.dtd");
            if(XMLString::endsWith(resourceIdentifier->getSystemId(), test))
            {
                ACE_Env_Value<std::string> otroot(ACE_TEXT("OTROOT"), "");
                std::string otrootvalue = (std::string)otroot;
                std::string otdatadir;
                if( otrootvalue.compare("") != 0 )
                {
                    //otdatadir = otrootvalue + "/data";
                    otdatadir = otrootvalue;// + "/share";
                    context->addDirectoryLast(otdatadir);
                }
                char * file = XMLString::transcode(resourceIdentifier->getSystemId());
                std::string filename( file ), fullname;
                XMLString::release( &file );
                bool result = context->findFile( filename, fullname );
                if( otrootvalue.compare("") != 0 )
                {
                    context->removeDirectory(otdatadir);
                }
                if( result == true )
                {
                    XMLCh * file = XMLString::transcode( fullname.c_str());
                    LocalFileInputSource * input = new LocalFileInputSource( file );
                    XMLString::release( &file );
                    XMLString::release( &test );
                    return input;
                }
            }
            XMLString::release( &test );
        }
        return NULL;
    }



    // constructor method

    XercesConfigurationParser::XercesConfigurationParser( Context & context_)
        : context( context_ )
    {

        // Initialize the XercesC system
        try {
            XMLPlatformUtils::Initialize();
        }
        catch (const XMLException& toCatch) {
            char * message = XMLString::transcode( toCatch.getMessage());
            logPrintE("XercesConfigurationParser Error during initialization: %s\n", message );
            XMLString::release( &message );
            exit(1);
        }

    }

    // Destructor method.

    XercesConfigurationParser::~XercesConfigurationParser()
    {
        references.clear();

        // Deinitialize the XercesC system
        try {
            XMLPlatformUtils::Terminate();
        }
        catch (const XMLException& toCatch) {
            char * message = XMLString::transcode( toCatch.getMessage());
            logPrintE( "XercesConfigurationParser Error during deinitialization: %s\n", message );
            XMLString::release( &message );
            exit(1);
        }
    }

    // builds a tree of configuration nodes.

    ConfigNode * XercesConfigurationParser::buildConfigTree( OT_DOMELEMENT * element , Graph * graph)
    {

        std::auto_ptr<StringTable> map ( parseElement( element ));
        char * tempName = XMLString::transcode( element->getLocalName());
        std::string tagName = tempName;
        XMLString::release( &tempName );
        ConfigNode * config = new ConfigNode( *map );

        config->put("OtNodeType", tagName);


        //auto_ptr<DOMNodeList> list( element->getChildNodes());
        DOMNodeList * list = element->getChildNodes();
        for( unsigned int i = 0; i < list->getLength(); i ++ )
        {
            if( list->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
            {
                OT_DOMELEMENT * childElement = (OT_DOMELEMENT *)list->item(i);
                ConfigNode * child = buildConfigTree( childElement, graph );
                
                //                config->addChild(*child);
                graph->connectNodes(config, child);

            }
        }
        return config;


    }

    // builds the tracker tree starting from a certain DOM_Element.

    Node * XercesConfigurationParser::buildTree( OT_DOMELEMENT * element, Graph * graph)
    {

        char * tempName = XMLString::transcode( element->getLocalName());
        std::string tagName = tempName;
        XMLString::release( &tempName );
        std::auto_ptr<StringTable> map ( parseElement( element ));
        // Test for a reference node
        logPrintI("Building Tree for node %s\n", tempName);
        map->put("OtNodeType", tagName);
        
        if( tagName.compare("Ref") == 0 )
        {
            NodeMap::iterator find = references.find(map->get("USE"));
            if( find != references.end()){

                Node * ref =  (Node*) ((*find).second);

                logPrintI("Found Reference node -> %s\n", map->get("USE").c_str());
                return ref;
            } else
            {
                logPrintE("Undefined reference %s\n", map->get("USE").c_str());
                return NULL;
            }
        }

        logPrintI("Calling the context create node\n");
        Node * value = context.createNode( tagName , *map );

        // if the value is NULL, it might be the case that the Module has not yet being loaded,
        // a trick to force the context to load this module, is to ask for it.
        if (value == NULL) {
            logPrintI("Didn't work, try loading and then creating\n");
            //try loading the module, and then creating the node
            if (context.getModuleFromNodeType(tagName) != NULL)
            {
                // logPrintW("Context found module from %s\n", tagName.c_str());
                // try creating the node again

                value = context.createNode( tagName , *map );
                
                
            }
        }
        if( value != NULL )
        {
      
            // Test for ID
            if( map->containsKey("DEF"))
            {
                references[map->get("DEF")] = value;
                value->setName ( map->get("DEF"));
                logPrintI("Storing Reference %s\n", (map->get("DEF").c_str()));
            }

            //auto_ptr<DOMNodeList> list ( element->getChildNodes());
            DOMNodeList * list = element->getChildNodes();
            for( unsigned int i = 0; i < list->getLength(); i ++ )
            {
                if( list->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
                {

                    OT_DOMELEMENT * childElement = (OT_DOMELEMENT *)list->item(i);
                    ///FIXXXME: what was the assignment for? 

                    Node * childNode = buildTree( childElement, graph );
                    if (childNode)
                    {
                        //                        value->addChild(*childNode);
                        graph->connectNodes(value, childNode);
                    }
                }
            }
        }
        else
        {
            logPrintW("Could not parse element %s\n", tagName.c_str());
        }
        return value;


    }


    // This method takes an InputSource and configures the OT graph accordingly
    Graph * XercesConfigurationParser::parseConfiguration(InputSource* input_source) 
    {
      
        // read and parse configuration file
        XercesDOMParser * parser = new XercesDOMParser();
        parser->setValidationScheme( XercesDOMParser::Val_Auto );
        parser->setIncludeIgnorableWhitespace( false );
        parser->setDoNamespaces(true);
        parser->setDoSchema(true);

        OpenTrackerResolver resolver( &context);
        parser->setXMLEntityResolver( &resolver );

        DOMTreeErrorReporter errReporter;
        parser->setErrorHandler( &errReporter );

        try
        {
            parser->parse( *input_source );
        }
        catch (const XMLException& e)
        {
            char * message = XMLString::transcode( e.getMessage());
            logPrintE("An error occured during parsing\n   Message: %s\n", message);
            XMLString::release( & message );
            exit(1);
        }
        catch (const DOMException& e)
        {
            char * message = XMLString::transcode( e.msg );
            logPrintE("An error occured during parsing\n   Message: %s\n", message);
            XMLString::release( & message );
            exit(1);
        }
        if( errReporter.errorsEncountered() > 0 )
        {
            logPrintE("There were non fatal errors in the configuration file !\n");
            logPrintE("Please check the file and start again.\n");
            exit(1);
        }

        DOMDocument * doc = parser->getDocument();
        DOMElement * root = doc->getDocumentElement();
        //        Node * node = new Node();
        Graph * graph = new Graph();

        char * tempName = XMLString::transcode( root->getLocalName());
        logPrintI("Root node is %s\n", tempName);
        XMLString::release( & tempName );

        const XMLCh* xmlspace = root->getNamespaceURI();
        if (xmlspace != NULL) {
            char * tempSpace = XMLString::transcode( xmlspace );
            logPrintI("Namespace is %s\n", tempSpace);
            XMLString::release( & tempSpace );
        }
        else {
            logPrintW("Not using namespaces!\n");
        }


        // get the configuration part
        XMLCh * configurationCh = XMLString::transcode( "configuration" );
        //auto_ptr<DOMNodeList> list( root->getElementsByTagName( configurationCh.get() ));
        DOMNodeList * list = root->getElementsByTagNameNS(xmlspace, configurationCh);
        XMLString::release( & configurationCh );
        if( list->getLength() != 1 )
        {
            logPrintE("not valid config file, not exactly one configuration tag\n");
            logPrintI("%d configurations.\n", list->getLength());
            exit(1);
        }

        logPrintI("parsing configuration section\n");

        // parse configuration elements subelements
        DOMElement * config = (DOMElement *)list->item(0);
        //auto_ptr<DOMNodeList> configlist( config->getChildNodes());
        DOMNodeList * configlist = config->getChildNodes();
        unsigned int i;

        Node * configNode = new Node();
        //        configNode->setGraph(graph);
        configNode->put("OtNodeType", "configuration");



        for( i = 0; i < configlist->getLength(); i ++ )
        {

            if( configlist->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
            {
                DOMElement * configElement = (DOMElement *)configlist->item(i);
                std::auto_ptr<StringTable> attributes( parseElement( configElement ));
                char * tempName = XMLString::transcode( configElement->getLocalName());
                std::string tagName = tempName;
                XMLString::release( &tempName );
                ConfigNode * base = new ConfigNode( *attributes );

                
                base->put("OtNodeType", tagName);                

                logPrintI("config for %s\n", tagName.c_str());

                //auto_ptr<DOMNodeList> nodelist( configElement->getChildNodes());
                DOMNodeList * nodelist = configElement->getChildNodes();

                unsigned int j;

                //                configNode->addChild(*base);
                graph->connectNodes(configNode, base);
                
                for( j = 0; j < nodelist->getLength(); j++ )
                {
                    if( nodelist->item(j)->getNodeType() == DOMNode::ELEMENT_NODE )
                    {
                        DOMElement * element = (DOMElement *)nodelist->item(j);
                        ///FIXXXME: What was the assignment good for?
                        ConfigNode * child = buildConfigTree( element, graph );
                        //                        base->addChild(*child);
                        graph->connectNodes(base, child);
                    }
                }
                Module * module = context.getModule( tagName );
                if( module != NULL )
                {
                    module->init( *attributes, base );
                }
            }
        }


        logPrintE("Number of config nodes %d\n", (configNode->countAllChildren()));
        if (configNode->countAllChildren() > 0)
        {            
            //            node->addChild(*configNode);
            //            graph->connectNodes(node, configNode);
            graph->addNode(configNode);
        }

        logPrintI("parsing tracker tree section\n");

        // parse the rest of the elements
        //auto_ptr<DOMNodeList> rootlist( root->getChildNodes());
        DOMNodeList * rootlist = root->getChildNodes();
        for( i = 0; i < rootlist->getLength(); i++ )
        {
            if( rootlist->item(i)->getNodeType() != DOMNode::ELEMENT_NODE )   // not an element node !
            {
                continue;
            }
            DOMElement * element = (DOMElement *)rootlist->item(i);
            char * tempTagName = XMLString::transcode(element->getLocalName());
            if( 0 == XMLString::compareString( tempTagName, "configuration" ))    // the configuration element, already handled
            {
                XMLString::release( &tempTagName );
                continue;
            }
            XMLString::release( &tempTagName );
            logPrintI("XERCESCONFIGURATIONPARSER::BUILDTREE\n");
            Node * child = buildTree( element, graph );
            logPrintI("XERCESCONFIGURATIONPARSER::BUILDTREE returned child %p\n", child);
            if (child)
            {
                //                node->addChild(*child);
                graph->addNode( child);
                logPrintI("XERCESCONFIGURATIONPARSER::graph added node\n");
            }

        }

        delete parser;
        //        root->release();




        return graph;
    }

    // parses an Elements attributes and returns a StringMap describing them.

    StringTable * XercesConfigurationParser::parseElement( OT_DOMELEMENT * element)
    {
        StringTable * value = new StringTable;
        // auto_ptr<DOMNamedNodeMap> map( element->getAttributes());   // is it still owned by the library ?
        DOMNamedNodeMap * map = element->getAttributes();
        for( unsigned int i = 0; i < map->getLength(); i++ )
        {
            DOMAttr * attribute = (DOMAttr *)map->item( i );
            char * nameTemp = XMLString::transcode( attribute->getName());
            char * valueTemp = XMLString::transcode( attribute->getValue());
            value->put(nameTemp, valueTemp );
            XMLString::release( &valueTemp );
            XMLString::release( &nameTemp );
        }
        return value;

    }

    // This method parses an XML configuration file.
    Graph * XercesConfigurationParser::parseConfigurationFile(const std::string& filename)
    {

        ACE_Env_Value<std::string> otroot(ACE_TEXT("OTROOT"), "");
        std::string otrootvalue = (std::string)otroot;
        std::string otdatadir;
        if( otrootvalue.compare("") != 0 )
        {
            //otdatadir = otrootvalue + "/data";
            otdatadir = otrootvalue;// + "/share";
            //context->addDirectoryLast(otdatadir);
        }
        LocalFileInputSource input_source((const XMLCh*) XMLString::transcode(filename.c_str()));
        return parseConfiguration(&input_source);
    }

    // This method parses an XML configuration string.

    Graph * XercesConfigurationParser::parseConfigurationString(const char* xmlstring)
    {

		MemBufInputSource* input_source = new MemBufInputSource(
                                                                (const XMLByte*) xmlstring
                                                                , strlen(xmlstring)
                                                                , otMemBufId
                                                                , false
                                                                );
        Graph*  graph= parseConfiguration(input_source);
        delete input_source;
        return graph;
    }


} // namespace ot

#endif //USE_XERCES
/* 
 * ------------------------------------------------------------
 *   End of XercesConfigurationParser.cxx
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

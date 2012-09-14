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
 *
 * $Id: ConfigurationParser.cxx 1739 2007-01-23 16:57:46Z veas $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#ifdef USE_TINYXML

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/Log_Msg.h>
#include <ace/Env_Value_T.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

// selects between usage of XERCES and TinyXML
#include <OpenTracker/tool/XMLSelection.h>

class TiXmlDocument;



#include <OpenTracker/core/ConfigurationParser.h>
#include <OpenTracker/core/Context.h>

#include <OpenTracker/core/DOMTreeErrorReporter.h>
#include <OpenTracker/core/Graph.h>

#include <iostream>

//using namespace std;



namespace ot {


    class Context;

    /// used to map a string to a node
    typedef std::map<std::string, Node *> NodeMap;

    /**
     * parses the XML configuration file. This class reads the configuration file
     * using DOM and builds the tracker tree. It is not part of the public API of
     * OpenTracker, therefore do not use it !
     * @author Gerhard Reitmayr
     * @ingroup core
     */
    class TinyXMLConfigurationParser: public ConfigurationParser
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
        TinyXMLConfigurationParser( Context & context_);
        /** Destructor method.*/
        virtual ~TinyXMLConfigurationParser();
        /**
         * These methods parses an XML configuration file. It returns a Node as
         * root with all the root nodes defined in the configuration file.
         * @param filename the name and path of the configuration file
         * @return pointer to the root Node or NULL
         */
        virtual Graph * parseConfigurationFile(const std::string& filename);
        virtual Graph * parseConfigurationString(const char* xmlstring);

        Graph * parseConfiguration(TiXmlDocument * tinyXMLDocument);

    };

    ConfigurationParser * getConfigurationParser(Context & context){
        return new TinyXMLConfigurationParser(context);
    };

    // constructor method

    TinyXMLConfigurationParser::TinyXMLConfigurationParser( Context & context_)
        : ConfigurationParser(), context( context_ )
    {
    }

    // Destructor method.

    TinyXMLConfigurationParser::~TinyXMLConfigurationParser()
    {
        references.clear();
    }

    // builds a tree of configuration nodes.

    ConfigNode * TinyXMLConfigurationParser::buildConfigTree( OT_DOMELEMENT * element , Graph * graph)
    {


        std::auto_ptr<StringTable> map ( parseElement( element ));
        std::string tagName = element->Value();

        ConfigNode * config = new ConfigNode( *map );

        config->put("OtNodeType", tagName);



        TiXmlElement * el = element->FirstChildElement();
        while( el != NULL )
        {
            ConfigNode * child = buildConfigTree( el, graph);
            graph->connectNodes(config, child);
            //            config->addChild(*child);
            
            el = el->NextSiblingElement();
        }
        return config;

    }

    // builds the tracker tree starting from a certain DOM_Element.

    Node * TinyXMLConfigurationParser::buildTree( OT_DOMELEMENT * element, Graph * graph)
    {

        std::string tagName = element->Value();
        std::auto_ptr<StringTable> map ( parseElement( element ));
        // Test for a reference node
        if( tagName.compare("Ref") == 0 )
        {
            NodeMap::iterator find = references.find(map->get("USE"));
            if( find != references.end()){
                Node * ref = (Node*) ((*find).second );


                logPrintI("Found Reference node -> %s\n", map->get("USE").c_str());
                return ref;
            } else
            {
                logPrintE("Undefined reference %s\n", map->get("USE").c_str());
                return NULL;
            }
        }


        map->put("OtNodeType", tagName);        
        Node * value = context.createNode( tagName , *map );

        if (value == NULL) {
            //try loading the module, and then creating the node
            if (context.getModuleFromNodeType(tagName) != NULL){
                // try creating the node again
                logPrintI("TinyXMLConfigurationParser trying to create node %s again \n", tagName.c_str());

                value = context.createNode(tagName, *map);
            }
        }
        if( value != NULL )
        {
            // Test for ID
            if( map->containsKey("DEF"))
            {
                references[map->get("DEF")] = value;
                value->setName(map->get("DEF"));
                logPrintI("Storing Reference %s\n", map->get("USE").c_str());
            }

            TiXmlElement * el = element->FirstChildElement();
            while( el != NULL )
            {
                Node * childNode = buildTree( el , graph);
                graph->connectNodes(value, childNode);
                //                value->addChild(*childNode);
                el = el->NextSiblingElement();
            }
        }
        else
        {
            logPrintW("Could not parse element %s\n", tagName.c_str());
        }
        return value;

    }


    Graph * TinyXMLConfigurationParser::parseConfiguration(TiXmlDocument* document) 
    {

        TiXmlElement* root = document->RootElement();
        Graph * graph = new Graph();


        /*        const char* tempName = root->Value();

        logPrintI("Root node is %s\n", tempName);
        */

        const char* xmlspace = NULL; //root->getNamespaceURI();
        if (xmlspace != NULL) {
            logPrintI("Namespace is %s\n", xmlspace);
        }
        else {
            logPrintW("Not using namespaces!\n");
        }

        // get the configuration part
        const char* configurationCh = "configuration";

        TiXmlElement * config = root->FirstChildElement(configurationCh);
        if(config->NextSiblingElement(configurationCh))
        {
            logPrintE("not valid config file, not exactly one configuration tag\n");
            exit(1);
        }

        logPrintI("parsing configuration section\n");


        Node * configNode = new Node();
        configNode->put("OtNodeType", "configuration");

        TiXmlElement * configElement = config->FirstChildElement();
        while(configElement)
        {
            std::auto_ptr<StringTable> attributes( parseElement( configElement ));
            std::string tagName = configElement->Value();

            ConfigNode * base = new ConfigNode( *attributes );
            
            base->put("OtNodeType", tagName);

            logPrintI("config for %s\n", tagName.c_str());

            graph->connectNodes(configNode, base);

            TiXmlElement * element = configElement->FirstChildElement();
            while(element)
            {
                ConfigNode * child = buildConfigTree( element , graph);
                graph->connectNodes(base, child);
                //                base->addChild(*child);
                
                element = element->NextSiblingElement();
            }

            Module * module = context.getModule( tagName );
            if( module != NULL )
            {
                module->init( *attributes, base );
            }
            //            graph->connectNodes(configNode, base);
            //            configNode->addChild(*base);



            configElement = configElement->NextSiblingElement();
        }

        
        //        node->addChild(*configNode);



        logPrintI("parsing tracker tree section\n");

        // parse the rest of the elements

        TiXmlElement * element = root->FirstChildElement();
        while(element)
        {
            if(strcmp(element->Value(), "configuration")!=0){
               Node * child= buildTree( element , graph);
               graph->addNode(child);
               //               node->addChild(*child);

            }

            element = element->NextSiblingElement();
        }



        return graph;

    }

    // parses an Elements attributes and returns a StringMap describing them.

    StringTable * TinyXMLConfigurationParser::parseElement( OT_DOMELEMENT * element)
    {



        StringTable * value = new StringTable;

        TiXmlAttribute* attribute = element->FirstAttribute();
        while(attribute)
        {
            value->put(attribute->Name(), attribute->Value());
            attribute = attribute->Next();
        }
        return value;


    }

    // This method parses an XML configuration file.
    Graph * TinyXMLConfigurationParser::parseConfigurationFile(const std::string& filename)
    {

        TiXmlDocument document;// = new TiXmlDocument();

        if(!document.LoadFile(filename.c_str()))
        {
            logPrintE("An error occured during parsing\n   Message: %s\n", document.ErrorDesc());
            exit(1);
        }
		return parseConfiguration(&document);

    }

    // This method parses an XML configuration string.

    Graph * TinyXMLConfigurationParser::parseConfigurationString(const char* xmlstring)
    {
        TiXmlDocument document;// = new TiXmlDocument();

        if(!document.Parse(xmlstring))
        {
            logPrintE("An error occured during parsing\n   Message: %s\n", document.ErrorDesc());
            exit(1);
        }
		return parseConfiguration(&document);

    }


} // namespace ot
#endif // USE_TINYXML
/* 
 * ------------------------------------------------------------
 *   End of TinyXMLConfigurationParser.cxx
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

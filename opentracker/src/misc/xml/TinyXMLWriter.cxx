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
/** source file for XMLWriter class.
 *
 * @author Eduardo E. Veas
 *
 * $Id: XMLWriter.cxx 1739 2007-01-23 16:57:46Z veas $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifndef OT_NO_XMLWRITER_SUPPORT

#  ifdef USE_TINYXML
// selects between usage of XERCES and TinyXML
#include <OpenTracker/tool/XMLSelection.h>



#include <memory>

#include <OpenTracker/misc/xml/XMLWriter.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

//using namespace std;



namespace ot {




    XMLWriter::XMLWriter( Context & context_ , unsigned int indent_ ) :
        context( context_ ), indent( indent_ )
    {};

    XMLWriter::~XMLWriter()
    {
        defnodes.clear();
    };

    char * header = "<?xml version=\"1.0\" standalone=\"no\" >\n<!-- OpenTracker data version 2.0 TinyXMLWriter -->"/*
                                                                                 "<OpenTracker>"*/;
 
    void insertNode(Node * node, TiXmlElement & parent, XMLWriter & writer){
        bool writeSubtree= true;
        std::string defstring (node->get("DEF"));

        if (defstring.compare("")!=0){
            XMLWriter::DefDict::iterator search = writer.defnodes.find(defstring);
            if (search != writer.defnodes.end()){        
                writeSubtree = false;
            } else
                writer.defnodes[defstring]= node;
        }
        
        if (writeSubtree){

            // create the node with the right name
            TiXmlElement theNode((node->getType()) .c_str());
            // add the attributes to the node
            StringTable & attr = node->getAttributes();
            KeyIterator keys(attr);
            while( keys.hasMoreKeys())
            {
                const std::string & key = keys.nextElement();
                theNode.SetAttribute(key.c_str(), attr.get(key) .c_str());
            }
            // add children to the node
            for (unsigned int i = 0; i < node->countAllChildren() ; i++){
                insertNode(node->getAnyChild(i), theNode, writer);
            }
            
            // insert the node into the tree
            parent.InsertEndChild(theNode);
            
        } else {
            // write a ref node
            TiXmlElement theNode("Ref");
            theNode.SetAttribute("USE", defstring.c_str());
            parent.InsertEndChild(theNode);
        }
       
    }
    
    void XMLWriter::write( const char * file )
    {
	TiXmlDocument doc;
        doc.Parse(header);
        if (doc.Error()){
            std::string errorstring = "XMLWriter: ERROR parsing header: ";
            errorstring +=  doc.Value();
            errorstring += doc.ErrorDesc();
            // throw OtException(errorstring);
        }

        TiXmlElement opentracker("OpenTracker");

        Graph * graph = context.getRootNode();
        if (graph != NULL){

            for (unsigned int i = 0; i < graph->countNodes() ; i++){
               Node * someRootNode = graph->getNode(i);
               logPrintI("XMLWriter:: writing node %s parentcount %u\n", (someRootNode->getType().c_str()), (someRootNode->countParents()));
               if (someRootNode->countParents() == 0) {
                   insertNode(graph->getNode(i), opentracker, *this);
               }
            }
        }
        
        doc.InsertEndChild(opentracker);


	doc.SaveFile(file);
    }



}  // namespace ot

# endif //USE_TINYXML

#else
#pragma message(">>> OT_NO_XMLWRITER_SUPPORT")
#endif //OT_NO_XMLWRITER_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of XMLWriter.cxx
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

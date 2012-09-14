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
 * $Id: XMLWriter.cxx 1738 2007-01-23 16:28:37Z veas $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifndef OT_NO_XMLWRITER_SUPPORT

// selects between usage of XERCES and TinyXML
#include <OpenTracker/tool/XMLSelection.h>

#ifdef USE_XERCES
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>


#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <xercesc/util/OutOfMemoryException.hpp>




#include <memory>

#include <OpenTracker/misc/xml/XMLWriter.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

//using namespace std;


XERCES_CPP_NAMESPACE_USE



namespace ot {




    XMLWriter::XMLWriter( Context & context_ , unsigned int indent_ ) :
        context( context_ ), indent( indent_ )
    {};

    XMLWriter::~XMLWriter()
    {
        defnodes.clear();
    };



// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of char* data to XMLCh data.
// ---------------------------------------------------------------------------
class XStr
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XStr(const char* const toTranscode)
    {
        // Call the private transcoding method
        fUnicodeForm = XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
        XMLString::release(&fUnicodeForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fUnicodeForm
    //      This is the Unicode XMLCh format of the string.
    // -----------------------------------------------------------------------
    XMLCh*   fUnicodeForm;
};

#    define X(str) XStr(str).unicodeForm()


    void writeNode(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode * toWrite, XERCES_CPP_NAMESPACE_QUALIFIER XMLFormatTarget * target )
    {
        XMLCh * lsCh =  XMLString::transcode("LS");
        DOMImplementation * impl = DOMImplementationRegistry::getDOMImplementation(lsCh);
        DOMWriter * writer = impl->createDOMWriter();
        if (writer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
            writer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);
        if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
            writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
        try {
            // do the serialization through DOMWriter::writeNode();
            writer->writeNode(target, *toWrite);
        }
        catch (const XMLException& toCatch) {
            char * message = XMLString::transcode(toCatch.getMessage());
            logPrintE("XMLWriter Exception message is: %s\n", message);
            XMLString::release( &message );
        }
        catch (const DOMException& toCatch) {
            char * message = XMLString::transcode(toCatch.msg);
            logPrintE("XMLWriter Exception message is: %s\n", message);
            XMLString::release( &message );
        }
        catch (...) {
            logPrintE("XMLWriter Unexpected Exception \n");
        }
        writer->release();
        XMLString::release( &lsCh );
    }


    
    DOMElement * createNode(DOMDocument * doc, Node * someNode, XMLWriter & writer){
        if ( doc == NULL || someNode ==NULL)
            return NULL;
        bool writeSubtree= true;
        std::string defstring (someNode->get("DEF"));

        if (defstring.compare("")!=0){
            XMLWriter::DefDict::iterator search = writer.defnodes.find(defstring);
            if (search != writer.defnodes.end()){        
                writeSubtree = false;
            } else
                writer.defnodes[defstring]= someNode;
        }
        


        if (writeSubtree){

            DOMElement* theElement = NULL;
            try {
                theElement = doc->createElement(X((someNode->getType()) .c_str()));
                // add all attributes
                StringTable & attr = someNode->getAttributes();
                KeyIterator keys(attr);
                while( keys.hasMoreKeys())
                {
                    
                    const std::string & key = keys.nextElement();
                    XMLCh * tempKey = XMLString::transcode( key.c_str() );
                    XMLCh * tempValue = XMLString::transcode( attr.get( key ) .c_str());
                    XMLCh * xmlspace = XMLString::transcode("");
                    theElement->setAttributeNS( xmlspace, tempKey, tempValue );
                    XMLString::release( & tempKey );
                    XMLString::release( & tempValue );
                    XMLString::release( & xmlspace );
                    
                }
                
                // add all children
                for (unsigned int i = 0; i < someNode->countAllChildren() ; i++){
                    DOMElement * el = createNode(doc, someNode->getAnyChild(i), writer);
                    
                    if (el != NULL)
                        theElement->appendChild(el);
                }
            }catch (const OutOfMemoryException&)
            {
                logPrintE("XMLWriter error, out of memory exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
            }
            catch (const DOMException&)
            {
                logPrintE("XMLWriter error, DOM exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
            }
            catch (...)
            {
                logPrintE("XMLWriter error, unknown exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
            }
            return theElement;
        }else {
            DOMElement* theElement = NULL;
            try {
                theElement = doc->createElement(X("Ref"));
                // add all attributes

                XMLCh * tempKey = XMLString::transcode( "USE" );
                XMLCh * tempValue = XMLString::transcode( defstring.c_str() );
                XMLCh * xmlspace = XMLString::transcode("");
                theElement->setAttributeNS( xmlspace, tempKey, tempValue );
                XMLString::release( & tempKey );
                XMLString::release( & tempValue );
                XMLString::release( & xmlspace );
                    
            }catch (const OutOfMemoryException&)
            {
                logPrintE("XMLWriter error, out of memory exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
            }
            catch (const DOMException&)
            {
                logPrintE("XMLWriter error, DOM exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
            }
            catch (...)
            {
                logPrintE("XMLWriter error, unknown exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
            }
            return theElement;


        }
    }

    DOMDocument * createDocument(Graph * graph, XMLWriter & writer){
        if (graph == NULL)
            return NULL;
       DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(X("Core"));
       DOMDocument * doc = NULL;
       DOMElement  * docElem = NULL;
       if (impl != NULL)
       {
           try
           {
               doc = impl->createDocument(
                           0,                    // root element namespace URI.
                           X("OpenTracker"),         // root element name
                           0);                   // document type object (DTD).
               docElem = doc->getDocumentElement();

               
           }catch(...){
               logPrintE("XMLWriter failed to create DOMDocument instance\n");
               
           }
       }

       if (docElem != NULL){
           DOMComment * com = doc->createComment(X("OpenTracker data version 2.0 XERCES XMLWriter"));
           docElem->appendChild(com);
           // now create all the nodes starting from the rootNode
           for (unsigned int i=0 ; i < (graph->countNodes()); i++){
               Node * someRootNode = graph->getNode(i);
               logPrintI("XMLWriter:: writing node %s parentcount %u\n", (someRootNode->getType().c_str()), (someRootNode->countParents()));
               if (someRootNode->countParents() == 0) {
                   DOMElement * el = createNode(doc, graph->getNode(i), writer);
                   if (el != NULL)
                       docElem->appendChild(el);
               }
           }
       }
       return doc;
    }


    void XMLWriter::write(const char * file){
        // create the DOMDocument from the graph
        DOMDocument * doc = createDocument(context.getRootNode(), *this);
        
	std::auto_ptr<XMLFormatTarget> myFormatTarget ( new LocalFileFormatTarget( file ));
        writeNode( (DOMNode *)(doc) , myFormatTarget.get());
        

        doc->release();
    };
}

#endif //USE_XERCES
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

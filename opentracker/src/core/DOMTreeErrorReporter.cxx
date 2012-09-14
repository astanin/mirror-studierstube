/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id: DOMTreeErrorReporter.cxx,v 1.6 2003/07/18 20:26:46 tamer Exp $
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include <OpenTracker/OpenTracker.h>

// selects between usage of XERCES and TinyXML
#include <OpenTracker/tool/XMLSelection.h>


#ifdef USE_XERCES


#include <xercesc/sax/SAXParseException.hpp>
#include <OpenTracker/core/DOMTreeErrorReporter.h>

#include <cstdlib>
#include <cstring>

#include <iostream>

//using namespace std;

void DOMTreeErrorReporter::warning(const SAXParseException& toCatch)
{
    std::cerr << "Warning at file \"" << XMLString::transcode(toCatch.getSystemId())
              << "\", line " << toCatch.getLineNumber()
              << ", column " << toCatch.getColumnNumber()
              << "\n   Message: " << XMLString::transcode( toCatch.getMessage()) << std::endl;
    errorNum++;
}

void DOMTreeErrorReporter::error(const SAXParseException& toCatch)
{
    std::cerr << "Error at file \"" << XMLString::transcode(toCatch.getSystemId())
              << "\", line " << toCatch.getLineNumber()
              << ", column " << toCatch.getColumnNumber()
              << "\n   Message: " << XMLString::transcode( toCatch.getMessage()) << std::endl;
    errorNum++;
}

void DOMTreeErrorReporter::fatalError(const SAXParseException& toCatch)
{
    std::cerr << "Fatal Error at file \"" << XMLString::transcode(toCatch.getSystemId())
              << "\", line " << toCatch.getLineNumber()
              << ", column " << toCatch.getColumnNumber()
              << "\n   Message: " << XMLString::transcode( toCatch.getMessage()) << std::endl;
    errorNum++;
}

void DOMTreeErrorReporter::resetErrors()
{
    errorNum = 0;
}

int DOMTreeErrorReporter::errorsEncountered()
{
    return errorNum;
}


#endif //USE_XERCES

/* 
 * ------------------------------------------------------------
 *   End of DOMTreeErrorReporter.cxx
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

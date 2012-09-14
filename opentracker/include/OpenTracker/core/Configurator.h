
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
/** header file for class Configurator.
 *
 * @author Eduardo E. Veas
 *
 * $Id: ConfigurationParser.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

#ifndef CONFIGURATOR_HH
#define CONFIGURATOR_HH

#include "../dllinclude.h"

#ifndef SWIG
#include <string>
#endif

namespace ot 
{

    class Context;
#ifdef USE_LIVE
    class LiveContext;
#endif
    class ThreadContext;
    class ConfigurationThread;
    enum ContextType {
        NORMAL = 0,
        THREAD = 1
#ifdef USE_LIVE
        ,  LIVE =2
#endif
    };

    class OPENTRACKER_API Configurator {
    public:
        typedef void (*ModuleInitFunc) (Context *, void * data);
	 
    protected:
        struct MIFunctor{
            ModuleInitFunc function;
            void * data;
            //MIFunctor(ModuleInitFunc f, void * d):function(f), data(d){};
        };
    public:
        typedef std::map<std::string, MIFunctor> Registry;
    protected:
	static Registry initFunctions;
	static Configurator * self;
        Context * ctx;
	bool isDefaultContext;

	ConfigurationThread * thread;

	static void doInitialization(Context & newctx);
      
	Configurator(int ctx_type = NORMAL);
#ifdef USE_LIVE
               LiveContext* createLiveContext();
#endif       

    public:
	

	static Configurator * instance(int ctx_type = NORMAL);
	virtual ~Configurator();

	Context * getContext();
	void parseConfigurationString(std::string configString);
	void changeConfiguration(std::string configString);
	void changeConfigurationFile(const char* xmlstring);
	void changeConfigurationString(const char* xmlstring);

	void runConfigurationThread(const char * filename);

	static void addModuleInit(const char * name, ModuleInitFunc func, void * data);
	static void loadModule(Context & newctx, const char * module);
	friend OPENTRACKER_API void initializeContext(Context *, void *);
    };

}


#endif //CONFIGURATOR_HH



/* 
 * ------------------------------------------------------------
 *   End of Configurator.h
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

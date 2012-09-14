// pythonbindings.cpp : Defines the entry point for the WIN32 DLL.

#include <stb/components/pythonbindings/pythonbindings.h>


#include <stb/components/pythonbindings/SoPyScript.h>




CREATE_COMPONENT_FUNC(Pythonbindings)

BEGIN_NAMESPACE_STB

/**
*     The Constructor
*/
Pythonbindings::Pythonbindings()
{
    isInit=false;
}
/**
*     The destructor.
*/
Pythonbindings::~Pythonbindings()
{
    //nil
}
/// Called before the application is destructed.
void
Pythonbindings::shutDown()
{

}

void 
Pythonbindings::setParameter(stb::string key, std::string value)
{
	if(key=="configFile")
	{
		configFile=value;
	}
}

bool
Pythonbindings::init(void)
{
    if(isInit)
        return true;

    isInit=true;
    SoPyScript::initClass();

    return isInit;
}


SoPyScript*
Pythonbindings::createSoPyScript()
{
    return new SoPyScript;
}


END_NAMESPACE_STB

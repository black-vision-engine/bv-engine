#pragma once
#include "ApplicationBase.h"
#include <string>

namespace bv
{

class ConsoleApplication : public ApplicationBase
{
protected:

    std::string		m_Title;

public:
    ConsoleApplication			( const std::string & title );
    virtual ~ConsoleApplication ();

    virtual int MainFun			( int argc, char ** argv ) = 0;

protected:

    static int MainImpl			( int argc, char ** argv );
};

}

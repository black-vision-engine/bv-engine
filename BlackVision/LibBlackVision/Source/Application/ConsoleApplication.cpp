#include "stdafx.h"

#include "ConsoleApplication.h"

namespace bv {

// *********************************
//
ConsoleApplication::ConsoleApplication ( const std::string & title )
    : m_Title( title )
{
}

// *********************************
//
ConsoleApplication::~ConsoleApplication ()
{
}

// *********************************
//
int ConsoleApplication::MainImpl		( int argc, char ** argv )
{
    return static_cast< ConsoleApplication * >( ApplicationBase::ApplicationInstance )->MainFun( argc, argv );
}

}
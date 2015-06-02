#pragma once
#include "Application\WindowedApplication.h"
#include "gtest/gtest.h"

namespace bv
{

class WinApplicationTester	:	public WindowedApplication
{
private:


public:
    static bool			m_sWindowedApplicationInitialized;
public:
    static void			StaticInitializer	();
    static bool			RegisterInitializer ();


public:
	WinApplicationTester();
	~WinApplicationTester();

	virtual int MainFun( int argc, char ** argv ) override;
	virtual void OnIdle();
};


}

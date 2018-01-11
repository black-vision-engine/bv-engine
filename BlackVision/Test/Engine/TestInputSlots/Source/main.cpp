#include "Framework/bvAppTest.h"


// Registers initializers for BlackVisionApp instance and other usufull things like loggers etc.
bool bv::BlackVisionAppFramework::m_configOverrideInitialized = bv::BlackVisionAppFramework::OverrideConfig( "TestConfigs/VideoCards/2Cards-FakeInputs.xml" );
bool bv::BlackVisionApp::m_sWindowedApplicationInitialized = bv::BlackVisionAppFramework::RegisterFrameworkInitializer();

#include "Framework/TestMain.h"

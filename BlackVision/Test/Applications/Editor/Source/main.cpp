#include "Framework/bvAppTest.h"


// Registers initializers for BlackVisionApp instance and other usufull things like loggers etc.
bool bv::BlackVisionAppFramework::m_configOverrideInitialized = bv::BlackVisionAppFramework::OverrideConfig( "TestConfigs/TestEditorConfig.xml" );
bool bv::BlackVisionApp::m_sWindowedApplicationInitialized = bv::BlackVisionAppFramework::RegisterFrameworkInitializer();

#include "Application/Main.h"

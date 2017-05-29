#include "Framework/bvAppTest.h"


// Registers initializers for BlackVisionApp instance and other usufull things like loggers etc.
bool bv::BlackVisionApp::m_sWindowedApplicationInitialized = bv::BlackVisionAppFramework::RegisterFrameworkInitializer();

#include "Application/Main.h"

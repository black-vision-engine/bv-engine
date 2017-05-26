#include "bvApp.h"

// Registers initializers for BlackVisionApp instance and other usufull things like loggers etc.
bool bv::BlackVisionApp::m_sWindowedApplicationInitialized = bv::BlackVisionApp::RegisterInitializer();

#include "Application/Main.h"

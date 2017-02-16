#include "bvApp.h"
#include "UseLoggerBVAppModule.h"



// *********************************
// Default file path and init message constats

const std::string DEFAULT_LOG_FILE_PATH = "../../../../Logs/DefaultLog.txt";
const std::string INIT_TEXT_STRING = "BlackVision initialized, version: ";

#ifdef _DEBUG
	const std::string VERSION_DEBUG_STRING = " Debug";
#else
	const std::string VERSION_DEBUG_STRING = " Release";
#endif

#ifdef _MSC_VER
	// Macros defined in visual compiler
	#ifdef _M_AMD64
		const std::string VERSION_ARCHITECTURE_STRING = "x64";
	#elif defined( _M_IX86 )
		const std::string VERSION_ARCHITECTURE_STRING = "x86";
	#else
		const std::string VERSION_ARCHITECTURE_STRING = "Unknown Architecture";
	#endif
#else
	// For other compilers
	const std::string VERSION_ARCHITECTURE_STRING = "";
#endif

// *********************************
// Initializer function
void			bv::BlackVisionApp::LoggerInitializer	( int /*argc*/, char * /*argv*/[] )
{
	auto& logger = Logger::GetLogger();
	logger.SetFileRotationSize( 1024 * 1024 );		// Affects files added after that.
	logger.AddConsole();

#ifdef _DEBUG
	logger.AddLogFile( DEFAULT_LOG_FILE_PATH, bv::SeverityLevel::debug );
#else
	logger.AddLogFile( DEFAULT_LOG_FILE_PATH, bv::SeverityLevel::info );
#endif

	LOG_MESSAGE( bv::SeverityLevel::info ) << INIT_TEXT_STRING << VERSION_ARCHITECTURE_STRING << VERSION_DEBUG_STRING;
}


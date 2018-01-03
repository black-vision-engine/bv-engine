#include "bvApp.h"
#include "UseLoggerBVAppModule.h"



// *********************************
// Default file path and init message constats

const std::string DEFAULT_LOG_FILE_PATH = "Logs/DefaultLog.txt";
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


// ***********************
//
int             FindArgument    ( int argc, char * argv[], const char * searchedString, int startIdx = 1 )
{
    for( int i = startIdx; i < argc; ++i )
    {
        if( strcmp( argv[ i ], searchedString ) == 0 )
        {
            return i;
        }
    }
    return 0;
}

// ***********************
//
const char * SeverityStrings[] =
{
    "debug",
    "info",
    "warning",
    "error",
    "critical"
};

// ***********************
//
bv::SeverityLevel       GetSeverityLevel    ( int argc, char * argv[], int startIdx, bv::SeverityLevel defaultLevel )
{
    for( int i = startIdx + 1; i < argc; ++i )
    {
        if( argv[ i ][ 0 ] == '-' )
        {
            // Next parameter. Stop parsing
            return defaultLevel;
        }

        for( int sevIdx = 0; sevIdx < ARRAYSIZE( SeverityStrings ); ++sevIdx )
        {
            if( strcmp( argv[ i ], SeverityStrings[ sevIdx ] ) == 0 )
                return static_cast< bv::SeverityLevel >( sevIdx );
        }
    }

    return defaultLevel;
}

// ***********************
//
std::string             GetLoggerFileName   ( int argc, char * argv[], int startIdx )
{
    // Logger sink file name should directly follow startIdx
    if( startIdx + 1 < argc )
        return argv[ startIdx + 1 ];
    else
        return "";
}

// ***********************
//
bool                    DisableDefaultLog   ( int argc, char * argv[] )
{
    return FindArgument( argc, argv, "-DisableDefaultLog" ) != 0;
}


// *********************************
// Initializer function
void			bv::BlackVisionApp::LoggerInitializer	( int argc, char * argv[] )
{

#ifdef _DEBUG
    bv::SeverityLevel defaultLevel = bv::SeverityLevel::debug;
#else
    bv::SeverityLevel defaultLevel = bv::SeverityLevel::warning;
#endif

	auto& logger = Logger::GetLogger();
	logger.SetFileRotationSize( 1024 * 1024 );		// Affects files added after that.

    auto consoleIdx = FindArgument( argc, argv, "-ConsoleLog" );
    if( consoleIdx )
    {
        bv::SeverityLevel level = GetSeverityLevel( argc, argv, consoleIdx, defaultLevel );
        logger.AddConsole( level );
    }

    auto fileLogIdx = FindArgument( argc, argv, "-FileLog" );
    while( fileLogIdx != 0 )
    {
        auto fileName = GetLoggerFileName( argc, argv, fileLogIdx );
        bv::SeverityLevel level = GetSeverityLevel( argc, argv, fileLogIdx, defaultLevel );

        if( !fileName.empty() )
            logger.AddLogFile( fileName, level );

        fileLogIdx = FindArgument( argc, argv, "-FileLog", fileLogIdx + 1 );
    }

    if( !DisableDefaultLog( argc, argv ) )
    {
        logger.AddLogFile( DEFAULT_LOG_FILE_PATH, defaultLevel );
    }


	LOG_MESSAGE( bv::SeverityLevel::info ) << INIT_TEXT_STRING << VERSION_ARCHITECTURE_STRING << VERSION_DEBUG_STRING;
}


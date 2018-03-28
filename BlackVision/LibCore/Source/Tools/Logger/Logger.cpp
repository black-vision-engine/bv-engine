#include "stdafxCore.h"

#include "Logger.h"

#include <fstream>
#include <unordered_map>
#include <iomanip>
#include "QueueSink.h"


#pragma warning( disable : 4512 )
// warning: could not generate contructor for...
#pragma warning( disable : 4100 )
// warning: unreferenced formal parameter x

#pragma warning ( disable : 4996 )
// warning '_vsnprintf': This function or variable may be unsafe. Consider using _vsnprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
#pragma warning( disable : 4714 )
// warning: funcion marked as __forceinline not inlined


#include <boost/log/support/date_time.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost\log\sinks\text_ostream_backend.hpp>
#include <boost\log\sinks\text_file_backend.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/sinks/sync_frontend.hpp>

#include "DataTypes/QueueConcurrent.h"
#include "Serialization/ConversionHelper.h"


typedef boost::log::sinks::asynchronous_sink< boost::log::sinks::text_file_backend > ASyncFileSink;
typedef boost::log::sinks::asynchronous_sink< boost::log::sinks::text_ostream_backend > ASyncStreamSink;
typedef boost::log::sinks::synchronous_sink< bv::QueueSink > SyncQueueSink;
typedef bv::LoggerType::char_type char_type;
typedef std::unordered_map< bv::ModuleEnum, std::string > ModuleMapping;


namespace bv { namespace LogHelperString {
	const char  * FILE_DESC_STRING = "In File: ";
	const char* LINE_DESC_STRING = ", Line ";
}}



const char * SEVERITY_DEBUG_STRING      = "debug";
const char * SEVERITY_INFO_STRING       = "info";
const char * SEVERITY_WARNING_STRING    = "warning";
const char * SEVERITY_ERROR_STRING      = "error";
const char * SEVERITY_CRITICAL_STRING   = "critical";

static const char* SEVERITY_STRINGS[] =
{
    SEVERITY_DEBUG_STRING,
    SEVERITY_INFO_STRING,
    SEVERITY_WARNING_STRING,
    SEVERITY_ERROR_STRING,
    SEVERITY_CRITICAL_STRING
};



unsigned int modulesStringAlignment = 14;
unsigned int severityLevelAlignment = 8;


// ***********************
//
ModuleMapping &     ModuleString()
{
    static ModuleMapping moduleString;
    return moduleString;
}

namespace bv
{

DECLARE_ENUM_SERIALIZATION( ModuleEnum );

namespace Convert
{

// ***********************
//
std::pair< ModuleEnum, const char* > ModuleMapping[] =
{ std::make_pair( ModuleEnum::ME_LibCore, "LibCore" )
, std::make_pair( ModuleEnum::ME_LibBlackVision, "LibBlackVision" )
, std::make_pair( ModuleEnum::ME_LibImage, "LibImage" )
, std::make_pair( ModuleEnum::ME_Prototyper, "Prototyper" )
, std::make_pair( ModuleEnum::ME_BlackVisionApp, "BlackVisionApp" )
, std::make_pair( ModuleEnum::ME_LibProjectManager, "LibProjectManager" )
, std::make_pair( ModuleEnum::ME_TCPServer, "LibTCPServer" )
, std::make_pair( ModuleEnum::ME_LibVideoCards, "LibVideoCards" )
, std::make_pair( ModuleEnum::ME_XMLScenParser, "LibXMLSceneParser" )
, std::make_pair( ModuleEnum::ME_LibEffect, "LibEffect" )
, std::make_pair( ModuleEnum::ME_Tests, "Tests" )
, std::make_pair( ModuleEnum::ME_TOTAL, "Unknown module" )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( ModuleEnum, ModuleMapping );

}
}



// *********************************
// Template specializaion is used to supress warning
// warning: funcion marked as __forceinline not inlined
template<>
::boost::log::aux::record_pump< bv::LoggerType >
	boost::log::aux::make_record_pump( bv::LoggerType& lg, ::boost::log::record& rec )
{
    return ::boost::log::aux::record_pump< bv::LoggerType >(lg, rec);
}



struct severity_tag;
struct module_tag;

// *********************************
//
boost::log::formatting_ostream& operator<< ( boost::log::formatting_ostream & strm, const boost::log::to_log_manip< bv::SeverityLevel, severity_tag > & manip )
{
    bv::SeverityLevel level = manip.get();
    if (static_cast< std::size_t >(level) < sizeof(SEVERITY_STRINGS) / sizeof(*SEVERITY_STRINGS))
        strm << SEVERITY_STRINGS[level];
    else
        strm << static_cast< int >(level);

    return strm;
}

// *********************************
//
std::string toString( bv::ModuleEnum moduleEnum )
{
    return bv::Convert::T2String( moduleEnum );
}

// *********************************
//
boost::log::formatting_ostream& operator<< ( boost::log::formatting_ostream& strm, boost::log::to_log_manip< bv::ModuleEnum, module_tag > const& manip )
{
    bv::ModuleEnum level = manip.get();
    strm << toString( level );

    return strm;
}



namespace bv{
	

// ==================================================================== //
//					Logging object

LoggingHelper::LoggingHelper( LoggerType& logger, SeverityLevel level, ModuleEnum module )
	:	m_record( logger.open_record( (boost::log::keywords::channel = module, boost::log::keywords::severity = level) ) ),
	m_logger( logger )
{}

::boost::log::aux::record_pump< LoggerType > 		LoggingHelper::recordPump()
{
	return ::boost::log::aux::make_record_pump((m_logger), m_record);
}

// ==================================================================== //
//					Logger class

Logger& Logger::GetLogger()
{
	static Logger sEngineLogger;

	return sEngineLogger;
}

Logger::Logger()
{
	InitForamatter();
	boost::log::add_common_attributes();

	m_fileRotationSize = 5 * 1024 * 1024;
}


Logger::~Logger()
{
}

void Logger::InitForamatter()
{
	namespace expr = boost::log::expressions;

    m_formatter = expr::stream
			<< std::left
			<< "[" << std::setw( modulesStringAlignment ) << expr::attr< bv::ModuleEnum, module_tag >("Channel") << "] "
			<< std::right
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << " "
			<< std::setw( severityLevelAlignment ) << expr::attr< bv::SeverityLevel, severity_tag >("Severity") << ": "
            << expr::message;
}



// ==================================================================== //
//					Helper function

// *********************************
// Sets filter
template<typename Type>
void SetFilter( boost::shared_ptr<Type> sink, SeverityLevel minLevel, int modules )
{
	//I don't know how to do this with loop. It don't want to work.
	//Operator & isn't defined for filters, so I can't do this like this ( module & modules )
	auto filter = 
		(severity >= minLevel) &&
		(
			(module == (modules & ( modules & (0x1 << 0) )))
			|| (module == (modules & ( modules & (0x1 << 1) )))
			|| (module == (modules & ( modules & (0x1 << 2) )))
			|| (module == (modules & ( modules & (0x1 << 3) )))
			|| (module == (modules & ( modules & (0x1 << 4) )))
			|| (module == (modules & ( modules & (0x1 << 5) )))
			|| (module == (modules & ( modules & (0x1 << 6) )))
			|| (module == (modules & ( modules & (0x1 << 7) )))
			|| (module == (modules & ( modules & (0x1 << 8) )))
			//|| (module == (modules & ( modules & (0x1 << 9) )))
			//|| (module == (modules & ( modules & (0x1 << 10) )))
			//|| (module == (modules & ( modules & (0x1 << 11) )))
			//|| (module == (modules & ( modules & (0x1 << 12) )))
			//|| (module == (modules & ( modules & (0x1 << 13) )))
			//|| (module == (modules & ( modules & (0x1 << 14) )))
			//|| (module == (modules & ( modules & (0x1 << 15) )))
			//|| (module == (modules & ( modules & (0x1 << 16) )))
			//|| (module == (modules & ( modules & (0x1 << 17) )))
			//|| (module == (modules & ( modules & (0x1 << 18) )))
			//|| (module == (modules & ( modules & (0x1 << 19) )))
			//|| (module == (modules & ( modules & (0x1 << 20) )))
			//|| (module == (modules & ( modules & (0x1 << 21) )))
			//|| (module == (modules & ( modules & (0x1 << 22) )))
			//|| (module == (modules & ( modules & (0x1 << 23) )))
			//|| (module == (modules & ( modules & (0x1 << 24) )))
			//|| (module == (modules & ( modules & (0x1 << 25) )))
			//|| (module == (modules & ( modules & (0x1 << 26) )))
			//|| (module == (modules & ( modules & (0x1 << 27) )))
			//|| (module == (modules & ( modules & (0x1 << 28) )))
			//|| (module == (modules & ( modules & (0x1 << 29) )))
			//|| (module == (modules & ( modules & (0x1 << 30) )))
			//|| (module == (modules & ( modules & (0x1 << 31) )))
		);

	sink->set_filter( filter );
}


// ========================================================================= //
// Functions adding sinks
// ========================================================================= //


// *********************************
//
std::unordered_map< int, boost::shared_ptr< boost::log::sinks::sink > >  globalSinksMap;
CriticalSection sinksLock;
int sinkID = 1;

// ***********************
//
int AddNewSink( boost::shared_ptr< boost::log::sinks::sink > newSink )
{
    int newSinkId;
    {
        ScopedCriticalSection lock( sinksLock );
        newSinkId = sinkID;
        globalSinksMap.insert( std::make_pair( sinkID++, newSink ) );
    }

	boost::log::core::get()->add_sink( newSink );
    return newSinkId;
}

// *********************************
//
// *********************************
//
// *********************************
//
// *********************************
//
int Logger::AddLogFile( const std::string & fileName, SeverityLevel minLevel, int modules )
{
    boost::shared_ptr< boost::log::sinks::text_file_backend > backend =
        boost::make_shared< boost::log::sinks::text_file_backend >(
            boost::log::keywords::file_name = fileName,
			boost::log::keywords::rotation_size = m_fileRotationSize,
			boost::log::keywords::open_mode = std::ios_base::app );

	backend->auto_flush( true );			// Needed when critical errors occure.

	boost::shared_ptr< ASyncFileSink > newSink = boost::make_shared< ASyncFileSink >( backend );

    newSink->set_formatter( m_formatter );
	SetFilter( newSink, minLevel, modules );
    
    return AddNewSink( newSink );
}

// *********************************
//
int Logger::AddConsole			( SeverityLevel minLevel, int modules )
{
	boost::shared_ptr< ASyncStreamSink > newSink = boost::make_shared< ASyncStreamSink >();
	boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
	newSink->locked_backend()->add_stream( stream );

    newSink->set_formatter( m_formatter );
    SetFilter( newSink, minLevel, modules );

    return AddNewSink( newSink );
}

// ***********************
//
QueueConcurrent<LogMsg>& Logger::AddLogQueue         ( int& logID, SeverityLevel minLevel, int modules )
{
    boost::shared_ptr< QueueSink > backend = boost::make_shared< QueueSink >();
    boost::shared_ptr< SyncQueueSink > newSink( new SyncQueueSink( backend ) );

	SetFilter( newSink, minLevel, modules );
    logID = AddNewSink( newSink );

    return backend->GetQueueReference();
}

// ***********************
//
void Logger::RemoveLog           ( int logID )
{
    ScopedCriticalSection lock( sinksLock );

    auto findResult = globalSinksMap.find( logID );
    if( findResult != globalSinksMap.end() )
    {
        boost::log::core::get()->remove_sink( findResult->second );
        globalSinksMap.erase( findResult );
    }
}

// ***********************
//
void QueueSink::consume( boost::log::record_view const& rec )
{
    namespace expr = boost::log::expressions;

    LogMsg message;
    message.message = *rec[ expr::smessage ];
    message.severity = SEVERITY_STRINGS[ rec[ ::bv::severity ].get() ];
    message.module = toString( rec[ ::bv::module ].get() );

    m_queue.Push( message );
}


} //bv

#pragma warning( default : 4996 )

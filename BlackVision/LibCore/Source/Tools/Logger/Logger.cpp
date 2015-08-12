#include "Logger.h"

#include <fstream>
#include <iomanip>
#include "UseLogger.h"

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
#include <boost/core/null_deleter.hpp>

typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > SyncSink;
typedef bv::LoggerType::char_type char_type;


const char* SEVERITY_DEBUG_STRING = "debug";
const char* SEVERITY_INFO_STRING = "info";
const char* SEVERITY_WARNING_STRING = "warning";
const char* SEVERITY_ERROR_STRING = "error";
const char* SEVERITY_CRITICAL_STRING = "critical";

static const char* SEVERITY_STRINGS[] =
{
    SEVERITY_DEBUG_STRING,
    SEVERITY_INFO_STRING,
    SEVERITY_WARNING_STRING,
    SEVERITY_ERROR_STRING,
    SEVERITY_CRITICAL_STRING
};


const char* MODULE_LIBBLACKVISION_STRING = "LibBlackVision";
const char* MODULE_LIBCORE_STRING = "LibCore";
const char* MODULE_LIBIMAGE_STRING = "LibImage";
const char* MODULE_PROTOTYPER_STRING = "Prototyper";
const char* MODULE_BLACKVISIONAPP_STRING = "BlackVisionApp";

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

boost::log::formatting_ostream& operator<< ( boost::log::formatting_ostream& strm, boost::log::to_log_manip< bv::SeverityLevel, severity_tag > const& manip )
{
    bv::SeverityLevel level = manip.get();
    if (static_cast< std::size_t >(level) < sizeof(SEVERITY_STRINGS) / sizeof(*SEVERITY_STRINGS))
        strm << SEVERITY_STRINGS[level];
    else
        strm << static_cast< int >(level);

    return strm;
}

boost::log::formatting_ostream& operator<< ( boost::log::formatting_ostream& strm, boost::log::to_log_manip< bv::ModuleEnum, module_tag > const& manip )
{
	// @todo mapowanie na stringi
    bv::ModuleEnum level = manip.get();

	switch( level )
	{
	case bv::ModuleEnum::LibBlackVision:
		strm << MODULE_LIBBLACKVISION_STRING;
		break;
	case bv::ModuleEnum::LibCore:
		strm << MODULE_LIBCORE_STRING;
		break;
	case bv::ModuleEnum::LibImage:
		strm << MODULE_LIBIMAGE_STRING;
		break;
	case bv::ModuleEnum::BlackVisionApp:
		strm << MODULE_BLACKVISIONAPP_STRING;
		break;
	case bv::ModuleEnum::Prototyper:
		strm << MODULE_PROTOTYPER_STRING;
		break;
	default:
		strm << "Unknown Module";
	}


    return strm;
}


namespace bv{




// ==================================================================== //
//					Logging object
	//, boost::log::keywords::channel = ModuleEnum::Prototyper
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

void								Logger::LoggerTest()
{
	//BOOST_LOG_CHANNEL_SEV( Logger::GetLogger().Get(), ModuleEnum::Prototyper, SeverityLevel::critical ) << "Channel";
	LOG_MESSAGE(SeverityLevel::info) << "Logger test passed";
}


Logger& Logger::GetLogger()
{
	static Logger sEngineLogger;

	return sEngineLogger;
}

Logger::Logger()
{
	InitForamatter();
	boost::log::add_common_attributes();
}


Logger::~Logger()
{
}

void Logger::InitForamatter()
{
	namespace expr = boost::log::expressions;

    m_formatter = expr::stream
            //<< std::hex << std::setw(8) << std::setfill('0') << expr::attr< unsigned int >("LineID") << ": "
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << " "
			<< std::setw(8) << expr::attr< bv::SeverityLevel, severity_tag >("Severity") << ": "
			<< "[" << expr::attr< bv::ModuleEnum, module_tag >("Channel") << "] "
            << expr::message;
}

void Logger::AddLogFile( const std::string& fileName, SeverityLevel minLevel, int modules )
{
	boost::shared_ptr< SyncSink > newSink = boost::make_shared< SyncSink >();
	newSink->locked_backend()->add_stream( boost::make_shared< std::ofstream >( fileName.c_str() ) );

    newSink->set_formatter( m_formatter );

	newSink->set_filter
	(
		(severity >= minLevel)
		&&
		(
			(module == (modules & ModuleEnum::BlackVisionApp))
			|| (module == (modules & ModuleEnum::LibBlackVision))
			|| (module == (modules & ModuleEnum::LibCore))
			|| (module == (modules & ModuleEnum::LibImage))
			|| (module == (modules & ModuleEnum::LibImage))
			|| (module == (modules & ModuleEnum::Prototyper))
		)
	);

	boost::log::core::get()->add_sink( newSink );
}

void Logger::AddConsole			( SeverityLevel minLevel, int modules )
{
	boost::shared_ptr< SyncSink > newSink = boost::make_shared< SyncSink >();
	boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
	newSink->locked_backend()->add_stream( stream );

    newSink->set_formatter( m_formatter );
	boost::log::core::get()->add_sink( newSink );
}

} //bv

#pragma warning( default : 4996 )

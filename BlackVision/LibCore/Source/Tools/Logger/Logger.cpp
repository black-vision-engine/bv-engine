#include "Logger.h"

#include <fstream>
#include <iomanip>

#pragma warning( disable : 4512 )
#pragma warning( disable : 4100 )

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


typedef bv::LoggerType::char_type char_type;

// Template specializaion is used to supress warning
// warning: funcion marked as __forceinline not inlined
template<>
::boost::log::aux::record_pump< bv::LoggerType >
	boost::log::aux::make_record_pump( bv::LoggerType& lg, ::boost::log::record& rec )
{
    return ::boost::log::aux::record_pump< bv::LoggerType >(lg, rec);
}


std::ostream& operator<< (std::ostream& strm, bv::Level::SeverityLevel level)
{
    static const char* strings[] =
    {
        "debug",
        "info",
        "warning",
        "error",
        "critical"
    };

    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast< int >(level);

    return strm;
}

namespace bv{




// ==================================================================== //

LoggingObject::LoggingObject( LoggerType& logger, Level::SeverityLevel level )
:	m_record( logger.open_record( boost::log::keywords::severity = level ) ),
	m_logger( logger )
{}

::boost::log::aux::record_pump< LoggerType > 		LoggingObject::recordPump()
{
	return ::boost::log::aux::make_record_pump((m_logger), m_record);
}

// ==================================================================== //

Logger& Logger::GetLogger()
{
	static Logger sEngineLogger;

	return sEngineLogger;
}

Logger::Logger()
{
}


Logger::~Logger()
{
}


void Logger::AddLogFile( const std::string& fileName )
{
	namespace expr = boost::log::expressions;

	boost::shared_ptr< SyncSink > newSink = boost::make_shared< SyncSink >();
	newSink->locked_backend()->add_stream( boost::make_shared< std::ofstream >( fileName.c_str() ) );

    boost::log::formatter formatter =
        expr::stream
            << std::hex << std::setw(8) << std::setfill('0') << expr::attr< unsigned int >("LineID") << ": "
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << " "
            << expr::message;
    newSink->set_formatter( formatter );

	boost::log::core::get()->add_sink( newSink );

	boost::log::add_common_attributes();
}

} //bv

#pragma warning( default : 4996 )

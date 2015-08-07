#include "Logger.h"

#include <fstream>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


namespace bv{


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
	boost::shared_ptr< SyncSink > newSink = boost::make_shared< SyncSink >();
	newSink->locked_backend()->add_stream( boost::make_shared< std::ofstream >( fileName.c_str() ) );

	boost::log::core::get()->add_sink( newSink );
	boost::log::add_common_attributes();
	
}

} //bv

#include "Logger.h"

#include <fstream>


#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

// Template specializaion is used to supress warning
// warning: funcion marked as __forceinline not inlined
template<>
::boost::log::aux::record_pump< boost::log::sources::logger > boost::log::aux::make_record_pump( ::boost::log::sources::logger& lg, ::boost::log::record& rec )
{
    return ::boost::log::aux::record_pump< boost::log::sources::logger >(lg, rec);
}



namespace bv{

//	 ZLO
::boost::log::aux::record_pump< boost::log::sources::logger >& Log()
{
	auto logger = (bv::Logger::GetLogger().Get());
	static ::boost::log::record rec_var = (logger).open_record();

	static auto pump = ::boost::log::aux::make_record_pump((logger), rec_var);
	return pump;
}



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


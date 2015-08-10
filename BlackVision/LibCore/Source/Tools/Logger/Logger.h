#pragma once

#include <string>

//#pragma warning( disable : 4714 )
// warning: funcion marked as __forceinline not inlined

//#pragma warning ( disable : 4996 )
// warning '_vsnprintf': This function or variable may be unsafe. Consider using _vsnprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.

#include <boost\log\sinks\text_ostream_backend.hpp>
#include <boost\log\sinks\sync_frontend.hpp>
#include <boost\log\sources\logger.hpp>
#include <boost\log\sources\record_ostream.hpp>

//#pragma warning( default : 4714 )



namespace bv{

class Logger;


typedef boost::log::sources::logger::char_type char_type;
::boost::log::aux::record_pump< boost::log::sources::logger >& Log();

#define LOG_MESSAGE (Log().stream())

//#define LOG_MESSAGE BOOST_LOG( (bv::Logger::GetLogger().Get()) )

class Logger
{
	typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > SyncSink;
private:
	boost::log::sources::logger				m_logger;

private:
	Logger();
	~Logger();

public:
	void									AddLogFile			( const std::string& fileName );

	// Logowanie
	boost::log::sources::logger&			Get()									{ return m_logger; }
	static Logger&							GetLogger();
};



} // bv



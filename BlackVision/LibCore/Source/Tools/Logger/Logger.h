#pragma once

#include <string>

#pragma warning( disable : 4714 )
// warning: funcion marked as __forceinline not inlined

#include <boost\log\sinks\text_ostream_backend.hpp>
#include <boost\log\sinks\sync_frontend.hpp>
#include <boost\log\sources\logger.hpp>
#include <boost\log\sources\record_ostream.hpp>

#pragma warning( default : 4714 )


namespace bv{

class Logger;

#define LOG_MESSAGE BOOST_LOG( (bv::Logger::GetLogger().Get()) )

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

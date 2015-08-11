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


/** This object must hide boost implementation of
opening record and creating record pump. Otherwise warnings occures.*/
class LoggingObject
{
private:
	boost::log::record				m_record;
	boost::log::sources::logger&	m_logger;
public:
	LoggingObject( boost::log::sources::logger& logger )
	:	m_record( logger.open_record() ),
		m_logger( logger )
	{}


	::boost::log::aux::record_pump< boost::log::sources::logger > 		recordPump();
	bool																operator!()	{ return !m_record; }
};


#define LOG_MESSAGE \
for( LoggingObject loggingObject( bv::Logger::GetLogger().Get() ); !!loggingObject; )		\
	loggingObject.recordPump().stream()

				

/**This is singleton logger object.*/
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



#pragma once

#include <string>

#pragma warning( disable : 4512 )
// warning: could not generate contructor for...
#pragma warning( disable : 4100 )
// warning: unreferenced formal parameter x

#include <boost\log\sinks\text_ostream_backend.hpp>
#include <boost\log\sinks\sync_frontend.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost\log\sources\record_ostream.hpp>




namespace bv{

class Logger;



enum SeverityLevel : int
{
	debug			= 0,
	info			= 1,
	warning			= 2,
	error			= 3,
	critical		= 4
};

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", ::bv::SeverityLevel)


typedef ::boost::log::sources::severity_channel_logger_mt < bv::SeverityLevel, std::string > LoggerType;

/** This object must hide boost implementation of
opening record and creating record pump. Otherwise warnings occures.*/
class LoggingHelper
{
private:
	boost::log::record										m_record;
	LoggerType&												m_logger;
public:
	LoggingHelper( LoggerType& logger, SeverityLevel level );

	::boost::log::aux::record_pump< LoggerType > 		recordPump();
	bool												operator!()	{ return !m_record; }
};


#define LOG_MESSAGE( severityLevel ) \
for( LoggingHelper loggingHelper( bv::Logger::GetLogger().Get(), severityLevel ); !!loggingHelper; )		\
	loggingHelper.recordPump().stream()

				

/**This is singleton logger object.*/
class Logger
{
	typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > SyncSink;
private:
	LoggerType				m_logger;

	boost::log::formatter	m_formatter;
private:
	Logger();
	~Logger();

	void InitForamatter();
public:
	void									AddLogFile			( const std::string& fileName );

	// Logowanie
	LoggerType&								Get()									{ return m_logger; }
	static Logger&							GetLogger();
};



} // bv



#pragma warning( default : 4512 )
#pragma warning( default : 4100 )

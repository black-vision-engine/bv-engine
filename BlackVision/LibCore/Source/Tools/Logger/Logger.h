#pragma once

#include <string>


#pragma warning( disable : 4512 )
// warning: could not generate contructor for...
#pragma warning( disable : 4100 )
// warning: unreferenced formal parameter x


#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost\log\sources\record_ostream.hpp>
#include <boost\log\sinks\sync_frontend.hpp>



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

enum ModuleEnum : int
{
	LibBlackVision	= 1 << 0,
	LibCore			= 1 << 1,
	LibImage		= 1 << 2,
	Prototyper		= 1 << 3,
	BlackVisionApp	= 1 << 4
};

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", ::bv::SeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", ::bv::ModuleEnum)


typedef ::boost::log::sources::severity_channel_logger_mt < bv::SeverityLevel, bv::ModuleEnum > LoggerType;

/** This object must hide boost implementation of
opening record and creating record pump. Otherwise warnings occur.*/
class LoggingHelper
{
private:
	boost::log::record										m_record;
	LoggerType&												m_logger;
public:
	LoggingHelper( LoggerType& logger, SeverityLevel level, ModuleEnum module );

	::boost::log::aux::record_pump< LoggerType > 		recordPump();
	bool												operator!()	{ return !m_record; }
};


#define LOG_MESSAGE( severityLevel ) \
for( LoggingHelper loggingHelper( bv::Logger::GetLogger().Get(), severityLevel, LOG_MODULE ); !!loggingHelper; )		\
	loggingHelper.recordPump().stream()

				

/**This is singleton logger object.*/
class Logger
{
private:
	LoggerType				m_logger;

	boost::log::formatter	m_formatter;
private:
	Logger();
	~Logger();

	void InitForamatter();
public:
	void									AddLogFile			( const std::string& fileName );
	void									AddConsole			();

	// Logowanie
	LoggerType&								Get()									{ return m_logger; }
	static Logger&							GetLogger();


	static void								LoggerTest();
};



} // bv



#pragma warning( default : 4512 )
#pragma warning( default : 4100 )

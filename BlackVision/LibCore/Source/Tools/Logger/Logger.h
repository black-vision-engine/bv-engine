#pragma once

#include <string>
#include "DataTypes/QueueConcurrent.h"
#include "LogMsgStruct.h"

#pragma warning( disable : 4512 )
// warning: could not generate contructor for...
#pragma warning( disable : 4100 )
// warning: unreferenced formal parameter x


#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost\log\sources\record_ostream.hpp>
//#include <boost\log\sinks\sync_frontend.hpp>
#include <boost\log\sinks\async_frontend.hpp>


#include "LoggerEnums.h"

namespace bv
{

class Logger;


BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", ::bv::SeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(module, "Channel", ::bv::ModuleEnum)

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


namespace LogHelperString
{

extern const char* FILE_DESC_STRING;
extern const char* LINE_DESC_STRING;

}

#define LOG_MESSAGE( severityLevel ) \
for( LoggingHelper loggingHelper( bv::Logger::GetLogger().Get(), severityLevel, LOG_MODULE ); !!loggingHelper; )		\
	loggingHelper.recordPump().stream()

#define LOG_MESSAGE_FILE_LINE( severityLevel ) \
for( LoggingHelper loggingHelper( bv::Logger::GetLogger().Get(), severityLevel, LOG_MODULE ); !!loggingHelper; )		\
	loggingHelper.recordPump().stream()	<< ::bv::LogHelperString::FILE_DESC_STRING << __FILE__ << ::bv::LogHelperString::LINE_DESC_STRING << __LINE__ << ": "

/**This is singleton logger object.*/
class Logger
{
private:
	LoggerType				m_logger;

	boost::log::formatter	m_formatter;

	unsigned int			m_fileRotationSize;
private:
	Logger();
	~Logger();

	void						InitForamatter		();
public:
	void						AddLogFile			( const std::string& fileName, SeverityLevel minLevel = SeverityLevel::debug, int modules = 0xFFFFFFFF );
	void						AddConsole			( SeverityLevel minLevel = SeverityLevel::debug, int modules = 0xFFFFFFFF );
    QueueConcurrent<LogMsg>&    AddLogQueue         ( SeverityLevel minLevel = SeverityLevel::debug, int modules = 0xFFFFFFFF );


	/// Affects all files, that will be added after this call.
	void						SetFileRotationSize	( unsigned int newSize )	{ m_fileRotationSize = newSize; }
	
	LoggerType&					Get					()							{ return m_logger; }
	static Logger&				GetLogger			();
};



} // bv



#pragma warning( default : 4512 )
#pragma warning( default : 4100 )

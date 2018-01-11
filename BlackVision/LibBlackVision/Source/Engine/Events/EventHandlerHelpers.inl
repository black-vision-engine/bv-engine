#include "Engine/Events/EventHandlerHelpers.h"

#include "Serialization/ISerializer.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Events/Events.h"


namespace bv
{

// ***********************
//
template< typename ParamTypePtr >
Json::Value             GetParamDescription         ( model::IParameterPtr p )
{
    string s_name = p->GetName();
    auto paramType = p->GetType();

    Json::Value entry;

    entry[ "name" ] = s_name;
    entry[ "type" ] = Convert::T2String( paramType );

    Json::Value jsonKeys;

    auto typedParam = QueryTypedParam< ParamTypePtr >( p );
    auto accessIntepolator = typedParam->AccessInterpolator();
    auto keys = accessIntepolator.AccessKeys();
    for( auto & k : keys )
    {
        jsonKeys.append( toString( k.t ) );
        jsonKeys.append( toString( k.val ) );
    }

    entry[ "keys" ] = jsonKeys;

    return entry;
}

// ***********************
//
const static std::string EVENT_ID_TYPE_STRING       = "EventID";
const static std::string COMMAND_TYPE_STRING        = "cmd";
const static std::string COMMAND_SUCCESS_STRING     = "Success";
const static std::string ERROR_INFO_STRING          = "ErrorInfo";
const static std::string TRUE_STRING                = "true";
const static std::string FALSE_STRING               = "false";
const static int         NO_RESPONSE_EVENT_ID       = -1;

// ***********************
//
inline void             SendResponse                ( JsonSerializeObject & ser, int socketID, int eventID )
{
    //Do not respond for requests with eventID == NO_RESPONSE_EVENT_ID
    if( eventID != NO_RESPONSE_EVENT_ID )
    {
        ResponseEventPtr msg = std::make_shared< ResponseEvent >();
        msg->Response = ser.GetString();
        msg->SocketID = socketID;
        GetDefaultEventManager().QueueResponse( msg );
    }
}

// ***********************
//
template< typename CommandType >
inline void             PrepareResponseTemplate     ( ISerializer & ser, const CommandType & commandType, Int32 eventID, bool success )
{
    PrepareResponseTemplate( ser, Convert::T2String( commandType ), eventID, success );
}

// ***********************
//
template<>
inline void             PrepareResponseTemplate< std::string >  ( ISerializer & ser, const std::string & commandName, Int32 eventID, bool success )
{
    ser.SetAttribute( EVENT_ID_TYPE_STRING, Convert::T2String( eventID ) );
    ser.SetAttribute( COMMAND_TYPE_STRING, commandName );
    if( success )
    {
        ser.SetAttribute( COMMAND_SUCCESS_STRING, TRUE_STRING );
    }
    else
    {
        ser.SetAttribute( COMMAND_SUCCESS_STRING, FALSE_STRING );
    }
}

// ***********************
//
template< typename CommandType >
inline void             ErrorResponseTemplate       ( ISerializer & ser, CommandType commandType, int eventID, const char * errorString )
{
    PrepareResponseTemplate( ser, commandType, eventID, false );
    ser.SetAttribute( ERROR_INFO_STRING, errorString );
}

// ***********************
//
template<>
inline void             ErrorResponseTemplate< std::string >     ( ISerializer & ser, std::string commandType, int eventID, const char * errorString )
{
    PrepareResponseTemplate( ser, commandType, eventID, false );
    ser.SetAttribute( ERROR_INFO_STRING, errorString );
}

// ***********************
//
template< typename CommandType >
inline void             SendSimpleResponse          ( CommandType commandType, int eventID, int socketID, bool success )
{
    SimpleJsonSerializeObject ser;
    PrepareResponseTemplate( ser, commandType, eventID, success );

    SendResponse( ser, socketID, eventID );
}

// ***********************
//
template< typename CommandType >
inline void             SendSimpleErrorResponse     ( CommandType commandType, int eventID, int socketID, const char * errorString )
{
    SimpleJsonSerializeObject ser;
    ErrorResponseTemplate( ser, commandType, eventID, errorString );

    SendResponse( ser, socketID, eventID );
}

} //bv
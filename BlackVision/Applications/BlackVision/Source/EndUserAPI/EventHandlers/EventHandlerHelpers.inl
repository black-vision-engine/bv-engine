#include "EventHandlerHelpers.h"

#include "Serialization/ISerializer.h"

namespace bv
{

// ***********************
//
template< typename ParamTypePtr >
Json::Value GetParamDescription( model::IParameterPtr p )
{
    string s_name = p->GetName();
    auto paramType = p->GetType();

    Json::Value entry;

    entry[ "name" ] = s_name;
    entry[ "type" ] = ParamTypeToString( paramType );

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


const static std::string EVENT_ID_TYPE_STRING       = "EventID";
const static std::string COMMAND_TYPE_STRING        = "cmd";
const static std::string COMMAND_SUCCESS_STRING     = "Success";
const static std::string ERROR_INFO_STRING          = "ErrorInfo";
const static std::string TRUE_STRING                = "true";
const static std::string FALSE_STRING               = "false";

// ***********************
//
template< typename CommandType >
inline void PrepareResponseTemplate( ISerializer& ser, CommandType commandType, unsigned int eventID, bool success )
{
    ser.SetAttribute( EVENT_ID_TYPE_STRING, toString( eventID ) );
    ser.SetAttribute( COMMAND_TYPE_STRING, toString( SerializationHelper::T2WString( commandType ) ) );
    if( success )
        ser.SetAttribute( COMMAND_SUCCESS_STRING, TRUE_STRING );
    else
        ser.SetAttribute( COMMAND_SUCCESS_STRING, FALSE_STRING );
}

// ***********************
//
template< typename CommandType >
inline void ErrorResponseTemplate( ISerializer& ser, CommandType commandType, unsigned int eventID, const char* errorString )
{
    PrepareResponseTemplate( ser, commandType, eventID, false );
    ser.SetAttribute( ERROR_INFO_STRING, errorString );
}

// ***********************
//
template< typename CommandType >
inline void SendSimpleResponse( CommandType commandType, unsigned int eventID, int socketID, bool success )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, commandType, eventID, success );

    ResponseEventPtr msg = std::make_shared<ResponseEvent>();
    msg->Response = toWString( ser.GetString() );
    msg->SocketID = socketID;
    GetDefaultEventManager().QueueResponse( msg );
}

// ***********************
//
template< typename CommandType >
inline void SendSimpleErrorResponse( CommandType commandType, unsigned int eventID, int socketID, const char* errorString )
{
    JsonSerializeObject ser;
    ErrorResponseTemplate( ser, commandType, eventID, errorString );

    ResponseEventPtr msg = std::make_shared<ResponseEvent>();
    msg->Response = toWString( ser.GetString() );
    msg->SocketID = socketID;
    GetDefaultEventManager().QueueResponse( msg );
}

} //bv
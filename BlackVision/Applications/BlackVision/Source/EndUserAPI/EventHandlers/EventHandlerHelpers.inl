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

const static std::string COMMAND_TYPE_STRING        = "cmd";
const static std::string COMMAND_SUCCESS_STRING     = "CommandSuccess";
const static std::string TRUE_STRING                = "true";
const static std::string FALSE_STRING               = "false";

// ***********************
//
template< typename CommandType >
void PrepareResponseTemplate( ISerializer& ser, CommandType commandType, bool success )
{
    ser.SetAttribute( COMMAND_TYPE_STRING, toString( SerializationHelper::T2WString( commandType ) ) );
    if( success )
        ser.SetAttribute( COMMAND_SUCCESS_STRING, TRUE_STRING );
    else
        ser.SetAttribute( COMMAND_SUCCESS_STRING, FALSE_STRING );
}

} //bv
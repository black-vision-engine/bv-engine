#include "stdafx.h"

#include "SerializationException.h"
#include "Serialization/SerializationHelper.h"


namespace bv
{

ExceptionType        SerializationException::type = Exception::RegisterType();

// ***********************
//
std::string         SerializationException::GetReason   ()
{
    return PrintSceneName() + PrintLineNumber() + RuntimeException::GetReason();
}

// ***********************
//
std::string         SerializationException::PrintLineNumber     () const
{
    // Note: Lines are numbered beginning from 1. If we got 0 here,
    // position information is useless and we can ommit it.
    if( m_filePosition.Line != 0 )
        return "Line: [" + SerializationHelper::T2String( m_filePosition.Line ) + "] Column: [" + SerializationHelper::T2String( m_filePosition.CharPosition ) + "]. ";
    return std::string();
}

// ***********************
//
std::string         SerializationException::PrintSceneName      () const
{
    if( !m_sceneName.empty() )
        return "Deserializing scene [" + m_sceneName + "]. ";

    return std::string();
}

}	// bv

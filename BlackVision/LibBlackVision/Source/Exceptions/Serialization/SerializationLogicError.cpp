#include "stdafx.h"

#include "SerializationLogicError.h"


namespace bv
{

ExceptionType        SerializationLogicError::type = Exception::RegisterType();


// ***********************
//
std::string         SerializationLogicError::GetReason                  () const
{
    return SerializationException::GetReason() + PrintIncorrectImplMessage();
}

// ***********************
//
std::string         SerializationLogicError::PrintIncorrectImplMessage  () const
{
    return "This is unexpected behavior. Check your code for correctness.";
}

}	// bv

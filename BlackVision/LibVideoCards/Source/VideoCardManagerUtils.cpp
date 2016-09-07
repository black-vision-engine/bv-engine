#include "VideoCardManagerUtils.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"


namespace bv { namespace videocards {
  
//**************************************
//
std::pair< IOType, const char * > IOTypeMapping[] = 
{   std::make_pair( IOType::FILL_KEY, "FILL_KEY" )
    , std::make_pair( IOType::FILL, "FILL" )
    , std::make_pair( IOType::KEY, "KEY" )
};

//**************************************
//
std::pair< ReferenceMode, const char * > ReferenceModeMapping[] = 
{   std::make_pair( ReferenceMode::FREERUN, "FREERUN" )
    , std::make_pair( ReferenceMode::IN_A, "IN_A" )
    , std::make_pair( ReferenceMode::IN_B, "IN_B" )
    , std::make_pair( ReferenceMode::ANALOG, "ANALOG" )
    , std::make_pair( ReferenceMode::GENLOCK, "GENLOCK" )
};

} //videocards


namespace SerializationHelper {

//**************************************
//
template<> Expected< videocards::IOType >           String2T        ( const std::string & s )
{ 
    return String2Enum( videocards::IOTypeMapping, s ); 
}

//**************************************
//
template<> std::string                              T2String        ( const videocards::IOType & t )
{
    return Enum2String( videocards::IOTypeMapping, t );
}

//**************************************
//
template<> Expected< videocards::ReferenceMode >    String2T        ( const std::string & s )
{
    return String2Enum( videocards::ReferenceModeMapping, s );
}

//**************************************
//
template<> std::string                              T2String        ( const videocards::ReferenceMode & t )
{
    return Enum2String( videocards::ReferenceModeMapping, t );
}

} //SerializationHelper

} //bv
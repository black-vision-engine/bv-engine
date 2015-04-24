#include "ValuesFactory.h"


namespace bv {

// *******************************
//
ValueIntPtr         ValuesFactory::CreateValueInt    ( const std::string & name )
{
    return std::make_shared< ValueInt >( name );
}

// *******************************
//
ValueFloatPtr         ValuesFactory::CreateValueFloat    ( const std::string & name )
{
    return std::make_shared< ValueFloat >( name );
}

// *******************************
//
ValueVec2Ptr          ValuesFactory::CreateValueVec2     ( const std::string & name )
{
    return std::make_shared< ValueVec2 >( name );
}

// *******************************
//
ValueVec3Ptr          ValuesFactory::CreateValueVec3     ( const std::string & name )
{
    return std::make_shared< ValueVec3 >( name );
}

// *******************************
//
ValueVec4Ptr          ValuesFactory::CreateValueVec4     ( const std::string & name )
{
    return std::make_shared< ValueVec4 >( name );
}

// *******************************
//
ValueMat2Ptr          ValuesFactory::CreateValueMat2     ( const std::string & name )
{
    return std::make_shared< ValueMat2 >( name );
}

// *******************************
//
ValueMat4Ptr          ValuesFactory::CreateValueMat4     ( const std::string & name )
{
    return std::make_shared< ValueMat4 >( name );
}


} // bv

#include "ValuesFactory.h"


namespace bv {

// *******************************
//
ValueFloat *         ValuesFactory::CreateValueFloat    ( const std::string & name )
{
    return new ValueFloat( name );
}

// *******************************
//
ValueVec3 *          ValuesFactory::CreateValueVec3     ( const std::string & name )
{
    return new ValueVec3( name );
}

// *******************************
//
ValueVec4 *          ValuesFactory::CreateValueVec4     ( const std::string & name )
{
    return new ValueVec4( name );
}

// *******************************
//
ValueMat2 *          ValuesFactory::CreateValueMat2     ( const std::string & name )
{
    return new ValueMat2( name );
}

// *******************************
//
ValueMat4 *          ValuesFactory::CreateValueMat4     ( const std::string & name )
{
    return new ValueMat4( name );
}

// *******************************
//
ValueFloatPtr        ValuesFactory::CreateValueFloatPtr ( const std::string & name )
{
    return ValueFloatPtr( CreateValueFloat( name ) );
}

// *******************************
//
ValueVec3Ptr         ValuesFactory::CreateValueVec3Ptr  ( const std::string & name )
{
    return ValueVec3Ptr( CreateValueVec3( name ) );
}

// *******************************
//
ValueVec4Ptr         ValuesFactory::CreateValueVec4Ptr  ( const std::string & name )
{
    return ValueVec4Ptr( CreateValueVec4( name ) );
}

// *******************************
//
ValueMat2Ptr         ValuesFactory::CreateValueMat2Ptr  ( const std::string & name )
{
    return ValueMat2Ptr( CreateValueMat2( name ) );
}

// *******************************
//
ValueMat4Ptr         ValuesFactory::CreateValueMat4Ptr  ( const std::string & name )
{
    return ValueMat4Ptr( CreateValueMat4( name ) );
}

} // bv

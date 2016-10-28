#include "stdafx.h"

#include "ValuesFactory.h"


namespace bv {

// ***********************
//
ValueBoolPtr         ValuesFactory::CreateValueBool     ( const std::string & name )
{
    return std::make_shared< ValueBool >( name );
}

// ***********************
//
ValueBoolPtr         ValuesFactory::CreateValueBool     ( const std::string & name, bool initVal )
{
    auto val = CreateValueBool( name );

    val->SetValue( initVal );

    return val;
}

// *******************************
//
ValueIntPtr         ValuesFactory::CreateValueInt    ( const std::string & name )
{
    return std::make_shared< ValueInt >( name );
}

// *******************************
//
ValueIntPtr         ValuesFactory::CreateValueInt    ( const std::string & name, Int32 initVal )
{
    auto val = CreateValueInt( name );

    val->SetValue( initVal );

    return val;
}

// *******************************
//
ValueFloatPtr         ValuesFactory::CreateValueFloat    ( const std::string & name )
{
   return std::make_shared< ValueFloat >( name );
}

// *******************************
//
ValueFloatPtr         ValuesFactory::CreateValueFloat    ( const std::string & name, Float32 initVal )
{
    auto val = CreateValueFloat( name );

    val ->SetValue( initVal );

    return val;
}

// *******************************
//
ValueDoublePtr          ValuesFactory::CreateValueDouble    ( const std::string & name )
{
    return std::make_shared< ValueDouble >( name );
}

// *******************************
//
ValueDoublePtr          ValuesFactory::CreateValueDouble    ( const std::string & name, Float64 initVal )
{
    auto val = CreateValueDouble( name );

    val->SetValue( initVal );

    return val;
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

// *******************************
//
ValueStringPtr       ValuesFactory::CreateValueString    ( const std::string & name )
{
    return std::make_shared< ValueString >( name );
}

// *******************************
//
ValueStringPtr         ValuesFactory::CreateValueString  ( const std::string & name, const std::string & initVal )
{
    auto val = CreateValueString( name );

    val->SetValue( initVal );

    return val;
}

// *******************************
//
ValueWStringPtr      ValuesFactory::CreateValueWString   ( const std::string & name )
{
    return std::make_shared< ValueWString >( name );
}

// *******************************
//
ValueWStringPtr      ValuesFactory::CreateValueWString( const std::string & name, const std::wstring & initVal )
{
    auto val = CreateValueWString( name );

    val->SetValue( initVal );

    return val;
}

} // bv

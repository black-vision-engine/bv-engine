#pragma once

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class ValuesFactory
{
public:

    static ValueBoolPtr         CreateValueBool     ( const std::string & name );
    static ValueBoolPtr         CreateValueBool     ( const std::string & name, bool initVal );
    static ValueIntPtr          CreateValueInt      ( const std::string & name );
    static ValueIntPtr          CreateValueInt      ( const std::string & name, Int32 initVal );
    static ValueFloatPtr        CreateValueFloat    ( const std::string & name );
    static ValueFloatPtr        CreateValueFloat    ( const std::string & name, Float32 initVal );
    static ValueDoublePtr       CreateValueDouble   ( const std::string & name );
    static ValueDoublePtr       CreateValueDouble   ( const std::string & name, Float64 initVal );
    static ValueVec2Ptr         CreateValueVec2     ( const std::string & name );
    static ValueVec3Ptr         CreateValueVec3     ( const std::string & name );
    static ValueVec4Ptr         CreateValueVec4     ( const std::string & name );
	static ValueVec4Ptr         CreateValueVec4     ( const std::string & name, const glm::vec4 & initVal );
    static ValueMat2Ptr         CreateValueMat2     ( const std::string & name );
    static ValueMat4Ptr         CreateValueMat4     ( const std::string & name ); 
    static ValueStringPtr       CreateValueString   ( const std::string & name );
    static ValueStringPtr       CreateValueString   ( const std::string & name, const std::string & initVal );
    static ValueWStringPtr      CreateValueWString  ( const std::string & name );
    static ValueWStringPtr      CreateValueWString  ( const std::string & name, const std::wstring & initVal );

    template< typename ValueType >
    static std::shared_ptr< typename ValueT< ValueType >::Type > CreateValue( const std::string & name, ValueType initVal = ValueType() )
    {
        auto val = std::make_shared< typename ValueT< ValueType >::Type >( name );
        val->SetValue( initVal );
        return val;
    }
};

} // bv

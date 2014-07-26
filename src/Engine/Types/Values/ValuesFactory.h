#pragma once

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class ValuesFactory
{
public:

    static ValueFloatPtr        CreateValueFloat    ( const std::string & name );
    static ValueVec2Ptr         CreateValueVec2     ( const std::string & name );
    static ValueVec3Ptr         CreateValueVec3     ( const std::string & name );
    static ValueVec4Ptr         CreateValueVec4     ( const std::string & name );
    static ValueMat2Ptr         CreateValueMat2     ( const std::string & name );
    static ValueMat4Ptr         CreateValueMat4     ( const std::string & name ); 
};

} // bv

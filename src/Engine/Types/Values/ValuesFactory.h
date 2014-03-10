#pragma once

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class ValuesFactory
{
public:

    static ValueFloat *         CreateValueFloat    ( const std::string & name );
    static ValueVec3 *          CreateValueVec3     ( const std::string & name );
    static ValueVec4 *          CreateValueVec4     ( const std::string & name );
    static ValueMat2 *          CreateValueMat2     ( const std::string & name );
    static ValueMat4 *          CreateValueMat4     ( const std::string & name ); 

    static ValueFloatPtr        CreateValueFloatPtr ( const std::string & name );
    static ValueVec3Ptr         CreateValueVec3Ptr  ( const std::string & name );
    static ValueVec4Ptr         CreateValueVec4Ptr  ( const std::string & name );
    static ValueMat2Ptr         CreateValueMat2Ptr  ( const std::string & name );
    static ValueMat4Ptr         CreateValueMat4Ptr  ( const std::string & name ); 

};

} // bv

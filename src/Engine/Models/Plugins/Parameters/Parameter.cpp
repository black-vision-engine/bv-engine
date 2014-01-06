#include "Parameter.h"


namespace bv { namespace model {

// *******************************
// 
ValueFloat::ValueFloat( const std::string & name )
    : BaseValue( name )
{
}

// *******************************
//
ValueVec4::ValueVec4( const std::string & name)
    : BaseValue( name )
{
}

// *******************************
//
ValueMat4::ValueMat4( const std::string & name )
    : BaseValue( name )
{
}
                                                 
} // model
} // bv

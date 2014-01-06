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

// *******************************
//                                               
float           ParamFloat::Evaluate        ( TimeType t )   const
{                                                
    t = BaseParameter::GetEvaluationTime( t );
    SetLastEvaluatedTime( t );
    return m_value.evaluate( t );
}                                                
                                                 
// *******************************               
//                                               
glm::vec4       ParamVec4::Evaluate         ( TimeType t )    const
{
    t = BaseParameter::GetEvaluationTime( t );
    SetLastEvaluatedTime( t );
    return m_value.evaluate( t );
}

// *******************************               
//  
glm::vec3       ParamVec3::Evaluate         ( TimeType t )   const
{
    t = BaseParameter::GetEvaluationTime( t );
    SetLastEvaluatedTime( t );
    return m_value.evaluate( t );
}

// *******************************
//
glm::mat2           ParamMat2::Evaluate ( TimeType t )   const
{
    t = BaseParameter::GetEvaluationTime( t );

    SetLastEvaluatedTime( t );
    glm::vec4 v = m_value.evaluate( t );

    return glm::mat2( v[ 0 ], v[ 1 ], v[ 2 ], v [ 3 ] );
}

} // model
} // bv

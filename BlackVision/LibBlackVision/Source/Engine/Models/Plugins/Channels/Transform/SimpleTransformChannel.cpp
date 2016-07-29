#include "stdafx.h"

#include "SimpleTransformChannel.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model
{

// ******************************
//
SimpleTransformChannel::SimpleTransformChannel  ( const ParamTransform & paramTransform )
    : m_transformParam( paramTransform )
{
    m_transformation = std::make_shared< Transform >();
}

// ******************************
//
void                                    SimpleTransformChannel::Update( TimeType t )
{
    { t; } // FIXME: suppress unused variable

    m_transformation->SetMatrix( m_transformParam.Evaluate() );
}

// ******************************
//
ParamTransform *                        SimpleTransformChannel::GetParamTransform    ()
{
    return &m_transformParam;
}

} // model
} // bv

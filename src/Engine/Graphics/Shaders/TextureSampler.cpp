#include "TextureSampler.h"

namespace bv {

// ******************************
//
TextureSampler::TextureSampler  ( int id, const std::string & name,  SamplerSamplingMode ssm, SamplerFilteringMode sfm, SamplerWrappingMode swm, const glm::vec4 & borderColor )
    : m_id( id )
    , m_name( name )
    , m_samplingMode( ssm )
    , m_filteringMode( sfm )
    , m_wrappingMode( swm )
    , m_borderColor( borderColor )
{
}

// ******************************
//
TextureSampler::~TextureSampler ()
{
}

// ******************************
//
int                     TextureSampler::GetId           () const
{
    return m_id;
}

// ******************************
//
const std::string &     TextureSampler::GetName         () const
{
    return m_name;
}

// ******************************
//
SamplerSamplingMode     TextureSampler::SamplingMode    () const
{
    return m_samplingMode;
}

// ******************************
//
SamplerFilteringMode    TextureSampler::FilteringMode   () const
{
    return m_filteringMode;
}

// ******************************
//
SamplerWrappingMode     TextureSampler::WrappingMode    () const
{
    return m_wrappingMode;
}

// ******************************
//
const glm::vec4 &       TextureSampler::GetBorderColor  () const
{
    return m_borderColor;
}

}

#include "TextureSampler.h"


namespace bv {

// ******************************
//
TextureSampler::TextureSampler  ( int id, const std::string & name,  SamplerSamplingMode ssm )
    : m_id( id )
    , m_name( name )
    , m_samplingMode( ssm )
{
	//Set default values - they will be overwritten in Shader
    for( int i = 0; i < (int) SamplerWrapDirection::SWD_TOTAL; ++i )
	{
        m_wrappingMode[ i ] = SamplerWrappingMode::SWM_CLAMP_BORDER;
	}
	m_filteringMode = SamplerFilteringMode::SFM_LINEAR;
	m_borderColor = glm::vec4( 0.f, 0.f, 0.f, 0.f );
}

// ******************************
//
TextureSampler::TextureSampler  ( int id, const std::string & name,  SamplerSamplingMode ssm, SamplerFilteringMode sfm, 
                                  SamplerWrappingMode swm[ (int) SamplerWrapDirection::SWD_TOTAL ], const glm::vec4 & borderColor )
    : m_id( id )
    , m_name( name )
    , m_samplingMode( ssm )
    , m_filteringMode( sfm )
    , m_borderColor( borderColor )
{
    for( int i = 0; i < (int) SamplerWrapDirection::SWD_TOTAL; ++i )
    {
        m_wrappingMode[ i ] = swm[ i ];
    }
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
SamplerWrappingMode     TextureSampler::WrappingMode    ( SamplerWrapDirection direction ) const
{
    return m_wrappingMode[ (int) direction ];
}

// ******************************
//
const glm::vec4 &       TextureSampler::GetBorderColor  () const
{
    return m_borderColor;
}

// ******************************
//
void                    TextureSampler::SetBorderColor  ( const glm::vec4 & bc )
{
    m_borderColor = bc;
}

// ******************************
//
void					TextureSampler::SetWrappingMode ( SamplerWrappingMode mode, SamplerWrapDirection wrap_direction )
{
	m_wrappingMode[ (int) wrap_direction ] = mode;
}

// ******************************
//
void                    TextureSampler::SetFilteringMode  ( SamplerFilteringMode mode )
{
	m_filteringMode = mode;
}

}

#pragma once

#include <string>

#include "glm/glm.hpp"

namespace bv {

enum class SamplerSamplingMode : int
{
    SSM_MODE_1D = 0,
    SSM_MODE_2D,
    SSM_MODE_3D,
    SSM_MODE_CUBIC,

    ST_TOTAL
};

//FIXME: add more filtering opitions when 
enum class SamplerFilteringMode : int 
{
    SFM_POINT = 0,
    SFM_LINEAR,

    //FIXME: point_point, point_linear, linear_point, linear_linear -> add when mipmapping is more or less implemented 
    SFM_TOTAL
};

enum class SamplerWrappingMode : int
{
    SWM_CLAMP,
    SWM_REPEAT,
    SWM_MIRROR,
    SWM_CLAMP_BORDER,
    SWM_CLAMP_EDGE, 

    SWM_TOTAL
};

class TextureSampler
{
private:

    int                     m_id;
    std::string             m_name;
    
    SamplerSamplingMode     m_samplingMode;
    SamplerFilteringMode    m_filteringMode;
    SamplerWrappingMode     m_wrappingMode;

    glm::vec4               m_borderColor;

public:

                            TextureSampler  ( int id, const std::string & name, SamplerSamplingMode ssm, SamplerFilteringMode sfm, SamplerWrappingMode swm, const glm::vec4 & borderColor );
                            ~TextureSampler ();

    int                     GetId           () const;
    const std::string &     GetName         () const;

    SamplerSamplingMode     SamplingMode    () const;
    SamplerFilteringMode    FilteringMode   () const;
    SamplerWrappingMode     WrappingMode    () const;

    const glm::vec4 &       GetBorderColor  () const;

    //FIXME: anisotropy
    //FIXME: LOD bias when LOD mipmapping is implemented

    //FIXME: glm::vec4 m_borderColor;
};

} //bv

#pragma once

#include <string>

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

enum SamplerWrappingMode
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

    //FIXME: anisotropy
    //FIXME: LOD bias when LOD mipmapping is implemented

    //FIXME: glm::vec4 m_borderColor;
};

}

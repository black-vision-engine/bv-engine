#pragma once

#include <string>
#include <vector>

#include "Mathematics/glm_inc.h"


namespace bv {

enum class SamplerSamplingMode : int
{
    SSM_MODE_1D = 0,
    SSM_MODE_2D,
    SSM_MODE_3D,
    SSM_MODE_CUBIC,

    ST_TOTAL
};

enum class SamplerFilteringMode : int 
{
    SFM_POINT = 0,
    SFM_LINEAR,
	SFM_POINT_MIPMAP_POINT,
	SFM_LINEAR_MIPMAP_POINT,
	SFM_POINT_MIPMAP_LINEAR,
	SFM_LINEAR_MIPMAP_LINEAR,

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

enum class SamplerWrapDirection : int 
{
    SWD_S = 0,
    SWD_T,
    SWD_R,

    SWD_TOTAL
};

class TextureSampler
{
private:

    int                     m_id;
    std::string             m_name;
    
    SamplerSamplingMode     m_samplingMode;
    SamplerFilteringMode    m_filteringMode;
    SamplerWrappingMode     m_wrappingMode[ (int) SamplerWrapDirection::SWD_TOTAL ];

    glm::vec4               m_borderColor;

    mutable int             m_loc;

public:

                            //FIXME: instead of passing whole array of wraping modes, it should be accessible via accessors (getters and setters)
							
							TextureSampler  ( int id, const std::string & name, SamplerSamplingMode ssm );
							TextureSampler  ( int id, const std::string & name, SamplerSamplingMode ssm, SamplerFilteringMode sfm, 
                                              SamplerWrappingMode swm[ (int) SamplerWrapDirection::SWD_TOTAL ], const glm::vec4 & borderColor );
                            ~TextureSampler ();

    int                     GetId           () const;
    const std::string &     GetName         () const;

    SamplerSamplingMode     SamplingMode    () const;
    SamplerFilteringMode    FilteringMode   () const;
    SamplerWrappingMode     WrappingMode    ( SamplerWrapDirection direction ) const;
    const glm::vec4 &       GetBorderColor  () const;

    void                    SetBorderColor  ( const glm::vec4 & );
	void					SetWrappingMode ( SamplerWrappingMode mode, SamplerWrapDirection wrap_direction );
	void					SetFilteringMode( SamplerFilteringMode mode );

    void                    CacheLoc        ( int loc ) const;
    int                     GetCachedLoc    () const;

    //FIXME: anisotropy
    //FIXME: LOD bias when LOD mipmapping is implemented

    //FIXME: glm::vec4 m_borderColor;
};

typedef std::vector< TextureSampler * >         TTextureSamplerVec;
typedef std::vector< const TextureSampler * >   TConstTextureSamplerVec;

} //bv

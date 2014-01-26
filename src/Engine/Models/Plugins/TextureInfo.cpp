#include "TextureInfo.h"

namespace bv { namespace model {

TextureInfo::TextureInfo(   const ResourceHandle * resHandle
                         ,  const std::string & texName
                         ,  const ParamTransform & texTransform
                         ,  const ParamFloat& alpha
                         ,  const ParamVec4& borderColor
                         ,  TextureWrappingMode wrappingModeX
                         ,  TextureWrappingMode wrappingModeY 
                         ,  TextureFilteringMode filteringMode )
    : m_resHandle( resHandle )
    , m_texName( texName )
    , m_texTransform( texTransform )
    , m_texAlpha( alpha )
    , m_texBorderColor( borderColor )
    , m_wrappingModeX( wrappingModeX )
    , m_wrappingModeY( wrappingModeY )
    , m_finteringMode( filteringMode )
{
    m_texTransformVal   = ValueMat4Ptr( new ValueMat4( texTransform.GetName() ) );
    m_texAlphaVal       = ValueFloatPtr( new ValueFloat( alpha.GetName() ) );
    m_texBorderColorVal = ValueVec4Ptr( new ValueVec4( borderColor.GetName() ) );
}

} // model
} // bv
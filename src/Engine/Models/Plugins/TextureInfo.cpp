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
    m_texTransformVal   = ValuesFactory::CreateValueMat4Ptr( texTransform.GetName() );
    m_texAlphaVal       = ValuesFactory::CreateValueFloatPtr( alpha.GetName() );
    m_texBorderColorVal = ValuesFactory::CreateValueVec4Ptr( borderColor.GetName() );
}

} // model
} // bv

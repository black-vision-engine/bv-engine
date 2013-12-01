#include "TextureInfo.h"

namespace bv { namespace model {

TextureInfo::TextureInfo( ResourceHandle * resHandle, const std::string & texName, const ParamTransform & texTransform, TextureWrappingMode wrappingModeX, TextureWrappingMode wrappingModeY )
    : m_resHandle( resHandle )
    , m_texName( texName )
    , m_texTransform( texTransform )
    , m_wrappingModeX( wrappingModeX )
    , m_wrappingModeY( wrappingModeY )
{
    m_texTransformVal = new ValueMat4( texTransform.GetName() );
}

} // model
} // bv
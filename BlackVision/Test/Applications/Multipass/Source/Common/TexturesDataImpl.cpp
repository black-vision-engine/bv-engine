#include "TexturesDataImpl.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Models/Interfaces/IAnimationDescriptor.h"


namespace bv {

// **************************
//
TexturesDataImpl::~TexturesDataImpl   ()
{
    for( auto desc : m_textures )
    {
        delete desc;
    }

    for( auto anim : m_animations )
    {
        delete anim;
    }
}

// **************************
//
const std::vector< ITextureDescriptor * > &     TexturesDataImpl::GetTextures       () const
{
    return m_textures;
}

// **************************
//
const std::vector< IAnimationDescriptor * > &   TexturesDataImpl::GetAnimations     () const
{
    return m_animations;
}

// **************************
//
bool                                            TexturesDataImpl::AddTextureFromFile( const std::string & textureFile, const std::string & textureName )
{
	auto textureResDesc = TextureAssetDesc::Create( textureFile, true );
    auto desc = model::DefaultTextureDescriptor::LoadTexture( textureResDesc, textureName );
    
    if( desc )
    {
        m_textures.push_back( desc );

        desc->SetFilteringMode( TextureFilteringMode::TFM_LINEAR );
        desc->SetWrappingModeX( TextureWrappingMode::TWM_MIRROR );
        desc->SetWrappingModeY( TextureWrappingMode::TWM_MIRROR );
        desc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );

        return true;
    }

    return false;
}

} // bv

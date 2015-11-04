#include "TexturesDataImpl.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Models/Interfaces/IAnimationDescriptor.h"


namespace bv {

// **************************
//
TexturesDataImpl::~TexturesDataImpl   ()
{
}

// **************************
//
const std::vector< ITextureDescriptorPtr > &     TexturesDataImpl::GetTextures       () const
{
    return m_textures;
}

// **************************
//
const std::vector< IAnimationDescriptorPtr > &   TexturesDataImpl::GetAnimations     () const
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

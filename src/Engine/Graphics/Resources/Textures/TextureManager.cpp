#include "TextureManager.h"

#include <iostream>

#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"

#include "Engine/Models/Resources/Resource.h"
#include "Engine/Models/Resources/TextureLoader.h"

#include <cassert>

namespace bv
{

TextureManager GTextureManager;

// *********************************
//
TextureManager::TextureManager()
{
}

// *********************************
//
TextureManager::~TextureManager()
{
    for( auto ptrTx : m_txMap )
    {
        delete ptrTx.second;
    }
}

// *********************************
//
Texture2D * TextureManager::LoadTexture( const model::ResourceHandle * resHandle, bool loadFromMemory )
{
    auto extraKind = resHandle->GetExtra()->GetResourceExtraKind();

    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( resHandle->GetExtra() );

    assert( texExtra->GetType() == TextureType::T_2D );

    auto format = texExtra->GetFormat();
    auto width  = texExtra->GetWidth();
    auto height = texExtra->GetHeight();

    //FIXME: add asserts for invalid dimensions (i.e. too large)
    auto newTex = new bv::Texture2DImpl( format, width, height );
    newTex->SetRawData( resHandle->GetData(), format, width, height );
    m_txMap[ newTex ] = newTex;

    return newTex;
}

// *********************************
//
void TextureManager::FreeTexture( Texture * tx )
{
    auto it = m_txMap.find( tx );

    if( it != m_txMap.end() )
    {
        delete it->second;
        m_txMap[ tx ] = nullptr;
    }
}

// *********************************
//
bool    TextureManager::AddFrame    ( Texture2DSequenceImpl * anim, const model::ResourceHandle * resHandle, bool loadFromMemory )
{
    auto extraKind = resHandle->GetExtra()->GetResourceExtraKind();
    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( resHandle->GetExtra() );
    assert( texExtra->GetType() == TextureType::T_2D );

    return anim->AddTextureSettingRawData( resHandle->GetData(), texExtra->GetFormat(), texExtra->GetWidth(), texExtra->GetHeight() );    
}

// *********************************
//
Texture2DSequenceImpl * TextureManager::CreateEmpty2DSequence   ( const model::ResourceHandle * resHandle )
{
    auto extraKind = resHandle->GetExtra()->GetResourceExtraKind();
    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( resHandle->GetExtra() );
    assert( texExtra->GetType() == TextureType::T_2D );

    auto sequence = new Texture2DSequenceImpl( texExtra->GetFormat(), texExtra->GetWidth(), texExtra->GetHeight() );

    return sequence;
}

} // bv

#include "TextureManager.h"

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "Engine/Graphics/Resources/Texture2D.h"

#include "FreeImagePlus.h"

#include "Engine/Models/Resources/Resource.h"
#include "Engine/Models/Resources/TextureLoader.h"


using namespace boost::filesystem;

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

    auto texExtra = static_cast< const model::TextureExtraData* >( resHandle->GetExtra() );

    assert( texExtra->GetType() == TextureType::T_2D );

    bv::Texture2D * newTex = new bv::Texture2D( texExtra->GetFormat(), texExtra->GetWidth(), texExtra->GetHeight() );
    TextureAccessor::WriteData( newTex, resHandle->GetData(), resHandle->GetSize() );

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

} // bv

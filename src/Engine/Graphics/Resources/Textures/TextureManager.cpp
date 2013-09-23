#include "TextureManager.h"
#include "Texture2D.h"

#include "FreeImagePlus.h"

#include "Engine\Models\Resources\Resource.h"
#include "Engine\Models\Resources\TextureLoader.h"

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

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
Texture2D * TextureManager::LoadTexture( const std::string & filename, bool loadFromMemory )
{
    model::Resource res( "ladna nazwa", filename );

    model::TextureLoader loader;

    model::ResourceHandle* handle = loader.LoadResource( &res );

    bv::Texture2D * newTex = new bv::Texture2D( Texture::TFormat::F_A8R8G8B8, handle->GetWidth(), handle->GetHeight() );
    newTex->WriteToBuffer( handle->GetData(), handle->GetSize() );

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

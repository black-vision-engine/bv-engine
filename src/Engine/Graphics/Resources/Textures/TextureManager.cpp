#include "TextureManager.h"
#include "Texture.h"

#include "FreeImagePlus.h"

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
Texture * TextureManager::LoadTexture( const std::string & filename, bool loadFromMemory )
{
    boost::filesystem::path filepath( filename );
    std::string errMsg( "Cannot read file: " + filename ); 

    if( !exists( filepath ) )
    {
        throw std::runtime_error( errMsg );
        return nullptr;
    }

    fipImage*  fipImg = new fipImage();

    if( loadFromMemory )
    {
        boost::filesystem::ifstream file(filepath);

        uintmax_t size = file_size(filepath);

        char* bufToRead = new char[ ( unsigned int ) size ];
    
        file.read( bufToRead, size );

        fipMemoryIO fipIO( ( BYTE * ) bufToRead, ( DWORD ) size );

        FREE_IMAGE_FORMAT type = fipIO.getFileType();

        if( !fipImg->loadFromMemory( fipIO ) )
        {
            throw std::runtime_error( errMsg );
            return nullptr;
        }

        delete bufToRead;
    }
    else
    {
        if( !fipImg->load( filename.c_str() ) )
        {
            throw std::runtime_error( errMsg );
            return nullptr;
        }
    }

    Texture * newTex = new bv::Texture( fipImg );

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

#include "ModelTextureManager.h"

#include "Engine/Models/Resources/TextureLoader.h"

#include "System/FileIO.h"

namespace bv { namespace model {

//// *******************************
////
//TextureManager::TextureManager()
//{
//}
//
//// *******************************
////
//TextureManager::~TextureManager()
//{
//}
//
//// *******************************
////
//ResourceHandleConstPtr          TextureManager::GetTexture      ( const std::string & path )
//{
//    auto absPath = File::GetAbsolutPath( path );
//    auto it = this->m_textures.find( absPath );
//
//    if( it != this->m_textures.end() )
//        return it->second;
//    else
//    {
//        if( auto newTexture = this->LoadTexture( absPath ) )
//            return newTexture;
//        else
//            return nullptr;
//    }
//}
//
//// *******************************
////
//void                            TextureManager::ClearCache      ()
//{
//    auto it = this->m_textures.begin();
//    while( it != this->m_textures.end() )
//        if( it->second.unique() )
//            it = this->m_textures.erase( it );
//        else
//            ++it;
//}
//
//// *******************************
////
//void                            TextureManager::ReloadTexture   ( const std::string & path )
//{
//    this->LoadTexture( path );
//}
//
//// *******************************
////
//TextureManager &                TextureManager::Get             ()
//{
//    static auto instance = TextureManager();
//    return instance;
//}
//
//// *******************************
////
//ResourceHandleConstPtr          TextureManager::LoadTexture     (  const std::string & path )
//{
//    auto absPath = File::GetAbsolutPath( path );
//
//    auto it = this->m_textures.find( absPath );
//
//    if( it != this->m_textures.end() )
//        this->m_textures.erase( absPath );    
//
//    Resource texture( absPath, absPath );
//
//    TextureLoader texLoader;
//    auto handle = ResourceHandlePtr( texLoader.LoadResource( &texture ) );
//
//    if ( handle == nullptr )
//    {
//        return nullptr;
//    }
//    else
//    {
//        this->m_textures[ absPath ] = handle;
//        return handle;
//    }
//}

} // model
} // bv

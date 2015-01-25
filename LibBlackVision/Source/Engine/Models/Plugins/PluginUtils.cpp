#include "PluginUtils.h"

#include "Engine\Models\Resources\Texture\TextureResourceDescr.h"
#include "Engine\Models\Resources\Texture\AnimationResourceDescr.h"
#include "Engine\Models\Resources\Texture\FontResourceDescr.h"


namespace bv { namespace model {

// *******************************
//
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile )
{
    TextureResourceDescrPtr desc = std::make_shared< TextureResourceDescr >( textureFile );

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadAnimation   ( IPluginPtr plugin, const std::string & animationPath, const std::string & filter )
{
    AnimationResourceDescrPtr desc = AnimationResourceDescr::CreateFromDirFrames( animationPath, filter );

    if( desc == nullptr )
    {
        return false;
    }

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadFont        ( IPluginPtr plugin, const std::string & fontFile )
{
    FontResourceDescrPtr desc( new FontResourceDescr( fontFile ) );

    return plugin->LoadResource( desc );
}

} //model
} //bv

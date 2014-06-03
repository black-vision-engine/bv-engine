#include "PluginUtils.h"

#include "Engine\Models\Resources\Texture\TextureResourceDescr.h"
#include "Engine\Models\Resources\Texture\AnimationResourceDescr.h"
#include "Engine\Models\Resources\Texture\FontResourceDescr.h"


namespace bv { namespace model {

// *******************************
//
bool    LoadTexture     ( IPlugin * plugin, const std::string & textureFile )
{
    TextureResourceDescrPtr desc( new TextureResourceDescr( textureFile ) );

    return plugin->LoadResource( desc.get() );
}

// *******************************
//
bool    LoadAnimation   ( IPlugin * plugin, const std::string & animationPath, const std::string & filter )
{
    AnimationResourceDescrPtr desc( AnimationResourceDescr::CreateFromDirFrames( animationPath, filter ) );

    if( desc == nullptr )
    {
        return false;
    }

    return plugin->LoadResource( desc.get() );
}

// *******************************
//
bool    LoadFont        ( IPlugin * plugin, const std::string & fontFile )
{
    FontResourceDescrPtr desc( new FontResourceDescr( fontFile ) );

    return plugin->LoadResource( desc.get() );
}

} //model
} //bv

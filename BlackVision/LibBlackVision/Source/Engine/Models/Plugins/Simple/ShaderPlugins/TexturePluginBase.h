#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv {
namespace model
{





// ***************************** PLUGIN ********************************** 
class TexturePluginBase : public BasePlugin
{
protected:

    DefaultPixelShaderChannelPtr    m_psc;

public:

    explicit                                    TexturePluginBase           ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    virtual                                     ~TexturePluginBase          () {}



    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;
    virtual std::string                         GetTextureName              ( UInt32 idx = 0 ) const = 0;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;

};

} // model
} // bv

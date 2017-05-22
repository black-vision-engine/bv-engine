#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Assets/Mesh/MeshAsset.h"



namespace bv {
namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultSVGPluginDesc : public BasePluginDescriptor
{
public:

    DefaultSVGPluginDesc                                        ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();

    static  std::string                     AssetName           ();

};

// ***************************** PLUGIN ********************************** 
class DefaultSVGPlugin : public BasePlugin
{
protected:
    DefaultPixelShaderChannelPtr    m_psc;
    VertexAttributesChannelPtr      m_vaChannel;
    MeshAssetConstPtr               m_meshAsset;

public:

    explicit                                    DefaultSVGPlugin            ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    ~DefaultSVGPlugin          ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:

    bool										InitVertexAttributesChannel ( bool recursive );
    void										AddGeometry                 ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, bool recursive );

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;
};

} // model
} // bv


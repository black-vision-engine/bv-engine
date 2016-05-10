#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Assets/Mesh/MeshAsset.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultMeshPluginDesc : public BasePluginDescriptor
{
public:

    DefaultMeshPluginDesc                                       ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     MeshName            ();

};

// ***************************** PLUGIN ********************************** 
class DefaultMeshPlugin : public BasePlugin< IPlugin >
{
protected:

    DefaultPixelShaderChannelPtr    m_psc;

    VertexAttributesChannelPtr      m_vaChannel;
    
    MeshAssetConstPtr               m_meshAsset;

public:

    explicit                                    DefaultMeshPlugin           ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultMeshPlugin          ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:

    bool										InitVertexAttributesChannel ( bool recursive );

    void										AddGeometry                 ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, Float3AttributeChannelPtr normChannel, Float2AttributeChannelPtr uvChannel, bool recursive );

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;

};

} // model
} // bv
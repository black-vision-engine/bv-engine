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
/// @ingroup PluginsList
class DefaultMeshPlugin : public BasePlugin
{
public:

    struct PARAMS
    {
        static const std::string        FlipU;
        static const std::string        FlipV;
    };

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

protected:

    bool										InitVertexAttributesChannel ( bool recursive );

    void										AddGeometry                 ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, Float3AttributeChannelPtr normChannel, Float2AttributeChannelPtr uvChannel, Float4AttributeChannelPtr tangentChannel, glm::mat4 & transform, bool recursive );
    glm::mat4                                   ComputeTransform            ( MeshAssetConstPtr meshAsset );

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

    void                                        AddUVChannel                ( Float2AttributeChannelPtr & uvChannel, const std::vector< glm::vec2 > & uvs, bool flipU, bool flipV );
};

} // model
} // bv

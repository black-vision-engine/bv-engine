#pragma once

#include "DefaultMeshPlugin.h"


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
class DefaultSVGPlugin : public DefaultMeshPlugin
{
protected:
public:

    explicit                                    DefaultSVGPlugin            ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    ~DefaultSVGPlugin          ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

private:

    bool										InitVertexAttributesChannel ( bool recursive );
    void										AddGeometry                 ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, glm::mat4 & transform, bool recursive );

};

} // model
} // bv


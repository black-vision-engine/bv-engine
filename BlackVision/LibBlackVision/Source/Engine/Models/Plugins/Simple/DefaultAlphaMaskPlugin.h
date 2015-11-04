#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultAlphaMaskPluginDesc : public BasePluginDescriptor
{
public:

    DefaultAlphaMaskPluginDesc                                  ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();
};

// ***************************** PLUGIN ********************************** 
class DefaultAlphaMaskPlugin : public BasePlugin< IPlugin >
{
private:

    DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    SizeType                        m_textureWidth;
    SizeType                        m_textureHeight;

public:

    explicit                                    DefaultAlphaMaskPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultAlphaMaskPlugin     ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    SizeType                                    GetAlphaTextureWidth        () const;
    SizeType                                    GetAlphaTextureHeight       () const;

    virtual void                                Update                      ( TimeType t ) override;

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;
    
private:
	void										InitVertexAttributesChannel ();
    void                                        RecalculateUVChannel		();

	//void										RegisterEvaluators			( IPluginPtr prev, const std::vector< std::string > & vsParamNames, const std::vector< std::string > & psParamNames ); 

};

} // model
} // bv

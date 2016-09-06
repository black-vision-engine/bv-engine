#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Events/BaseEvent.h"
#include "Assets/Font/Text.h"



namespace bv { namespace model {


// ***************************** DESCRIPTOR **********************************
class DefaultText3DPluginDesc : public BasePluginDescriptor
{
public:

    DefaultText3DPluginDesc                                     ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();
};

// ***************************** PLUGIN ********************************** 
class DefaultText3DPlugin : public BasePlugin< IPlugin >
{
private:
    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;
	DefaultTransformChannelPtr		m_transformChannel;

    ParamWStringPtr                 m_textParam;

    UInt32		                    m_fontSize;
	UInt32		                    m_outlineSize;

    ParamFloatPtr                   m_spacingParam;
    ParamFloatPtr                   m_alignmentParam;
    ParamFloatPtr                   m_maxTextLengthParam;
	ValueMat4Ptr					m_scaleValue;

    TextConstPtr                    m_text;

    void                                        SetText                     ( const std::wstring & newText );

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;
    void                                        RebuildText                 ();

public:

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

private:
	virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & trans ) const override;

    virtual void                                Update                      ( TimeType t ) override;

	void										ScaleToMaxTextLength		();

public:
	explicit                                    DefaultText3DPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
												~DefaultText3DPlugin        ();

    virtual void								SetPrevPlugin               ( IPluginPtr plugin );

	std::wstring                                GetText                     () const;

    static bool									SetText						( IPluginPtr, const std::wstring& );

};

} // model
} // bv

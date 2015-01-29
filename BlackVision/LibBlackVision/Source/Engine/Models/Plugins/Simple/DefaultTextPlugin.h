#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/Resources/Font/TextHelper.h"

namespace bv { namespace model {

class TextAtlas;

// ***************************** DESCRIPTOR **********************************
class DefaultTextPluginDesc : public BasePluginDescriptor
{
public:

    DefaultTextPluginDesc                                       ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                 ();

    static  std::string                     VertexShaderSource  ();
    static  std::string                     PixelShaderSource   ();

    static  std::string                     TextureName         ();
    static  std::string                     FontFileName        ();
};

// ***************************** PLUGIN ********************************** 
class DefaultTextPlugin : public BasePlugin< IPlugin >
{
private:
    DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    DefaultTexturesDataPtr          m_texturesData;

    unsigned int                    m_texCoordChannelIndex;

    std::wstring                    m_text;
    const TextAtlas *				m_atlas;
    bool                            m_textSet;
	Float32							m_textLength;

    ParamFloatPtr                   m_fontSizeParam;
    ParamFloatPtr                   m_blurSizeParam;
	ParamFloatPtr                   m_outlineSizeParam;
    ParamFloatPtr                   m_spacingParam;
    ParamFloatPtr                   m_alignmentParam;
    ParamFloatPtr                   m_maxTextLengthParam;

    //delegates
    void                                        OnSetText                   ( IEventPtr evt );

    void                                        SetText                     ( const std::wstring & newText );

	void										LoadTexture					(	DefaultTexturesDataPtr,
																				ResourceHandleConstPtr,
																				const std::string &,
																				TextureWrappingMode,
																				TextureWrappingMode,
																				TextureFilteringMode,
																				const glm::vec4 &,
																				DataBuffer::Semantic );

	void										LoadAtlas					( const std::string &, SizeType, SizeType, SizeType );

    virtual bool                                LoadResource                ( IPluginResourceDescrConstPtr resDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

	virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & trans ) const override;

    virtual void                                Update                      ( TimeType t ) override;

    void                                        InitAttributesChannel       ( IPluginPtr prev );

public:
	explicit                                    DefaultTextPlugin           ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
												~DefaultTextPlugin          ();

	static bool									SetText						( IPluginPtr, const std::wstring& );
};

} // model
} // bv
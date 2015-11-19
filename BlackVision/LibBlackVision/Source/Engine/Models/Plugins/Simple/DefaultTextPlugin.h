#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Events/BaseEvent.h"
#include "Assets/Font/TextHelper.h"
#include "TextArranger.h"

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

    static  std::string                     TextureName         ();
};

// ***************************** PLUGIN ********************************** 
class DefaultTextPlugin : public BasePlugin< IPlugin >
{
private:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;
	glm::mat4						m_scaleMat;

    ParamWStringPtr                 m_textParam;
    std::wstring                    m_currentText;
    TextAtlasConstPtr				m_atlas;
	Float32							m_textLength;

    UInt32		                    m_fontSize;
    UInt32		                    m_blurSize;
	UInt32		                    m_outlineSize;

    ParamFloatPtr                   m_spacingParam;
    ParamFloatPtr                   m_alignmentParam;
    ParamFloatPtr                   m_maxTextLengthParam;
    ParamFloatPtr                   m_timeParam;
	ValueMat4Ptr					m_scaleValue;

    TextArranger *                  m_arranger;

    //delegates
    void                                        OnSetText                   ( IEventPtr evt );

    void                                        SetText                     ( const std::wstring & newText, TimeType t = TimeType( 0.f ) );

	void										LoadTexture					(	DefaultTexturesDataPtr,
																				TextureAssetConstPtr,
																				const std::string &,
																				TextureWrappingMode,
																				TextureWrappingMode,
																				TextureFilteringMode,
																				const glm::vec4 &,
																				DataBuffer::Semantic );

	void										LoadAtlas					( const FontAssetDescConstPtr & fontAssetDesc );

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

public:

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

private:

	virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & trans ) const override;

    virtual void                                Update                      ( TimeType t ) override;


	void										ScaleToMaxTextLength		();

public:

	explicit                                    DefaultTextPlugin           ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
												~DefaultTextPlugin          ();

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;

	std::wstring                                GetText                     () const;
    static bool									SetText						( IPluginPtr, const std::wstring& );
};

} // model
} // bv

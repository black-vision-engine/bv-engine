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
#include "Assets/Font/3D/FontAsset3D.h"
#include "Assets/Font/3D/FontAsset3DDesc.h"
#include "Assets/Font/TextHelper.h"



namespace bv { namespace model {


typedef ParamEnum< TextAlignmentType > ParamEnumTAT;
DEFINE_PTR_TYPE( ParamEnumTAT );

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
/// @ingroup PluginsList
class DefaultText3DPlugin : public BasePlugin
{
public:
    struct PARAMS
    {
        static const std::string        TEXT;
        static const std::string        FONT_SIZE;
        static const std::string        NEW_LINE_SIZE;
        static const std::string        SPACING;
        static const std::string        ALIGNEMENT;
        static const std::string        USE_KERNING;
        static const std::string        ALIGN_CHARACTER;
        static const std::string        TEXT_BOX;
        static const std::string        USE_TEXT_BOX;
    };

private:
    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    ParamWStringPtr                 m_textParam;

    ValueBoolPtr                    m_useKerningValue;
    ValueFloatPtr                   m_spacingValue;
    ValueFloatPtr                   m_fontSize;
    ValueFloatPtr                   m_newLineSize;

    ParamEnumTATPtr                 m_alignmentParam;
    ValueIntPtr                     m_alignCharacter;

    FontAsset3DConstPtr             m_fontAsset;

    void                                        SetText                     ( const std::wstring & newText );
    void                                        RebuildText                 ();

public:

    virtual void                                Update                      ( TimeType t ) override;
    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

private:
	virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & trans ) const override;

	void										ScaleToMaxTextLength		();

public:
	explicit                                    DefaultText3DPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
												~DefaultText3DPlugin        ();

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin );

	std::wstring                                GetText                     () const;

    static bool									SetText						( IPluginPtr, const std::wstring& );

};

} // model
} // bv

#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultBlendTexturePluginDesc : public BasePluginDescriptor
{
public:

    DefaultBlendTexturePluginDesc                               ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultBlendTexturePlugin : public BasePlugin
{
public:

    enum BlendingMode
    {
        BM_Normal = 0,
        BM_Lighten,
        BM_Darken,
        BM_Multiply,
        BM_Average,
        BM_Add,
        BM_Substract,
        BM_Difference,
        BM_Negation,
        BM_Exclusion,
        BM_Screen,
        BM_Overlay,
        BM_SoftLight,
        BM_HardLight,
        BM_ColorDodge,
        BM_ColorBurn,
        BM_LinearDodge,
        BM_LinearBurn,
        BM_LinearLight,
        BM_VividLight,
        BM_PinLight,
        BM_HardMix,
        BM_Reflect,
        BM_Glow,
        BM_Phoenix,
        BM_Hue,
        BM_Saturation,
        BM_Color,
        BM_Luminosity,

        BM_Total
    };

    struct PARAMS
    {
        static const std::string        ALPHA;
        static const std::string        BLENDING_MODE;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    SizeType	                    m_textureWidth;
    SizeType	                    m_textureHeight;

public:

    explicit                                    DefaultBlendTexturePlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultBlendTexturePlugin  ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    SizeType									GetTextureWidth             () const;
    SizeType									GetTextureHeight            () const;

    virtual void                                Update                      ( TimeType t ) override;

private:
    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;
};


template<>
inline bool SetParameter< DefaultBlendTexturePlugin::BlendingMode >( IParameterPtr param, TimeType t, const DefaultBlendTexturePlugin::BlendingMode & val )
{
    //return SetSimpleTypedParameter< DefaultCone::DefaultConePlugin::WeightCenter> >( param, t, val );
    typedef ParamEnum< DefaultBlendTexturePlugin::BlendingMode > ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}


} // model
} // bv



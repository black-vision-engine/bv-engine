#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultHeightMapPluginDesc : public BasePluginDescriptor
{
public:

    DefaultHeightMapPluginDesc                                      ();

    virtual IPluginPtr                      CreatePlugin            ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel      ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo         ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                     ();

    static  std::string                     HeightMapTextureName    ();
    static  std::string                     HillTextureName         ();
    static  std::string                     CoveredDistTextureName  ();
    static  std::string                     BackgroundTextureName   ();

};

// ***************************** PLUGIN ********************************** 
class DefaultHeightMapPlugin : public BasePlugin< IPlugin >
{
private:

    enum class TextureSlot : int
    {
        TS_HEIGHT_MAP = 0,
        TS_HILL,
        TS_COVERED_DIST,
        TS_BACKGROUND,

        TS_TOTAL
    };

private:

    DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    DefaultTexturesDataPtr          m_texturesData;

    ParamFloatPtr                   m_hmHeightScale;
    ParamFloatPtr                   m_GroundLevelHeight;
    ParamFloatPtr                   m_MaxHeightValue;

    unsigned int                    m_texCoordChannelIndex;

    const unsigned char *           m_hmRawData;

public:

    explicit                                    DefaultHeightMapPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultHeightMapPlugin     ();

    virtual bool                                LoadResource                ( IPluginResourceDescrConstPtr resDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

    glm::vec2                                   QueryPosition               ( float distInMeter ) const;

private:

    void                                        InitAttributesChannel       ( IPluginPtr prev );
    void                                        SetTextureParams            ( TextureSlot slot, DefaultTextureDescriptor * txDesc ) const;

    float                                       DecodeFixedPoint            ( const unsigned char * data ) const;
    float                                       DecodeHeight                ( const unsigned char * data, float scl, float groundLevel, float maxHeight ) const;
    //// *****************************
//// FIXME: constant
//float decodeHeight( vec4 col )
//{
//	return hmHeightScale * ( decodeFixedPointValue( col.r, col.g, 1.0 / 16.0 ) - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight );
//}
//
//// *****************************
//// FIXME: constant
//float sampleHeight( vec2 uv )
//{
//    float x = ( uv.x * 3840.0 );
//    float w = fract( x );
//
//    //CASE w == 1 - epsilon but sampler samples next texel instead of the current one, that's why we have to force sampler to stay in the left pixel (texelsize * 0.995) at the cost of interpolation errors at the end of texel
//    //but it can be noticed when the magnification ratio is more thann 2000:1 which is highly unlikely here
//
//    if( NVIDIA_PASS )
//    {
////NVIDIA PASS
//        float h0 = decodeHeight( texture( HeightMapTex, uv + vec2( 0.005 / 3840.0, 0.0 )) );
//        float h1 = decodeHeight( texture( HeightMapTex, uv + vec2( 1.0 / 3840.0, 0.0 ) ) );
//
//        return mix( h0, h1, w );
//    }
//    else
//    {
////ATI PASS
//        float h0 = decodeHeight( texture( HeightMapTex, uv ) );
//        float h1 = decodeHeight( texture( HeightMapTex, uv + vec2( 0.995 / 3840.0, 0.0 ) ) );
//
//        return mix( h0, h1, w );
//    }
//}
//
//// *****************************
////
//float filterHeight( vec2 uv, float h )
//{
//    float dx = 1.0 / ( 1920.0 * scale.x ); //FIXME: magic number
//
//    float hklen = kernelHalfLen();
//
//    float sum = h;
//    float w = 1.0;
//
//    int i = 1;
//    while ( hklen > 0.001 )
//    {
//        hklen -= 1.0;
//
//        float tw = 1.0;
//
//        if( hklen < 0.0 )
//        {
//            tw = hklen + 1.0;
//        }
//
//        sum += sampleHeight( uv + vec2( pixelOffset[ i ] * dx, 0.0 ) ) * tw;
//        sum += sampleHeight( uv - vec2( pixelOffset[ i ] * dx, 0.0 ) ) * tw;
//
//        ++i;
//        w += 2.0 * tw;
//    }
//
//    return sum / w;
//}

};

} // model
} // bv

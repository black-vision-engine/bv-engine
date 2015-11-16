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

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    ParamFloatPtr                   m_hmHeightScale;
    ParamFloatPtr                   m_GroundLevelHeight;
    ParamFloatPtr                   m_MaxHeightValue;
    ParamFloatPtr                   m_totalDistInMeters;
    ParamFloatPtr                   m_curDistInMeters;
    ParamVec2Ptr                    m_scale;
    ParamFloatPtr                   m_hmOffsetYInPixels;

    unsigned int                    m_texCoordChannelIndex;
    unsigned int                    m_currTextureIdx;

    const unsigned char *           m_hmRawData;

public:

    explicit                                    DefaultHeightMapPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultHeightMapPlugin     ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

    glm::vec2                                   QueryEdgePosition           ( float distInMeters ) const;

private:

    void                                        InitAttributesChannel       ( IPluginPtr prev );

    float                                       DecodeFixedPoint            ( const unsigned char * data ) const;
    float                                       DecodeHeight                ( const unsigned char * data, float sclHeight, float groundLevel, float maxHeight ) const;
    float                                       SampleHeight                ( float x, float sclHeight, float groundLevel, float maxHeight ) const;
    float                                       FilterHeight                ( float x, float sclHeight, float groundLevel, float maxHeight, float sclX, bool isVS ) const;
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

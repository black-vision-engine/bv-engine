#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFloat.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec2.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamInt.h"

namespace bv {

class LightScatteringEffect : public RenderableEffect
{
private:
    ShaderParamFloat *  m_paramExposure;
    ShaderParamFloat *  m_paramWeight;
    ShaderParamFloat *  m_paramDecay;
    ShaderParamFloat *  m_paramDensity;
    ShaderParamVec2  *  m_paramLightPositionOnScreen;
    ShaderParamFloat *  m_paramNumSamples;

public:

    void                SetExposureVal              ( const IValue * exposureVal );
    void                SetWeightVal                ( const IValue * weightVal );
    void                SetDecayVal                 ( const IValue * decayVal );
    void                SetDensityVal               ( const IValue * densityVal );
    void                SetLightPositionOnScreenVal ( const IValue * lightPositionOnScreenVal );
    void                SetNumSamplesVal            ( const IValue * numSamplesVal );
    void                AddTexture                  ( Texture2DPtr texture );

                        LightScatteringEffect ();
                        ~LightScatteringEffect();

private:

    PixelShader *       CreatePS        ();
    VertexShader *      CreateVS        ();

    TextureSampler *    CreateSampler   ();
};

} //bv

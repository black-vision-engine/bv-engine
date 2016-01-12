#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffectTr.h"
#include "Rendering/Logic/FullScreen/FullscreenEffectData.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Interfaces/IValue.h"

namespace  bv {

class Camera;
class TriangleStrip;
class VertexShader;
class PixelShader;

class SimpleFullscreenEffect : public FullscreenEffectTr
{
private:

    FullscreenEffectData    m_data;

    TriangleStrip *         m_fullscreenQuad;

    Camera *                m_fullscreenCamera;
    
public:

                    SimpleFullscreenEffect      ( const FullscreenEffectData & inputData );
                    ~SimpleFullscreenEffect     ();

    virtual void            Render              ( FullscreenEffectContext * ctx ) override;

    virtual unsigned int    GetNumInputs        () const override;

private:

    TriangleStrip *     CreateFullscreenQuad    ( const FullscreenEffectData & inputData ) const;

    RenderableEffectPtr CreateEffect            ( const FullscreenEffectData & inputData ) const;

    VertexShader *      CreateVertexShader      ( const FullscreenEffectData & inputData ) const;
    PixelShader *       CreatePixelShader       ( const FullscreenEffectData & inputData ) const;

    RenderablePass *    CreateRenderablePass    ( PixelShader * ps, VertexShader * vs, const FullscreenEffectData & inputData ) const;

    TextureSampler *    CreateSampler           ( const std::string & samplerName ) const;

protected:

    virtual void        SynchronizeInputData    ( FullscreenEffectContext * ctx ) override;

    bool                DebugVerifyInput        ( const std::vector< RenderTarget * > * rtVec, unsigned int startIdx );

};

DEFINE_PTR_TYPE(SimpleFullscreenEffect)
DEFINE_CONST_PTR_TYPE(SimpleFullscreenEffect)

} //bv

#pragma once

#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffect.h"


namespace bv {

class RenderTarget;
class RenderLogicContext;

class FullscreenEffectInstance
{
private:

    bool                            m_initialized;

    FullscreenEffectPtr             m_effect;
    FullscreenEffectContext         m_ctx;
    std::vector< RenderTarget * >   m_inputRenderTargets;

public:

                                        FullscreenEffectInstance    ( FullscreenEffectPtr effect );
                                        ~FullscreenEffectInstance   ();

    void                                Render                      ( RenderTarget * output, RenderLogicContext * ctx );

    std::vector< RenderTarget * > *     AccessInputRenderTargets    ();
    void                                SetSyncRequired             ( bool required );

    std::vector< IValuePtr >            GetValues                   () const;

private:

    void                                InitializeGuard             ( RenderLogicContext * ctx );
    void                                RenderImpl                  ( RenderTarget * output, FullscreenEffectContext * ctx );

};

DEFINE_PTR_TYPE(FullscreenEffectInstance)
DEFINE_CONST_PTR_TYPE(FullscreenEffectInstance)

} //bv

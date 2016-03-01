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

    void                        Update                      ();
    void                        Render                      ( RenderTarget * output, RenderLogicContext * ctx );

    void                        UpdateInputRenderTargets    ( const std::vector< RenderTarget * > & renderTargets );

    std::vector< IValuePtr >    GetValues                   () const;

    unsigned int                GetNumInputRenderTargets    () const;
    const RenderTarget *        GetRenderTarget             ( unsigned int i ) const;
    void                        SetRenderTarget             ( unsigned int i, RenderTarget * rt );

    bool                        SetValue                    ( const std::string & name, int value );
    bool                        SetValue                    ( const std::string & name, float value );

private:

    void                        InitializeGuard             ( RenderLogicContext * ctx );
    void                        RenderImpl                  ( RenderTarget * output, FullscreenEffectContext * ctx );

};

DEFINE_PTR_TYPE(FullscreenEffectInstance)
DEFINE_CONST_PTR_TYPE(FullscreenEffectInstance)

} //bv

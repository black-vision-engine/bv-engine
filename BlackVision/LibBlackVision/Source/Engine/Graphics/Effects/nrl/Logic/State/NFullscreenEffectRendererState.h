#pragma once


namespace bv { namespace nrl {

class NFullscreenEffectRendererState
{
private:

    bool m_blendFlag;
    bool m_depthTestFlag;
    bool m_cullFlag;

public:

            NFullscreenEffectRendererState  ();

    bool    GetBlendFlag                    () const;
    bool    GetDepthTestFlag                () const;
    bool    GetCullFlag                     () const;

    void    SetBlendFlag                    ( bool flag );
    void    SetDepthTestFlag                ( bool flag );
    void    SetCullFlag                     ( bool flag );

};

} //nrl
} //bv

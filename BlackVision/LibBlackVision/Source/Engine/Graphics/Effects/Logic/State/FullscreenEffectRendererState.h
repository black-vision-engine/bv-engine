#pragma once


namespace bv { 

class FullscreenEffectRendererState
{
private:

    bool m_blendFlag;
    bool m_depthTestFlag;
    bool m_cullFlag;

public:

            FullscreenEffectRendererState  ();

    bool    GetBlendFlag                    () const;
    bool    GetDepthTestFlag                () const;
    bool    GetCullFlag                     () const;

    void    SetBlendFlag                    ( bool flag );
    void    SetDepthTestFlag                ( bool flag );
    void    SetCullFlag                     ( bool flag );

};


} //bv

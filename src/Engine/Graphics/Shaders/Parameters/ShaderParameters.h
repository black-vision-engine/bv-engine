#pragma once

#include <vector>

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderTextureParameters.h"


namespace bv {

class Texture;

//FIXME: Textura - klasa Texture, nawet niekoniecznie Texture2D trafia tutaj (a w zasadzie wszystkie tekstury, z których korzysta shader)
//FIXME: potem mapowanie texture unitow na konkretne tekstury (to juz shader + pdr) wiazanie z konkretnymi texture unitami odbywa sie juz w pdr - dodajemy do zerowej tekstury
//FIXME: kolejne, ktore sa potrzebne i pobieramy kolejne dopuszczalne teksture unity (trzeba je swoja droga tutaj ograniczyc dynamicznie zgodnie z max texture units do pobrania z GLUtils)
//FIXME: Buffer usage dla texture unitu to jest DYNAMIC_DRAW, to samo trzeba robic dla dynamicznych geometrii, ktore co klatka wymagaja wrzucenia calego kanalu atrybutow !!!!
class ShaderParameters
{
private:

    typedef std::vector< GenericShaderParam * > TShaderParamVec;

private:

    TShaderParamVec             m_shaderParams;
    ShaderTextureParameters     m_textureParams;

public:

                                            ShaderParameters                ();
                                            ~ShaderParameters               ();

    void                                    UpdateParameters                ( RenderableEntity * renderable, Camera * camera );

    void                                    AddParameter                    ( GenericShaderParam * param );

    inline unsigned int                     NumParameters                   () const;

    inline const GenericShaderParam *       GetParam                        ( unsigned int idx ) const;
    inline const GenericShaderParam *       GetParam                        ( const std::string & name ) const;

    inline const ShaderTextureParameters &  GetTextureParameters            () const;
    inline ShaderTextureParameters &        TextureParameters               ();

};

} //bv

#include "ShaderParameters.inl"

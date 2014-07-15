#pragma once

#include <vector>

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"
#include "Engine/Graphics/Resources/Texture2D.h" //FIXME: tymczasowo obslugujemy tylko tekstury 2D, wiec nie trzeba trzymac wektora tekstur, ale docelowo warto miec wektor tekstur tutaj


namespace bv {

class Texture2D;

//FIXME: Textura - klasa Texture, nawet niekoniecznie Texture2D trafia tutaj (a w zasadzie wszystkie tekstury, z których korzysta shader)
//FIXME: potem mapowanie texture unitow na konkretne tekstury (to juz shader + pdr) wiazanie z konkretnymi texture unitami odbywa sie juz w pdr - dodajemy do zerowej tekstury
//FIXME: kolejne, ktore sa potrzebne i pobieramy kolejne dopuszczalne teksture unity (trzeba je swoja droga tutaj ograniczyc dynamicznie zgodnie z max texture units do pobrania z GLUtils)
//FIXME: Buffer usage dla texture unitu to jest DYNAMIC_DRAW, to samo trzeba robic dla dynamicznych geometrii, ktore co klatka wymagaja wrzucenia calego kanalu atrybutow !!!!
class ShaderParameters
{
private:

    typedef std::vector< GenericShaderParam * > TShaderParamVec;
    typedef std::vector< Texture2DPtr >         TTexture2DVec; //FIXME: tymczasowo obslugujemy tylko tekstury 2D, wiec nie trzeba trzymac wektora tekstur, ale docelowo warto miec wektor tekstur tutaj

private:

    TShaderParamVec             m_shaderParams;
    TTexture2DVec               m_textures; //FIXME: tymczasowo obslugujemy tylko tekstury 2D, wiec nie trzeba trzymac wektora tekstur, ale docelowo warto miec wektor tekstur tutaj

public:

                                            ShaderParameters                ();
                                            ~ShaderParameters               ();

    void                                    UpdateParameters                ( RenderableEntity * renderable, Camera * camera );

    void                                    AddParameter                    ( GenericShaderParam * param ); //FIXME: tymczasowo obslugujemy tylko tekstury 2D, wiec nie trzeba trzymac wektora tekstur, ale docelowo warto miec wektor tekstur tutaj
    inline unsigned int                     NumParameters                   () const;

    inline const GenericShaderParam *       GetParam                        ( unsigned int idx ) const;
    inline const GenericShaderParam *       GetParam                        ( const std::string & name ) const;

    void                                    AddTexture                      ( Texture2DPtr texture );
    inline unsigned int                     NumTextures                     () const;

    inline Texture2DPtr                     GetTexture                      ( unsigned int idx ); //FIXME: tymczasowo obslugujemy tylko tekstury 2D, wiec nie trzeba trzymac wektora tekstur, ale docelowo warto miec wektor tekstur tutaj

};

} //bv

#include "ShaderParameters.inl"

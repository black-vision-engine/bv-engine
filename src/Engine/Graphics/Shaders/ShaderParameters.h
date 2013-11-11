#pragma once

#include <vector>

#include "Engine/Graphics/Shaders/ShaderParam.h"


namespace bv {

class Shader;
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

    TShaderParamVec m_shaderParams;    
    Shader *        m_shader;

    std::vector< Texture * >    m_textures;

public:

                                        ~ShaderParameters               ();

    void                                SetOwner                        ( Shader * owner );

    bool                                IsRegistered                    ( const std::string & name ) const;

    void                                RegisterParameter               ( GenericShaderParam * param );
    int                                 NumParameters                   () const;

    GenericShaderParam *                GetParam                        ( int id );
    GenericShaderParam *                GetParam                        ( const std::string & name );

    const std::vector< Texture * > &    Textures                        () const;
    void                                AddTexture                      ( Texture * tex );

    void                                SetValue                        ( const std::string & name , const char * pBuf );

    void                                Update                          ( /* ???? */ );

private:

    GenericShaderParam *                FindParam                       ( const std::string & name ) const;

};

}

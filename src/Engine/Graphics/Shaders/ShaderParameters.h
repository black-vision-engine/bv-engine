#pragma once

#include <vector>

#include "Engine/Graphics/Shaders/ShaderParam.h"
#include "Engine/Graphics/Shaders/ShaderTextureParameters.h"


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

    typedef std::vector< UniformShaderParam * > TShaderParamVec;

private:

    TShaderParamVec             m_shaderParams;    
    Shader *                    m_shader;

    ShaderTextureParameters     m_textureParams;

public:

                                        ~ShaderParameters               ();

    void                                SetOwner                        ( Shader * owner );

    bool                                IsRegistered                    ( const std::string & name ) const;

    void                                RegisterParameter               ( UniformShaderParam * param );
    int                                 NumParameters                   () const;

    UniformShaderParam *                GetParam                        ( int id );
    UniformShaderParam *                GetParam                        ( const std::string & name );

    const ShaderTextureParameters &     GetTextureParameters            () const;
    ShaderTextureParameters &           TextureParameters               ();

    void                                SetValue                        ( const std::string & name , const char * pBuf );

    void                                Update                          ( /* ???? */ );

private:

    UniformShaderParam *                FindParam                       ( const std::string & name ) const;

};

}

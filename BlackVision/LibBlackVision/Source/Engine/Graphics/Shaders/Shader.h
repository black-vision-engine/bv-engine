#pragma once

#include "Engine/Graphics/Shaders/Parameters/ShaderParameters.h"
#include "Engine/Graphics/Shaders/TextureSampler.h"


namespace bv {

//FIXME: sampler tekstury powinien byc osobna klasa
//FIXME: w shaderze powinna byc trzymana lista samplerow (ma byc ich tyle, ile tekstur aktywnych ten shader moze rownolegle renderowac)
//FIXME: ta lista to tez ewentualnie moze byc oddzielna klasa
//FIXME: pojedynczy sampler powinien miec (typ (ze 1D, 2D, 3D, cubic), podawac texture unit dla siebie, wrap mode, anisotropy, lodbias (niekoniecznie bez mipmap), filter, border color)
//FIXME: taki sampler jest statycznie powiazany z tekstura poki co czyli jego parametry sa ustalane statycznie przy tworzeniu sceny - ale moze bedzie trzeba pozwolic na ich zmiane
//FIXME: juz w trakcie dzialania renderera
class RenderableEntity;
class Camera;

class Shader
{
protected:

    ShaderParameters *        m_parameters;
    std::string               m_programSurce;
    
    std::vector< const TextureSampler * >   m_textureSamplers;

protected:

                        Shader              ( const std::string & programSource, ShaderParameters * params );
                        ~Shader             ();

public:  

    ShaderParameters *  GetParameters       ();

    void                AddTextureSampler   ( const TextureSampler * sampler );

    const std::string & ProgramSource       () const;

    void                Update              ( RenderableEntity * renderable, Camera * camera );

    const std::vector< const TextureSampler * > &  Samplers  () const;

};

} // bv

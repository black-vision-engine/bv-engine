#pragma once

#include "Engine/Graphics/Shaders/ShaderParameters.h"
#include "Engine/Graphics/Shaders/TextureSampler.h"

namespace bv {

//FIXME: sampler tekstury powinien byc osobna klasa
//FIXME: w shaderze powinna byc trzymana lista samplerow (ma byc ich tyle, ile tekstur aktywnych ten shader moze rownolegle renderowac)
//FIXME: ta lista to tez ewentualnie moze byc oddzielna klasa
//FIXME: pojedynczy sampler powinien miec (typ (ze 1D, 2D, 3D, cubic), podawac texture unit dla siebie, wrap mode, anisotropy, lodbias (niekoniecznie bez mipmap), filter, border color)
//FIXME: taki sampler jest statycznie powiazany z tekstura poki co czyli jego parametry sa ustalane statycznie przy tworzeniu sceny - ale moze bedzie trzeba pozwolic na ich zmiane
//FIXME: juz w trakcie dzialania renderera
class ShaderParamUpdater;

class Shader
{
protected:

    ShaderParameters *        m_parameters;
    std::string               m_programSurce;
    ShaderParamUpdater *      m_paramUpdater;
    
    //FIXME: who owns TextureSamplers?
    std::vector< const TextureSampler * >   m_textureSamplers;

protected:

                        Shader                      ( const std::string& programSource );
                        ~Shader                     ();

public:  

    //FIXME: some additional code should be added to make paramater creation a bit less error prone (right now we give away parameters and have no control over them)
    ShaderParameters *  GetOrCreateShaderParameters ();
    void                RegisterUpdater             ( ShaderParamUpdater* updater );
    void                AddTextureSampler           ( const TextureSampler* sampler );
    ShaderParameters *  Parameters                  ();

    const std::string & ProgramSource               () const;

    void                Update                      ();

    const std::vector< const TextureSampler * > &  Samplers  () const;

};

} // bv

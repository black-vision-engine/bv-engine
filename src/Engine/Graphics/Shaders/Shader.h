#pragma once

#include "ShaderParameters.h"


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

    ShaderParameters *      m_parameters;
    std::string             m_programSurce;
    ShaderParamUpdater*     m_paramUpdater;
    
protected:

    Shader  ( const std::string& programSource );
    ~Shader ();

public:  

    void                Update              ();

    ShaderParameters *  GetOrCreateShaderParameters();
    void                RegisterUpdater     ( ShaderParamUpdater* updater );
    ShaderParameters *  Parameters          ();

    const std::string & ProgramSource       () const;


//FIXME: textures and samplers
};

} // bv

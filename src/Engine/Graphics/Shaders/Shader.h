#pragma once

#include "ShaderParameters.h"


namespace bv {

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

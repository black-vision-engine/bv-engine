#pragma once

namespace bv { namespace model {

template< class IFace >
class ShaderChannel : public IFace
{
    std::string                     m_shaderFile;
    std::vector<IValue*>            m_values; 
    std::vector<IParameter*>        m_params;

public:

    virtual const std::string&                      GetShaderFile               () const { return m_shaderFile; }
    virtual const std::vector<IValue*> &            GetValuesList               () const { return m_values; }
    virtual const std::vector<IParameter*> &        GetParametersList           () const { return m_params; }

    explicit            ShaderChannel( const std::string& shaderFile )
        : m_shaderFile( shaderFile )
    {}
};

} // model
} // bv
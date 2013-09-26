#pragma once

namespace bv { namespace model {

template< class IFace, class ParameterDescriptor  >
class ShaderChannel : public IFace
{
protected:

    typedef ParameterDescriptor     ParamDesc;

    std::string                     m_shaderFile;
    std::vector<IValue*>            m_values; 
    std::vector<IParameter*>        m_params;

public:

    virtual void                                    Update                      ( float ) {}
    virtual bool                                    IsReadOnly                  () const { return true; }

    virtual const std::string&                      GetShaderFile               () const { return m_shaderFile; }
    virtual const std::vector<IValue*> &            GetValuesList               () const { return m_values; }
    virtual const std::vector<IParameter*> &        GetParametersList           () const { return m_params; }

    void                                            RegisterValue               ( IValue * v ) { m_values.push_back( v ); }

    explicit            ShaderChannel( const std::string& shaderFile )
        : m_shaderFile( shaderFile )
    {}
};

} // model
} // bv
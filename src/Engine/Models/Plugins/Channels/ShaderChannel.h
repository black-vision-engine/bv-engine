#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "System/BasicTypes.h"
#include "System/FileIO.h"


namespace bv { namespace model {

class IValue;

template< class IFace, class ParameterDescriptor  >
class ShaderChannel : public IFace
{
protected:

    typedef ParameterDescriptor     ParamDesc;

    std::string                     m_shaderSource;
    std::vector< IValue* >          m_values; 

public:

    virtual void                                    Update                      ( TimeType ) 
    {
    }

    virtual bool                                    IsReadOnly                  () const { return true; }

    virtual const std::string &                     GetShaderSource             () const { return m_shaderSource; }
    virtual const std::vector< IValue* > &          GetValuesList               () const { return m_values; }

    void                                            RegisterValue               ( IValue * v ) { m_values.push_back( v ); }

    // *********************************
    //
    explicit            ShaderChannel( const std::string & shaderFile )
    {
        std::stringstream shaderSource;

        ReadFile( shaderSource, shaderFile );

        m_shaderSource = shaderSource.str(); 
    }

};

} // model
} // bv

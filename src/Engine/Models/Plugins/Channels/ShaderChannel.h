#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IValue.h"
#include "System/BasicTypes.h"
#include "System/FileIO.h"


namespace bv { namespace model {

template< typename ShaderChannelIface >
class ShaderChannel : public ShaderChannelIface
{
protected:

    std::string                 m_shaderSource;
    const IValueSet *           m_values; 

public:

    explicit                                ShaderChannel               ( const std::string & shaderFile, const IValueSet * valueSet );

    virtual const std::string &             GetShaderSource             () const override;
    virtual bool                            IsReadOnly                  () const override;  //Should also be in IChannel
    virtual void                            PostUpdate                  () override;        //Should also be in IChannel

    virtual const std::vector< IValue* > &  GetValues                   () const override;
    virtual IValue *                        GetValue                    ( const std::string & name ) const override;

};

} // model
} // bv

#include "ShaderChannel.inl"

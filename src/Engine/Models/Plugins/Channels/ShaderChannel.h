#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"
#include "System/BasicTypes.h"
#include "System/FileIO.h"


namespace bv { namespace model {

class IValue;

template< typename ShaderChannelIface >
class ShaderChannel : public ShaderChannelIface
{
protected:

    std::string                 m_shaderSource;
    std::vector< IValue * > *   m_values; 

public:

    explicit                            ShaderChannel               ( const std::string & shaderFile, std::vector< IValue * > * values );

    virtual const std::string &         GetShaderSource             () const override;
    virtual bool                        IsReadOnly                  () const override;
    virtual void                        PostUpdate                  () override;      //Should also be in IChannel

    virtual std::vector< IValue* > &    GetValues                   () override;

};

} // model
} // bv

#include "ShaderChannel.inl"

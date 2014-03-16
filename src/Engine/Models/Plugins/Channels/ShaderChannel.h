#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"
#include "Engine/Interfaces/IValue.h"
#include "System/BasicTypes.h"
#include "System/FileIO.h"


namespace bv { namespace model {

template< typename ShaderChannelIface >
class ShaderChannel : public ShaderChannelIface
{
protected:

    std::string         m_shaderSource;
    const IValueSet  *  m_values; 

public:

    explicit                                            ShaderChannel               ( const std::string & shaderFile, const IValueSet * valueSet );

    virtual const std::string &                         GetShaderSource             () const override;
    virtual bool                                        IsReadOnly                  () const override;  //Should also be in IChannel
    virtual void                                        PostUpdate                  () override;        //Should also be in IChannel

    virtual const std::vector< const bv::IValue * > &   GetValues                   () const override;
    virtual const bv::IValue *                          GetValue                    ( const std::string & name ) const override;

    virtual ITexturesData *                             GetTexturesData             () const override;

};

} // model
} // bv

#include "ShaderChannel.inl"

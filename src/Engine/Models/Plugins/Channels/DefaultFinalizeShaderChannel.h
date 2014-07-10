#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cassert>

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"

#include "Engine/Interfaces/IValue.h"

#include "System/BasicTypes.h"


namespace bv { namespace model {

template< typename ShaderChannelIface >
class DefaultFinalizeShaderChannel : public ShaderChannelIface
{
protected:

    std::shared_ptr< ShaderChannelIface >   m_channel;

public:

    explicit                                            DefaultFinalizeShaderChannel    ( std::shared_ptr< ShaderChannelIface > channel );
                                                        ~DefaultFinalizeShaderChannel   ();

    virtual bool                                        IsReadOnly                      () const override;  //Should also be in IChannel
    virtual void                                        PostUpdate                      () override;        //Should also be in IChannel

    virtual const std::vector< bv::IValueConstPtr > &   GetValues                       () const override;
    virtual bv::IValueConstPtr                          GetValue                        ( const std::string & name ) const override;

    virtual ITexturesDataConstPtr                       GetTexturesData                 () const override;

};

} // model
} // bv

#include "DefaultFinalizeShaderChannel.inl"

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <cassert>

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"

#include "Engine/Interfaces/IValue.h"

#include "CoreDEF.h"


namespace bv { namespace model {

typedef std::map< std::vector< std::string >, std::string >  PluginUIDHashMap;

template< typename ShaderChannelIface >
class DefaultFinalizeShaderChannel : public ShaderChannelIface
{
protected:

    std::shared_ptr< ShaderChannelIface >   m_channel;
    mutable std::string                     m_shaderSource;
    std::string                             m_shadersDir;

    static std::vector< std::vector< std::string > >    ms_acceptedPluginLists;
    static std::vector< std::string >                   ms_baseShaderFileNames;

public:

    explicit                                            DefaultFinalizeShaderChannel    ( std::shared_ptr< ShaderChannelIface > channel, const std::string & shadersDir );
                                                        ~DefaultFinalizeShaderChannel   ();

    virtual bool                                        IsReadOnly                      () const override;  //Should also be in IChannel
    virtual void                                        PostUpdate                      () override;        //Should also be in IChannel

    virtual const std::vector< bv::IValueConstPtr > &   GetValues                       () const override;
    virtual bv::IValueConstPtr                          GetValue                        ( const std::string & name ) const override;

    virtual const std::string &                         GetShaderSource                 () const override;
    virtual ITexturesDataConstPtr                       GetTexturesData                 () const override;

    void                                                RegenerateShaderSource          ( const std::vector< std::string > & uids ) const;

protected:

    virtual std::string                                 GetShaderSource                 ( const std::vector< std::string > & uids ) const = 0;

    static const std::vector< std::vector< std::string > > &    GetAcceptedPluginLists  ();
    static const std::vector< std::string > &                   GetBaseShaderFileNames  ();
    const std::string &                                         ShaderStorageDirectory  () const;

};

} // model
} // bv

#include "DefaultFinalizeShaderChannel.inl"

#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cassert>


#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"

#include "Engine/Interfaces/IValue.h"

#include "System/BasicTypes.h"


namespace bv { namespace model {

typedef std::map< std::vector< std::string >, std::string >  PluginUIDHashMap;

template< typename ShaderChannelIface >
class DefaultFinalizeShaderChannel : public ShaderChannelIface
{
protected:

    std::shared_ptr< ShaderChannelIface >   m_channel;
    std::string                             m_shaderSource;

    static std::vector< std::vector< std::string > >    ms_acceptedPluginLists;
    static std::vector< std::string >                   ms_baseShaderFileNames;

public:

    explicit                                            DefaultFinalizeShaderChannel    ( std::shared_ptr< ShaderChannelIface > channel );
                                                        ~DefaultFinalizeShaderChannel   ();

    virtual bool                                        IsReadOnly                      () const override;  //Should also be in IChannel
    virtual void                                        PostUpdate                      () override;        //Should also be in IChannel

    virtual const std::vector< bv::IValueConstPtr > &   GetValues                       () const override;
    virtual bv::IValueConstPtr                          GetValue                        ( const std::string & name ) const override;

    virtual const std::string &                         GetShaderSource                 () const override;
    virtual ITexturesDataConstPtr                       GetTexturesData                 () const override;

    void                                                ReGenerateSource                ( const std::vector< std::string > & uids );

protected:

    virtual std::string                                 GetShaderSource                 ( const std::vector< std::string > & uids ) const = 0;

    static const std::vector< std::vector< std::string > > &    GetAcceptedPluginLists  ();
    static const std::vector< std::string > &                   GetBaseShaderFileNames  ();

};

} // model
} // bv

#include "DefaultFinalizeShaderChannel.inl"

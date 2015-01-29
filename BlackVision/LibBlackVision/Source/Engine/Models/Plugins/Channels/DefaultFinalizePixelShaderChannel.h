#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizePixelShaderChannel : public DefaultFinalizeShaderChannel< IPixelShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IPixelShaderChannel > Parent;

private:

    static PluginUIDHashMap    ms_pixelShaderMapping;

private:

    void     InitializePixelShaderMapping       () const;

public:

        DefaultFinalizePixelShaderChannel       ( IPixelShaderChannelPtr channel, const std::string & shadersDir );
        ~DefaultFinalizePixelShaderChannel      ();

        virtual RendererContextConstPtr  GetRendererContext  () const override;

protected:

        virtual std::string     GetShaderSource ( const std::vector< std::string > & uids ) const override;
};

DEFINE_PTR_TYPE(DefaultFinalizePixelShaderChannel)

} //model
} //bv